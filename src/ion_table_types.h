#if !defined(ION_TABLE_TYPES_H_)
#define ION_TABLE_TYPES_H_

#if defined(__cplusplus__)
extern "C" {
#endif

#include <stdint.h>
#include "stack_memory_manager.h"

#if !defined(false)
#define false 0
#endif

#if !defined(true)
#define true 1
#endif

typedef enum {
	ION_TABLE_OPCODE_DEFINE_ATTRIBUTE,
	ION_TABLE_OPCODE_NONE
} ion_table_opcode_e;

typedef enum {
	ION_TABLE_ATTRIBUTE_TYPE_INT,
	ION_TABLE_ATTRIBUTE_TYPE_CHAR,
} ion_table_attribute_type_e;

typedef enum {
	ION_TABLE_ERROR_OK,
	ION_TABLE_ERROR_FILE_NOT_OPENED,
	ION_TABLE_ERROR_FILE_BAD_WRITE,
	ION_TABLE_ERROR_FILE_BAD_READ,
	ION_TABLE_ERROR_FILE_BAD_SEEK,
} ion_table_error_e;

typedef uint8_t ion_table_opcode_t;

typedef uint8_t ion_table_stop_flag_t;

typedef uint8_t ion_table_instruction_index_t;

typedef uint8_t ion_table_attribute_size_t;

typedef uint8_t ion_table_attribute_type_t;

typedef uint8_t ion_table_schema_count_t;

typedef uint8_t ion_table_error_t;

typedef union {
	void			*pointer;
	int 			integer;
	unsigned int	u_integer;
} ion_table_parameter_t;

typedef struct {
	ion_table_opcode_t		opcode;
	ion_table_parameter_t	p1;
	ion_table_parameter_t	p2;
	ion_table_parameter_t	p3;
} ion_table_instruction_t;

typedef struct {
	ion_table_instruction_t			*instructions;
	db_query_mm_t					*mem_man;
	ion_table_stop_flag_t			*stop;
	ion_table_instruction_index_t	current_instr_idx;
	ion_table_opcode_t				last_opcode;
} ion_table_executable_t;

typedef struct {
	int a;
} ion_table_tuple_t;

typedef struct {
	ion_table_attribute_size_t		size;
	ion_table_attribute_type_t		type;
} ion_table_schema_item_t;

typedef struct {
	int 							id;	// TODO: Make this the actual master table id type.
	ion_table_schema_count_t		num_attributes;
	ion_table_schema_item_t			*items;
	char 							**names;
} ion_table_schema_t;

#if defined(__cplusplus__)
}
#endif

#endif /* ION_TABLE_TYPES_H_ */
