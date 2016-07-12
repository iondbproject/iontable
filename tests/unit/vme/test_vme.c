/******************************************************************************/
/**
@file
@author		Graeme Douglas, Wade Penson
@brief
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

#include "test_vme.h"

void
test_create_table_001(
	planck_unit_test_t *tc
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

	PLANCK_UNIT_ASSERT_TRUE(tc, 1);
}

planck_unit_suite_t *
vme_getsuite(
) {
	planck_unit_suite_t *suite = planck_unit_new_suite();

	PLANCK_UNIT_ADD_TO_SUITE(suite, test_create_table_001);

	return suite;
}

void
run_all_tests_vme(
	void
) {
	planck_unit_suite_t *suite = vme_getsuite();
	planck_unit_run_suite(suite);
	planck_unit_destroy_suite(suite);
}
