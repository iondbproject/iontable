#ifndef ION_TABLE_H_
#define ION_TABLE_H_

#include <stdint.h>
#include "ion_table_types.h"
#include "stack_memory_manager.h"

#if !defined(false)
#define false 0
#endif

#if !defined(true)
#define true 1
#endif


void
ion_table_init_executable(
	ion_table_executable_t	*plan,
	ion_table_instruction_t	*instructions,
	db_query_mm_t			*mem_man
);

int
ion_table_execute(
	ion_table_executable_t *plan,
	ion_table_tuple_t *tuple
);

#endif /* ION_TABLE_H_ */
