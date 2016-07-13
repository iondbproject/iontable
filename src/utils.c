#include "utils.h"

uint32_t
hash_string(
	char *str
)
{
	uint32_t hash = 5381;
	int8_t c;

	while ((c = *str++))
	{
		hash = (uint32_t)((hash << 5) + hash) ^ c; /* hash * 33 ^ c */
	}

	return hash;
}
