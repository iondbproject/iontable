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
	plan->mem_man		= mem_man;
	plan->prev_opcode	= ION_TABLE_OPCODE_NOP;
	plan->stop			= ION_TABLE_FALSE;
}

ion_table_status_t
ion_table_execute(
	ion_table_executable_t *plan,
	ion_table_tuple_t *tuple // TODO: This is not needed for updates. Use a double pointer instead?
)
{
/**
@brief	The current instruction being executed.
*/
#define CUROP plan->instructions[plan->current_instr_idx]

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
				}
				else {
					item = db_qmm_balloc(plan->mem_man, sizeof(ion_table_schema_item_t));
				}
				item->type	= (ion_table_attribute_type_t) CUROP.p1.u_integer;
				item->attribute_size	= (ion_table_attribute_size_t) CUROP.p2.u_integer;

				// TODO: Third parameter is attribute name

				break;
			}
			case ION_TABLE_CREATE_TABLE: {

				break;
			}
			case ION_TABLE_OPCODE_NOP: {
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
