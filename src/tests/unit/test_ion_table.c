#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../ion_table.h"

void
test_create_table_001(
) {
	db_query_mm_t			mem_man;
	ion_table_executable_t	plan;
	ion_table_instruction_t	instructions[]	= {
			{ION_TABLE_OPCODE_DEFINE_ATTRIBUTE, { .u_integer = ION_TABLE_ATTRIBUTE_TYPE_INT }, { .u_integer = sizeof(int) }, { .u_integer = 0 }},
			{ION_TABLE_OPCODE_DEFINE_ATTRIBUTE, { .u_integer = ION_TABLE_ATTRIBUTE_TYPE_INT }, { .u_integer = sizeof(int) }, { .u_integer = 0 }},
			{ION_TABLE_OPCODE_DEFINE_ATTRIBUTE, { .u_integer = ION_TABLE_ATTRIBUTE_TYPE_INT }, { .u_integer = sizeof(int) }, { .u_integer = 0 }},
	};
	int						size			= 1000;
	char					memory[size];

	init_query_mm(&mem_man, memory, size);

	ion_table_init_executable(&plan, instructions, &mem_man);
}

void
run_all_ion_table_tests(
	void
) {
	test_create_table_001();
}
