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

#include "schema_manager.h"

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

	uint8_t name_length	= strlen(table_name);
	char filename[name_length + 5];
	sprintf(filename, "%s.sch", table_name);

	/* Make sure file does not exist. */
	file = fopen(filename, "r");
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
	/* First, write out size of each name, then the name itself. */
	for (i = 0; i < (int)(schema->num_attributes); i++) {
		name_length = strlen(schema->names[i]) + 1;
		if (1 != QUICKWRITE(name_length, file)) {
			goto BAD_WRITE;
		}
		if (1 != fwrite(schema->names[i], name_length, 1, file)) {
			goto BAD_WRITE;
		}
	}
	/* Next, write out each item. */
	for (i = 0; i < (int)(schema->num_attributes); i++) {
		if (1 != QUICKWRITE(schema->items[i].type, file)) {
			goto BAD_WRITE;
		}
		if (1 != QUICKWRITE(schema->items[i].size, file)) {
			goto BAD_WRITE;
		}
	}

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
	uint8_t name_length	= strlen(table_name);

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
	schema->items = db_qmm_falloc(mem_man, sizeof(ion_table_schema_item_t) * schema->num_attributes);

	/* First, read in size of each name and the name. */
	// TODO: Use fextend, do the arithmetic.
	schema->names = db_qmm_falloc(mem_man, sizeof(char *) * schema->num_attributes);
	for (i = 0; i < (int)(schema->num_attributes); i++) {
		if (1 != QUICKREAD(name_length, file)) {
			goto BAD_READ;
		}
		schema->names[i] = db_qmm_falloc(mem_man, name_length);
		if (1 != fread(schema->names[i], name_length, 1, file)) {
			goto BAD_READ;
		}
	}
	/* Next, read out each item. */
	for (i = 0; i < (int)(schema->num_attributes); i++) {
		if (1 != QUICKREAD(schema->items[i].type, file)) {
			goto BAD_READ;
		}
		if (1 != QUICKREAD(schema->items[i].size, file)) {
			goto BAD_READ;
		}
	}

	return ION_TABLE_ERROR_OK;

	BAD_READ:
		fclose(file);
		return ION_TABLE_ERROR_FILE_BAD_READ;
}

