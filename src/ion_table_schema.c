#include "ion_table_schema.h"
#include <string.h>

#define QUICKWRITE(thing, file) fwrite(&(thing), sizeof(thing), 1, file)
#define QUICKREAD(thing, file) fread(&(thing), sizeof(thing), 1, file)

ion_table_error_t
ion_table_create_schema_file(
	char				*schema_name,
	ion_table_schema_t	*schema
) {
	int i;
	FILE *file;
	// TODO: Check if the schema file already exists.
	uint8_t length	= strlen(schema_name);
	char filename[length+5];
	/*
	memcpy(filename, schema_name, length);
	filename[length+1]	= '.';
	filename[length+2]	= 's';
	filename[length+3]	= 'c';
	filename[length+4]	= 'h';
	filename[length+5]	= '\0';
	 */
	sprintf(filename, "%s.sch", schema_name);

	file = fopen(filename, "wb+");
	if (NULL == file) {
		return ION_TABLE_ERROR_FILE_NOT_OPENED;
	}

	if (1 != QUICKWRITE(schema->id, file)) {
		goto BAD_WRITE;
	}
	if (1 != QUICKWRITE(schema->num_attributes, file)) {
		goto BAD_WRITE;
	}
	/* First, write out size of each name, then the name itself. */
	for (i = 0; i < (int)(schema->num_attributes); i++) {
		length = strlen(schema->names[i]) + 1;
		if (1 != QUICKWRITE(length, file)) {
			goto BAD_WRITE;
		}
		if (1 != fwrite(schema->names[i], length, 1, file)) {
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
	char *schema_name,
	ion_table_schema_t *schema,
	db_query_mm_t *mem_man
)
{
	int i;
	FILE *file;
	// TODO: Check if the schema file already exists.
	uint8_t length	= strlen(schema_name);
	char filename[length+5];
	/*
	memcpy(filename, schema_name, length);
	filename[length+1]	= '.';
	filename[length+2]	= 's';
	filename[length+3]	= 'c';
	filename[length+4]	= 'h';
	filename[length+5]	= '\0';
	 */
	sprintf(filename, "%s.sch", schema_name);

	file = fopen(filename, "rb");
	if (NULL == file) {
		return ION_TABLE_ERROR_FILE_NOT_OPENED;
	}
	if (0 != fseek(file, 0, SEEK_SET)) {
		return ION_TABLE_ERROR_FILE_BAD_SEEK;
	}

	if (1 != QUICKREAD(schema->id, file)) {
		goto BAD_READ;
	}
	if (1 != QUICKREAD(schema->num_attributes, file)) {
		goto BAD_READ;
	}

	schema->items = db_qmm_falloc(mem_man, sizeof(ion_table_schema_item_t)*schema->num_attributes);

	/* First, read in size of each name and the name. */
	// TODO: Use fextend, do the arithmetic.
	schema->names = db_qmm_falloc(mem_man, sizeof(char *) * schema->num_attributes);
	for (i = 0; i < (int)(schema->num_attributes); i++) {
		if (1 != QUICKREAD(length, file)) {
			goto BAD_READ;
		}
		schema->names[i] = db_qmm_falloc(mem_man, length);
		if (1 != fread(schema->names[i], length, 1, file)) {
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

