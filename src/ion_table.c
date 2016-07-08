#include "ion_table.h"

void // TODO: Make this an error.
ion_table_init_executable(
	ion_table_executable_t	*plan,
	ion_table_instruction_t	*instructions,
	db_query_mm_t			*mem_man
) {
	plan->instructions	= instructions;
	plan->mem_man		= mem_man;
	plan->last_opcode	= ION_TABLE_OPCODE_NONE;
	plan->stop			= false;
}

// TODO: We need to actually.
int	// TODO: ion_bool_t
ion_table_execute(
		ion_table_executable_t *plan,
		ion_table_tuple_t *tuple
)
{
#define CUROP plan->instructions[plan->current_instr_idx]
	/* The big loop where all instructions are processed. */
	while (!plan->stop) {
		switch (CUROP.opcode) {
			case ION_TABLE_OPCODE_DEFINE_ATTRIBUTE: {
				ion_table_schema_item_t *item	= NULL;
				if (ION_TABLE_OPCODE_DEFINE_ATTRIBUTE == plan->last_opcode) {
					item = db_qmm_bextend(plan->mem_man, sizeof(ion_table_schema_item_t));
				}
				else {
					item = db_qmm_balloc(plan->mem_man, sizeof(ion_table_schema_item_t));
				}
				item->type	= (ion_table_attribute_type_t) CUROP.p1.u_integer;
				item->size	= (ion_table_attribute_size_t) CUROP.p2.u_integer;
			} break;
			default:
				return -1;
		}
		plan->last_opcode	= plan->instructions[plan->current_instr_idx].opcode;
		plan->current_instr_idx++;
	}

	return 0;
}
