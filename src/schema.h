/******************************************************************************/
/**
@file
@author		Graeme Douglas, Wade Penson
@brief		Interface for managing table schema data.
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

#if !defined(ION_TABLE_SCHEMA_H_)
#define ION_TABLE_SCHEMA_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#include "types.h"
#include "stack_memory_manager.h"

/**
@brief		Creates a new schema file for a table. It also keeps track of the table ID and any other meta data
 			associated with a table.
@param[in]	table_name
 				Name of a table.
@param[in]	schema
 				Defined schema of the table.
@return		An error defined in @ref ion_table_error_e.
*/
ion_table_error_t
ion_table_create_schema(
	char *table_name,
	ion_table_schema_t *schema
);

/**
@brief		Creates a new schema file for a table. It also keeps track of the table ID and any other meta data
 			associated with a table.
@param[in]	table_name
 				Name of a table.
@param[out]	schema
 				Empty schema that will be populated.
@param[in]	mem_man
 				Memory manager used for storing schema.
@return		An error defined in @ref ion_table_error_e.
*/
ion_table_error_t
ion_table_load_schema(
	char *table_name,
	ion_table_schema_t *schema,
	db_query_mm_t *mem_man
);

/**
@brief		Deletes the schema from disk.
@param[in]	table_name
 				Name of a table.
@return		An error defined in @ref ion_table_error_e.
*/
ion_table_error_t
ion_table_delete_schema(
	char *table_name
);

/**
@brief		Frees the names in the schema from memory if they were allocated through the db_query_mm_t.
@param[in]	schema
 				Defined schema of the table.
@param[in]	mem_man
 				Memory manager used to store schema.
@return		An error defined in @ref ion_table_error_e.
*/
ion_table_error_t
ion_table_free_schema_names(
	ion_table_schema_t *schema,
	db_query_mm_t *mem_man
);

/**
@brief		Frees the items in the scheme from memory if they were allocated through the db_query_mm_t
@param[in]	schema
 				Defined schema of the table.
@param[in]	mem_man
 				Memory manager used to store schema.
@return		An error defined in @ref ion_table_error_e.
*/
ion_table_error_t
ion_table_free_schema_items(
	ion_table_schema_t *schema,
	db_query_mm_t *mem_man
);

#if defined(__cplusplus)
}
#endif

#endif /* ION_TABLE_SCHEMA_H_ */
