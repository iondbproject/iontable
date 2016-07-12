/******************************************************************************/
/**
@file
@author		Graeme Douglas, Wade Penson
@brief		A place for various macros to reside.
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

#if !defined(MACROS_H_)
#define MACROS_H_

/**
@brief		Macro for moving any pointer some number of bytes cleanly.
@param		d
 				The destination pointer. This is the pointer that is to be set.
@param		s
 				The source pointer. This is the pointer that is to be moved. IT WILL **NOT** BE SET!
@param		n
 				The integral number of bytes to move. May be negative.
@param		t
 				The pointer type to cast back to. **Do not include include brackets for the type.**
@todo		Make it so all calls to this macro actually use @ref POINTERATNBYTES.
*/
#ifndef MOVEPOINTERNBYTES
#define MOVEPOINTERNBYTES(d, s, n, t) (d) = (t)(((unsigned char*)((s)))+(n))
#else
#error "MACRO NAME CLASH ON MOVEPOINTERNBYTES!"
#endif

/**
@brief		Macro for accessing any pointer some number of bytes away cleanly.
@param		s
 				The source pointer. This is the pointer that is to be moved. IT WILL **NOT** BE SET!
@param		n
 				The integral number of bytes to move. May be negative.
@param		t
 				The pointer type to cast back to. **Do not include brackets for the type.**
*/
#ifndef POINTERATNBYTES
#define POINTERATNBYTES(s, n, t) ((t)(((unsigned char*)((s)))+(n)))
#else
#error "MACRO NAME CLASH ON POINTERATNBYTES!"
#endif

/**
@brief		Macro for measuring the distance between the two pointers.
@details	Calculates the distance in bytes between p1 and p2. If p1 comes after p2, the result shall be positive.
			Keep in mind, this only works within the same array (same rules as normal in C).
@param		p1
 				The pointer to be subtracted from.
@param		p2
 				The pointer to be subtracted.
*/
#ifndef POINTERBYTEDIST
#define POINTERBYTEDIST(p1, p2) (((unsigned char*)(p1)) - ((unsigned char*)(p2)))
#else
#error "MACRO NAME CLASH ON POINTERBYTEDIST!"
#endif

/**
@brief		Move a pointer exactly a number of units cleanly.
@details	Macro for moving any pointer @p s by @p n units cleanly by casting it to type source pointer type @p ts
 			then casting it back to destination pointer type @p td.
@param		d
 				The destination pointer. This is the pointer that is to be set.
@param		s
 				The source pointer. This is the pointer that is to be moved. IT WILL **NOT** BE SET!
@param		n
 				The integral number of units (**not** bytes) to move. May be negative.
@param		ts
 				The pointer type to cast the source to before moving. **Do not include include brackets for the type.**
@param		td
 				The pointer type to cast the result to before assigning. **Do not include include brackets for
 				the type.**
*/
#ifndef MOVEPOINTERNUNITS
#define MOVEPOINTERNUNITS(d, s, n, td, ts) (d) = (td)(((ts)((s)))+(n))
#else
#error "MACRO NAME CLASH ON MOVEPOINTERUNITS!"
#endif

/**
@brief		Creates a status struct with a given error and count.
@param		error
 				Error of type @ref ion_table_error_e.
@param		count
 				Number of elements affected before creation of error.
@return		A compound literal that is an @ref ion_table_status_t.
*/
#define ION_TABLE_STATUS_CREATE(error, count) ((ion_table_status_t) { (error), (count) })

/**
@brief		Creates a status struct with an uninitialized error and count of 0.
@return		A compound literal that is an @ref ion_table_status_t.
*/
#define ION_TABLE_STATUS_INITIALIZE ((ion_table_status_t) { ION_TABLE_ERROR_STATUS_UNINITIALIZED, 0 })

/**
@brief		Creates a status struct with a given error and count of 0.
@param		error
 				Error of type @ref ion_table_error_e.
@return		A compound literal that is an @ref ion_table_status_t.
*/
#define ION_TABLE_STATUS_ERROR(error) ((ion_table_status_t) { (error), 0 })

/**
@brief		Creates a status struct that indicates a success and that a specified number of elements were affected.
@param		count
 				Number of elements affected.
@return		A compound literal that is an @ref ion_table_status_t.
*/
#define ION_TABLE_STATUS_OK(count) ((ion_table_status_t) { ION_TABLE_ERROR_OK, (count) })

#endif /* MACROS_H_ */
