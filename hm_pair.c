#include "hm_pair.h"

#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#define TIMESTAMP_MICROTIME_MULTIPLE 1 * 1000 * 1000

#define MAX_KEYSIZE	0xffff
#define MAX_VALSIZE	0xffffffff


inline static timestamp_t _hm_pair_now();

hm_pair_t *hm_pair_create(const char*key, const char*val){
	/*
	In first version, we kept the character data inside the buffer,
	and skipped the last \0 terminating character.

	By this way we "save" 2 bytes.

	However examinig memory allocation shows, that memory used is
	much more than memory allocated, no matter what allocator is used.

	Tests were made using standard malloc(), jemalloc and tcmalloc.

	By this reason we will put the terminating character,
	so we can pass key and value back to the client using refference
	to stored value. Comparrisson will work easyer and better too.
	*/

	if (key == NULL || val == NULL)
		return NULL;

	uint64_t keylen = strlen(key) + 1;
	uint64_t vallen = strlen(val) + 1;

	if (keylen >= MAX_KEYSIZE || vallen >= MAX_VALSIZE)
		return NULL;

	hm_pair_t *pair = malloc( sizeof(hm_pair_t) + keylen + vallen);

	if (pair == NULL)
		return NULL;

	pair->next	= NULL;
	pair->created	= _hm_pair_now();
	pair->keylen	= keylen;
	pair->vallen	= vallen;

	// memcpy so we can switch to blobs later...
	memcpy(& pair->buffer[0     ], key, keylen);
	memcpy(& pair->buffer[keylen], val, vallen);

	return pair;
};

hm_pair_t *hm_pair_createx(const char*key, const char*val, expires_t expires){
	hm_pair_t *pair = hm_pair_create(key, val);

	if (pair == NULL)
		return NULL;

	pair->expires = expires;

	return pair;
}

const char *hm_pair_getkey(const hm_pair_t *pair){
	return & pair->buffer[0];
}

const char *hm_pair_getval(const hm_pair_t *pair){
	return & pair->buffer[ pair->keylen ];
}

int hm_pair_equalkey(const hm_pair_t *pair, const char *key){
	if (key == NULL)
		return 0;

	if (pair->keylen != strlen(key) + 1)
		return 0;

	return ! memcmp(& pair->buffer[0], key, pair->keylen);
}

int hm_pair_equalpair(const hm_pair_t *pair1, const hm_pair_t *pair2){
	if (pair1 == NULL || pair2 == NULL)
		return 0;

	if (pair1->keylen != pair2->keylen)
		return 0;

	return ! memcmp(& pair1->buffer[0], & pair2->buffer[0], pair1->keylen);
}

int hm_pair_valid(const hm_pair_t *pair){
	if (! pair->expires)
		return 1;

	return pair->created + pair->expires * TIMESTAMP_MICROTIME_MULTIPLE > _hm_pair_now();
}

// ===============================================================

inline static timestamp_t _hm_pair_now(){
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return tv.tv_sec * TIMESTAMP_MICROTIME_MULTIPLE + tv.tv_usec;
}
