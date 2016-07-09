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

#if !defined(ION_TABLE_TYPES_H_)
#define ION_TABLE_TYPES_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "stack_memory_manager.h"

/**
@brief	Error codes for ion table
*/
typedef enum {
	/**> An error code that indicates that everything is OK. */
	ION_TABLE_ERROR_OK,
	/**> An error code that indicates that a file could not be created. */
	ION_TABLE_ERROR_FILE_NOT_CREATED,
	/**> An error code that indicates that a file could not be opened. */
	ION_TABLE_ERROR_FILE_NOT_OPENED,
	/**> An error code that indicates that a write in a file failed. */
	ION_TABLE_ERROR_FILE_BAD_WRITE,
	/**> An error code that indicates that a read in a file failed. */
	ION_TABLE_ERROR_FILE_BAD_READ,
	/**> An error code that indicates that a seek in a file failed. */
	ION_TABLE_ERROR_FILE_BAD_SEEK,
	/**> An error code used for ion_table_status_t to indicate that a status has not been initialized yet. */
	ION_TABLE_ERROR_STATUS_UNINITIALIZED,
	/**> An error code used to indicate that a table (or schema) already exists. */
	ION_TABLE_ERROR_TABLE_EXISTS,
	/**> An error code that indicates that an opcode in an execution plan is invalid. */
	ION_TABLE_ERROR_UNDEFINED_OPCODE
} ion_table_error_e;

/**
@brief	Enum to be used as a replacement for boolean values.
*/
typedef enum {
	/**> False boolean value. */
	ION_TABLE_FALSE = 0,
	/**> Used as a replacement for true boolean value. */
	ION_TABLE_TRUE
} ion_table_bool_e;

/**
@brief	A status that indicates the result of an execution step in the virtual machine.
*/
typedef struct {
	/**> The type of error. */
	ion_table_error_e error;
	/**> The number of items (rows, etc.) affected. */
	uint32_t count;
} ion_table_status_t;

/**
@brief	Set of opcodes used by the virtual machine and in execution plans.
*/
typedef enum {
	/**> Sets up an attribute when creating the schema. */
	ION_TABLE_OPCODE_DEFINE_ATTRIBUTE,
	/**> Creates a table given that the schema is defined and table name is given. */
	ION_TABLE_CREATE_TABLE,
	/**> No operation. */
	ION_TABLE_OPCODE_NOP
} ion_table_opcode_e;

/**
@brief	Attribute types
*/
typedef enum {
	/**> Integer type */
	ION_TABLE_ATTRIBUTE_TYPE_INT,
	/**> Character type */
	ION_TABLE_ATTRIBUTE_TYPE_CHAR,
} ion_table_attribute_type_e;

/**
@brief	Opcode ID
*/
typedef uint8_t ion_table_opcode_t;

/**
@brief	Flag used to indicate to the query executor and user that the query has stopped and required intervention.
*/
typedef uint8_t ion_table_stop_flag_t;

/**
@brief	The index for the instruction in the instruction array (or instruction counter).
*/
typedef uint8_t ion_table_instruction_index_t;

/**
@brief	The size of an attribute in a table or index.
*/
typedef uint8_t ion_table_attribute_size_t;

/**
@brief	The type of an attribute in a table or index.
*/
typedef uint8_t ion_table_attribute_type_t;

// TODO: Ask Graeme what this is for
/**
@brief
*/
typedef uint8_t ion_table_schema_count_t;

/**
@brief	The ID of an error code.
*/
typedef uint8_t ion_table_error_t;

/**
@brief	Parameter in an instruction for a query plan. Can be an integer, unsigned integer, or pointer.
*/
typedef union {
	void			*pointer;
	int 			integer;
	unsigned int	u_integer;
} ion_table_parameter_t;

/**
@brief
*/
typedef struct {
	ion_table_opcode_t		opcode;
	ion_table_parameter_t	p1;
	ion_table_parameter_t	p2;
	ion_table_parameter_t	p3;
} ion_table_instruction_t;

/**
@brief
*/
typedef struct {
	ion_table_instruction_t			*instructions;
	db_query_mm_t					*mem_man;
	ion_table_stop_flag_t			stop;
	ion_table_instruction_index_t	current_instr_idx;
	ion_table_opcode_t				prev_opcode;
} ion_table_executable_t;

/**
@brief
*/
typedef struct {
	int a;
} ion_table_tuple_t;

/**
@brief
*/
typedef struct {
	ion_table_attribute_size_t		size;
	ion_table_attribute_type_t		type;
} ion_table_schema_item_t;

/**
@brief
*/
typedef struct {
	int 							id;	// TODO: Make this the actual master table id type.
	ion_table_schema_count_t		num_attributes;
	ion_table_schema_item_t			*items;
	char 							**names;
} ion_table_schema_t;

#if defined(__cplusplus)
}
#endif

#endif /* ION_TABLE_TYPES_H_ */