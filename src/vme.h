/******************************************************************************/
/**
@file
@author		Graeme Douglas, Wade Penson
@brief		Interface for Virtual Machine Executor (VME) used to execute query
 			plans.
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

#if !defined(ION_TABLE_VME_H_)
#define ION_TABLE_VME_H_

#include <stdint.h>

#include "types.h"
#include "macros.h"
#include "stack_memory_manager.h"

/**
@brief		Initializes a query plan.
@param[out]	plan
 				Virtual Machine query plan, which can be executed.
@param[in]	instructions
 				Array of instructions used by query plan
@param[in]	mem_man
 				Memory manager to be used by query plan.
*/
void
ion_table_init_executable(
	ion_table_executable_t	*plan,
	ion_table_instruction_t	*instructions,
	db_query_mm_t			*mem_man
);

/**
@brief		Executes a query plan.
@details	This will run the query plan until the end or until a result row is available. The user must continue
 			calling this for each result row to get the complete result set.
@param[in]	plan
 				Virtual Machine query plan to be executed.
@param[out]	tuple
 				Result row or tuple that is returned.
@return		Return status of the query. It contains an @ref ion_table_error_e code and a count of the number of items
 			affected by the query.
*/
ion_table_status_t
ion_table_execute(
	ion_table_executable_t *plan,
	ion_table_tuple_t *tuple
);

#endif /* ION_TABLE_VME_H_ */
