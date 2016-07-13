/******************************************************************************/
/**
@file
@author		Graeme Douglas, Wade Penson
@brief		Virtual Machine Executor (VME) used to execute query plans.
@copyright	Copyright 2016
				The University of British Columbia,
				IonDB Project Contributors (see AUTHORS.md)
@par
			Licensed under the Apache License, Version 2.0 (the "License");
			you may not use this file except in compliance with the License.
			You may obtain a copy of the License at
					http://www.apache.org/licenses/LICENSE-2.0
@par
			Unless required by applicable law or agreed to in writing,
			software distributed under the License is distributed on an
			"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
			either express or implied. See the License for the specific
			language governing permissions and limitations under the
			License.
*/
/******************************************************************************/

#include "vme.h"
#include "types.h"

void
ion_table_init_executable(
	ion_table_executable_t	*plan,
	ion_table_instruction_t	*instructions,
	db_query_mm_t			*mem_man
) {
	plan->instructions	= instructions;
	plan->current_instr_idx = 0;
	plan->mem_man		= mem_man;
	plan->prev_opcode	= ION_TABLE_OPCODE_NOP;
	plan->stop			= ION_TABLE_FALSE;
	plan->count			= 0;
	plan->pointer		= NULL;
}

ion_table_status_t
ion_table_execute(
	ion_table_executable_t *plan
	//ion_table_tuple_t *tuple // TODO: This is not needed for updates. Move this to separate function, pointer in mem_man, or something
)
{
/**
@brief	The current instruction being executed.
*/
#define CUROP plan->instructions[plan->current_instr_idx]
	plan->stop = ION_TABLE_FALSE;

	/* The big loop where all instructions are processed. */
	while (ION_TABLE_FALSE == plan->stop) {
		/***************************************************************************************************************
		 * Start of operation switch statement
		 **************************************************************************************************************/
		switch (CUROP.opcode) {
			case ION_TABLE_OPCODE_DEFINE_ATTRIBUTE: {
				ion_table_schema_item_t *item = NULL;

				if (ION_TABLE_OPCODE_DEFINE_ATTRIBUTE == plan->prev_opcode) {
					item = db_qmm_bextend(plan->mem_man, sizeof(ion_table_schema_item_t));
					if (NULL == item) {
						return ION_TABLE_STATUS_ERROR(ION_TABLE_ERROR_OUT_OF_MEMORY);
					}

					plan->count++;
				}
				else {
					item = db_qmm_balloc(plan->mem_man, sizeof(ion_table_schema_item_t));
					if (NULL == item) {
						return ION_TABLE_STATUS_ERROR(ION_TABLE_ERROR_OUT_OF_MEMORY);
					}

					plan->count = 1;
					plan->pointer = item;
				}

				item->type = (ion_table_attribute_type_t) CUROP.p1.u_integer;
				item->attribute_size = (ion_table_attribute_size_t) CUROP.p2.u_integer;
				item->name = CUROP.p3.pointer;
				item->name_length = (uint8_t) (strlen(item->name) + 1);

				break;
			}
			case ION_TABLE_OPCODE_CREATE_TABLE: {
				ion_table_schema_t schema;
				schema.num_attributes = (ion_table_schema_count_t) plan->count;
				schema.id = 0; // TODO
				schema.items = plan->pointer;

				if (ion_table_create_schema((char *) CUROP.p3.pointer, &schema) != ION_TABLE_ERROR_OK) {
					return ION_TABLE_STATUS_ERROR(ION_TABLE_ERROR_TABLE_ALREADY_EXISTS);
				}

				if (ion_table_free_schema_items(&schema, plan->mem_man) != ION_TABLE_ERROR_OK) {
					return ION_TABLE_STATUS_ERROR(ION_TABLE_ERROR_FAILED_TO_FREE_MEMORY);
				}

				break;
			}
			case ION_TABLE_OPCODE_NOP: {
				break;
			}
			case ION_TABLE_OPCODE_HALT: {
				plan->stop = ION_TABLE_TRUE;
				break;
			}
			default: {
				return ION_TABLE_STATUS_ERROR(ION_TABLE_ERROR_UNDEFINED_OPCODE);
			}
		}
		/***************************************************************************************************************
 		 * End of operation switch statement
 		 **************************************************************************************************************/

		/* Store previously executed instruction and then increment the instruction counter. */
		plan->prev_opcode = plan->instructions[plan->current_instr_idx].opcode;
		plan->current_instr_idx++;
	}

	return ION_TABLE_STATUS_OK(0);
}
