#if !defined(ION_TABLE_UTILS_H_)
#define ION_TABLE_UTILS_H_

#include <stdint.h>

/**
@brief		Hashes a string.
@details	The type of hash function is dbj2a. It has a very poor random distribution. However, it has very few
 			collisions so it is a fitting hash function since it is quite small and fairly fast.
@param[in]	str A string with a null terminating character (\0).
@return		A 4 byte integer (aka. your hash).
*/
uint32_t
hash_string(
	char *str
);

#endif // ION_TABLE_UTILS_H_
