/******************************************************************************/
/**
@file
@author		Graeme Douglas, Wade Penson
@brief		Manages table schema data.
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

#include "schema.h"
#include "types.h"

/**
@brief	Write a single primitive type to a file.
*/
#define QUICKWRITE(thing, file) fwrite(&(thing), sizeof(thing), 1, file)

/**
@brief	Reads a single primitive type to a file.
*/
#define QUICKREAD(thing, file) fread(&(thing), sizeof(thing), 1, file)

ion_table_error_t
ion_table_create_schema(
	char *table_name,
	ion_table_schema_t *schema
) {
	int i;
	FILE *file;

	uint8_t name_length	= (uint8_t) (strlen(table_name));
	char filename[name_length + 5];
	sprintf(filename, "%s.sch", table_name);

	/* Make sure file does not exist. */
	file = fopen(filename, "rb");
	if (NULL != file) {
		fclose(file);
		return ION_TABLE_ERROR_TABLE_EXISTS;
	}

	/* Create a new schema file. */
	file = fopen(filename, "wb+");
	if (NULL == file) {
		return ION_TABLE_ERROR_FILE_NOT_CREATED;
	}

	/* Write the table id and the number of attributes that the table has. */
	if (1 != QUICKWRITE(schema->id, file)) {
		goto BAD_WRITE;
	}
	if (1 != QUICKWRITE(schema->num_attributes, file)) {
		goto BAD_WRITE;
	}
	/* Write out each item. */
	for (i = 0; i < (int)(schema->num_attributes); i++) {
		name_length = (uint8_t) (strlen(schema->items[i].name) + 1);
		if (1 != QUICKWRITE(name_length, file)) {
			goto BAD_WRITE;
		}
		if (1 != fwrite(schema->items[i].name, name_length, 1, file)) {
			goto BAD_WRITE;
		}

		if (1 != QUICKWRITE(schema->items[i].type, file)) {
			goto BAD_WRITE;
		}

		if (1 != QUICKWRITE(schema->items[i].attribute_size, file)) {
			goto BAD_WRITE;
		}
	}

	fclose(file);
	return ION_TABLE_ERROR_OK;

	BAD_WRITE:
		fclose(file);
		return ION_TABLE_ERROR_FILE_BAD_WRITE;
}

ion_table_error_t
ion_table_load_schema(
	char *table_name,
	ion_table_schema_t *schema,
	db_query_mm_t *mem_man
) {
	int i;
	FILE *file;
	uint8_t name_length	= (uint8_t) strlen(table_name);

	char filename[name_length + 5];
	sprintf(filename, "%s.sch", table_name);

	/* Open the schema file and seek to the beginning. */
	file = fopen(filename, "rb");
	if (NULL == file) {
		return ION_TABLE_ERROR_FILE_NOT_OPENED;
	}
	if (0 != fseek(file, 0, SEEK_SET)) {
		return ION_TABLE_ERROR_FILE_BAD_SEEK;
	}

	/* Read the table id and the number of attributes that the table has. */
	if (1 != QUICKREAD(schema->id, file)) {
		goto BAD_READ;
	}
	if (1 != QUICKREAD(schema->num_attributes, file)) {
		goto BAD_READ;
	}

	/* Allocate enough space in memory for the schema. */
	schema->items = db_qmm_balloc(mem_man, sizeof(ion_table_schema_item_t) * schema->num_attributes);
	if (NULL == schema->items) {
		return ION_TABLE_ERROR_OUT_OF_MEMORY;
	}

	/* Read in each item. */
	// TODO: Use fextend, do the arithmetic.
	for (i = 0; i < (int)(schema->num_attributes); i++) {
		if (1 != QUICKREAD(schema->items[i].name_length, file)) {
			goto BAD_READ;
		}

		schema->items[i].name = db_qmm_balloc(mem_man, name_length);
		if (NULL == schema->items[i].name) {
			return ION_TABLE_ERROR_OUT_OF_MEMORY;
		}

		if (1 != fread(schema->items[i].name, schema->items[i].name_length, 1, file)) {
			goto BAD_READ;
		}

		if (1 != QUICKREAD(schema->items[i].type, file)) {
			goto BAD_READ;
		}

		if (1 != QUICKREAD(schema->items[i].attribute_size, file)) {
			goto BAD_READ;
		}
	}

	fclose(file);
	return ION_TABLE_ERROR_OK;

	BAD_READ:
		fclose(file);
		return ION_TABLE_ERROR_FILE_BAD_READ;
}

ion_table_error_t
ion_table_delete_schema(
	char *table_name
) {
	uint8_t name_length	= (uint8_t) strlen(table_name);
	char filename[name_length + 5];
	sprintf(filename, "%s.sch", table_name);

	if (remove(filename) != 0) {
		return ION_TABLE_ERROR_FILE_NOT_REMOVED;
	}

	return ION_TABLE_ERROR_OK;
}

ion_table_error_t
ion_table_free_schema_from_memory(
	ion_table_schema_t *schema,
	db_query_mm_t *mem_man
) {
	int i;
	for (i = (int)(schema->num_attributes) -1; i >= 0 ; i--) {
		if (db_qmm_bfree(mem_man, schema->items[i].name) != 1) {
			return ION_TABLE_ERROR_FAILED_TO_FREE_MEMORY;
		}
	}

	if (db_qmm_bfree(mem_man, schema->items) != 1) {
		return ION_TABLE_ERROR_FAILED_TO_FREE_MEMORY;
	}

	schema->items = NULL;

	return ION_TABLE_ERROR_OK;
}
