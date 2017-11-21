/*
	a custom randombytes must implement:

	void ED25519_FN(ed25519_randombytes_unsafe) (void *p, size_t len);

	ed25519_randombytes_unsafe is used by the batch verification function
	to create random scalars
*/

#include "rand.h"

#define ed25519_randombytes_unsafe(p, len) random_buffer((uint8_t *)(p), (len))
