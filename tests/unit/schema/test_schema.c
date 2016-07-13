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

#include "test_schema.h"
#include "../../../src/types.h"

/* Build up schema in memory. */
char *tables[] = {"table1", "table2"};
const int tables_length = sizeof(tables) / sizeof(const char *);

ion_table_schema_item_t items[] = {
	{
		.name_length = 4,
		.name = "one",
		.attribute_size = (ion_table_attribute_type_t) ION_TABLE_ATTRIBUTE_TYPE_INT,
		.type = (ion_table_attribute_size_t) sizeof(int)
	},
	{
		.name_length = 4,
		.name = "two",
		.attribute_size = (ion_table_attribute_type_t) ION_TABLE_ATTRIBUTE_TYPE_INT,
		.type = (ion_table_attribute_size_t) sizeof(int)
	}
};

ion_table_schema_t schema = {.id = 1, .num_attributes = 2, .items = items};

static ion_table_bool_e
run_test_setup() {
	int i;
	for (i = 0; i < tables_length; i++) {
		char temp_string[strlen(tables[i]) + 5];
		sprintf(temp_string, "%s.sch", tables[i]);
		remove(temp_string);
	}

	return ION_TABLE_TRUE;
}

void
test_save_schema(
	planck_unit_test_t *tc
) {
	PLANCK_UNIT_ASSERT_TRUE(tc, run_test_setup() == ION_TABLE_TRUE);

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_create_schema(tables[0], &schema));

	char table1[strlen(tables[0]) + 5];
	sprintf(table1, "%s.sch", tables[0]);

	/* Open schema file and check to make sure data is correct. */
	FILE *fp = fopen(table1, "rb");
	PLANCK_UNIT_ASSERT_TRUE(tc, NULL != fp);

	rewind(fp);

	char buffer[10];
	/* id == 1 */
	PLANCK_UNIT_ASSERT_TRUE(tc, fread(buffer, 2, 1, fp) == 1);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, schema.id, (uint16_t) *buffer);

	/* num_attributes == 2 */
	PLANCK_UNIT_ASSERT_TRUE(tc, fread(buffer, sizeof(ion_table_schema_count_t), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, schema.num_attributes, (ion_table_schema_count_t) *buffer);

	/* Attribute 1, name_length == 4 */
	PLANCK_UNIT_ASSERT_TRUE(tc, fread(buffer, sizeof(items[0].name_length), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[0].name_length, (uint8_t) *buffer);

	/* Attribute 1, name == "one" */
	PLANCK_UNIT_ASSERT_TRUE(tc, fread(buffer, items[0].name_length, 1, fp) == 1);
	PLANCK_UNIT_ASSERT_STR_ARE_EQUAL(tc, items[0].name, buffer);

	/* Attribute 1, type == ION_TABLE_ATTRIBUTE_TYPE_INT */
	PLANCK_UNIT_ASSERT_TRUE(tc, fread(buffer, sizeof(ion_table_attribute_type_t), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[0].type, (ion_table_attribute_type_t) *buffer);

	/* Attribute 1, size == sizeof(int) */
	PLANCK_UNIT_ASSERT_TRUE(tc, fread(buffer, sizeof(ion_table_attribute_size_t), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[0].attribute_size, (ion_table_attribute_size_t) *buffer);

	/* Attribute 2, name_length == 4 */
	PLANCK_UNIT_ASSERT_TRUE(tc, fread(buffer, sizeof(items[1].name_length), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[1].name_length, (uint8_t) *buffer);

	/* Attribute 2, name == "two" */
	PLANCK_UNIT_ASSERT_TRUE(tc, fread(buffer, items[0].name_length, 1, fp) == 1);
	PLANCK_UNIT_ASSERT_STR_ARE_EQUAL(tc, items[1].name, buffer);

	/* Attribute 2, type == ION_TABLE_ATTRIBUTE_TYPE_INT */
	PLANCK_UNIT_ASSERT_TRUE(tc, fread(buffer, sizeof(ion_table_attribute_type_t), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[1].type, (ion_table_attribute_type_t) *buffer);

	/* Attribute 2, size == sizeof(int) */
	PLANCK_UNIT_ASSERT_TRUE(tc, fread(buffer, sizeof(ion_table_attribute_size_t), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[1].attribute_size, (ion_table_attribute_size_t) *buffer);

	PLANCK_UNIT_ASSERT_TRUE(tc, fclose(fp) == 0);
}

void
test_load_schema(
	planck_unit_test_t *tc
) {
	PLANCK_UNIT_ASSERT_TRUE(tc, run_test_setup() == ION_TABLE_TRUE);

	char table1[strlen(tables[0]) + 5];
	sprintf(table1, "%s.sch", tables[0]);

	/* Open schema file and check to make sure data is correct. */
	FILE *fp = fopen(table1, "wb+");
	PLANCK_UNIT_ASSERT_TRUE(tc, NULL != fp);

	/* Write out schema/table ID and the number of attributes it has. */
	PLANCK_UNIT_ASSERT_TRUE(tc, fwrite(&(schema.id), 2, 1, fp) == 1);
	PLANCK_UNIT_ASSERT_TRUE(tc, fwrite(&(schema.num_attributes), sizeof(ion_table_schema_count_t), 1, fp) == 1);

	/* Write out item 1. */
	PLANCK_UNIT_ASSERT_TRUE(tc, fwrite(&(int){items[0].name_length}, sizeof(items[0].name_length), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_TRUE(tc, fwrite(items[0].name, items[0].name_length, 1, fp) == 1);
	PLANCK_UNIT_ASSERT_TRUE(tc, fwrite(&(items[0].type), sizeof(ion_table_attribute_type_t), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_TRUE(tc, fwrite(&(items[0].attribute_size), sizeof(ion_table_attribute_size_t), 1, fp) == 1);

	/* Write out item 2. */
	PLANCK_UNIT_ASSERT_TRUE(tc, fwrite(&(int){items[1].name_length}, sizeof(items[1].name_length), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_TRUE(tc, fwrite(items[1].name, items[1].name_length, 1, fp) == 1);
	PLANCK_UNIT_ASSERT_TRUE(tc, fwrite(&(items[1].type), sizeof(ion_table_attribute_type_t), 1, fp) == 1);
	PLANCK_UNIT_ASSERT_TRUE(tc, fwrite(&(items[1].attribute_size), sizeof(ion_table_attribute_size_t), 1, fp) == 1);

	PLANCK_UNIT_ASSERT_TRUE(tc, fclose(fp) == 0);

	db_query_mm_t			mem_man;
	int						size			= 100;
	char					memory[size];
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 1, init_query_mm(&mem_man, memory, size));

	ion_table_schema_t loaded_schema;

	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_load_schema(tables[0], &loaded_schema, &mem_man));

	/* id == 1 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, schema.id, loaded_schema.id);

	/* num_attributes == 2 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, schema.num_attributes, loaded_schema.num_attributes);

	/* Attribute 1, name_length == 4 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[0].name_length, loaded_schema.items[0].name_length);

	/* Attribute 1, name == "one" */
	PLANCK_UNIT_ASSERT_STR_ARE_EQUAL(tc, items[0].name, loaded_schema.items[0].name);

	/* Attribute 1, type == ION_TABLE_ATTRIBUTE_TYPE_INT */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[0].type, loaded_schema.items[0].type);

	/* Attribute 1, size == sizeof(int) */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[0].attribute_size, loaded_schema.items[0].attribute_size);

	/* Attribute 2, name_length == 4 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[1].name_length, loaded_schema.items[0].name_length);

	/* Attribute 2, name == "two" */
	PLANCK_UNIT_ASSERT_STR_ARE_EQUAL(tc, items[1].name, loaded_schema.items[1].name);

	/* Attribute 2, type == ION_TABLE_ATTRIBUTE_TYPE_INT */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[1].type, loaded_schema.items[1].type);

	/* Attribute 2, size == sizeof(int) */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[1].attribute_size, loaded_schema.items[1].attribute_size);
}

void
test_save_then_load_schema(
	planck_unit_test_t *tc
) {
	PLANCK_UNIT_ASSERT_TRUE(tc, run_test_setup() == ION_TABLE_TRUE);

	db_query_mm_t			mem_man;
	int						size			= 100;
	char					memory[size];
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 1, init_query_mm(&mem_man, memory, size));

	ion_table_schema_t loaded_schema;

	/* First save the schema then load it. */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_create_schema(tables[0], &schema));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_load_schema(tables[0], &loaded_schema, &mem_man));

	/* id == 1 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, schema.id, loaded_schema.id);

	/* num_attributes == 2 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, schema.num_attributes, loaded_schema.num_attributes);

	/* Attribute 1, name_length == 4 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[0].name_length, loaded_schema.items[0].name_length);

	/* Attribute 1, name == "one" */
	PLANCK_UNIT_ASSERT_STR_ARE_EQUAL(tc, items[0].name, loaded_schema.items[0].name);

	/* Attribute 1, type == ION_TABLE_ATTRIBUTE_TYPE_INT */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[0].type, loaded_schema.items[0].type);

	/* Attribute 1, size == sizeof(int) */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[0].attribute_size, loaded_schema.items[0].attribute_size);

	/* Attribute 2, name_length == 4 */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[1].name_length, loaded_schema.items[0].name_length);

	/* Attribute 2, name == "two" */
	PLANCK_UNIT_ASSERT_STR_ARE_EQUAL(tc, items[1].name, loaded_schema.items[1].name);

	/* Attribute 2, type == ION_TABLE_ATTRIBUTE_TYPE_INT */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[1].type, loaded_schema.items[1].type);

	/* Attribute 2, size == sizeof(int) */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, items[1].attribute_size, loaded_schema.items[1].attribute_size);
}

void
test_delete_schema(
	planck_unit_test_t *tc
) {
	PLANCK_UNIT_ASSERT_TRUE(tc, run_test_setup() == ION_TABLE_TRUE);
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_create_schema(tables[0], &schema));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_delete_schema(tables[0]));

	/* Check to make sure file is not there */
	FILE *fp = fopen(tables[0], "rb");
	PLANCK_UNIT_ASSERT_TRUE(tc, NULL == fp);
}

void
test_free_schema(
	planck_unit_test_t *tc
) {
	PLANCK_UNIT_ASSERT_TRUE(tc, run_test_setup() == ION_TABLE_TRUE);

	db_query_mm_t			mem_man;
	int						size			= 100;
	char					memory[size];
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, 1, init_query_mm(&mem_man, memory, size));

	ion_table_schema_t loaded_schema;

	/* First save the schema then load it. */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_create_schema(tables[0], &schema));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_load_schema(tables[0], &loaded_schema, &mem_man));

	/* Free the loaded schema from memory. */
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_free_schema_names(&loaded_schema, &mem_man));
	PLANCK_UNIT_ASSERT_INT_ARE_EQUAL(tc, ION_TABLE_ERROR_OK, ion_table_free_schema_items(&loaded_schema, &mem_man));

	PLANCK_UNIT_ASSERT_TRUE(tc, loaded_schema.items == NULL);
}

planck_unit_suite_t *
schema_getsuite(
) {
	planck_unit_suite_t *suite = planck_unit_new_suite();

	PLANCK_UNIT_ADD_TO_SUITE(suite, test_save_schema);
	PLANCK_UNIT_ADD_TO_SUITE(suite, test_load_schema);
	PLANCK_UNIT_ADD_TO_SUITE(suite, test_save_then_load_schema);
	PLANCK_UNIT_ADD_TO_SUITE(suite, test_delete_schema);
	PLANCK_UNIT_ADD_TO_SUITE(suite, test_free_schema);

	return suite;
}

void
run_all_tests_schema(
	void
) {
	planck_unit_suite_t *suite = schema_getsuite();
	planck_unit_run_suite(suite);
	planck_unit_destroy_suite(suite);
}
