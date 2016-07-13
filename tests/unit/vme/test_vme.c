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
#include "../../../src/types.h"

char *tables[] = {"table1", "table2"};
const int tables_length = sizeof(tables) / sizeof(const char *);

char *attributes[] = {"one", "two"};
const int attributes_length = sizeof(tables) / sizeof(const char *);

static ion_table_bool_e
run_test_setup() {
	int i;
	for (i = 0; i < tables_length; i++) {
		char temp_string[strlen(tables[i]) + 5];
		sprintf(temp_string, "%s.sch", tables[i]);
		remove(temp_string);
	}

	remove(ION_MASTER_TABLE_FILENAME);
	remove("1.bpt");
	remove("1.val");

	return ION_TABLE_TRUE;
}

void
test_create_table_001(
	planck_unit_test_t *tc
) {
	PLANCK_UNIT_ASSERT_TRUE(tc, run_test_setup() == ION_TABLE_TRUE);

	db_query_mm_t			mem_man;
	ion_table_executable_t	plan;
	ion_table_instruction_t	instructions[]	= {
		{ION_TABLE_OPCODE_DEFINE_ATTRIBUTE,
			{ .u_integer = ION_TABLE_ATTRIBUTE_TYPE_INT },
			{ .u_integer = sizeof(int) },
			{ .pointer = attributes[0] }},
		{ION_TABLE_OPCODE_CREATE_TABLE,
			{ .u_integer = 0 },
			{ .u_integer = 0 },
			{ .pointer = tables[0] }},
		{ION_TABLE_OPCODE_HALT,
			{ .u_integer = 0 },
			{ .u_integer = 0 },
			{ .u_integer = 0 }}
	};
	int						size			= 1000;
	char					memory[size];

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 1, init_query_mm(&mem_man, memory, size));

	ion_table_init_executable(&plan, instructions, &mem_man);
	ion_table_status_t status = ion_table_execute(&plan);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, status.error);

	ion_table_schema_t loaded_schema;
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_load_schema(tables[0], &loaded_schema, &mem_man));

	/* id == 0 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 1, loaded_schema.id);

	/* num_attributes == 1 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 1, loaded_schema.num_attributes);

	/* Attribute 1, name_length == 4 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, (int) strlen(attributes[0]) + 1, loaded_schema.items[0].name_length);

	/* Attribute 1, name == "one" */
	PLANCK_UNIT_ASSERT_STR_ARE_EQUAL(tc, attributes[0], loaded_schema.items[0].name);

	/* Attribute 1, type == ION_TABLE_ATTRIBUTE_TYPE_INT */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ATTRIBUTE_TYPE_INT, loaded_schema.items[0].type);

	/* Attribute 1, size == sizeof(int) */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, sizeof(int), loaded_schema.items[0].attribute_size);
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
