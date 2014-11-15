#include "hm_pair.h"

#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#define MIN(a, b) a < b ? a : b

#define MICRO_TIME_MULTIPLE 1000 * 1000

inline unsigned long int _hm_pair_now(){
	struct timeval tv;

	gettimeofday(&tv,NULL);

	return tv.tv_sec * MICRO_TIME_MULTIPLE + tv.tv_usec;
}

HMPair *hm_pair_create(const char*key, const char*val, unsigned long int expires){
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
	size_t keylen = strlen(key) + 1;
	size_t vallen = strlen(val) + 1;

	HMPair *pair = malloc( sizeof(HMPair) + keylen + vallen); // + 1

	if (pair == NULL)
		return NULL;

	pair->next	= NULL;
	pair->created	= _hm_pair_now();
	pair->expires	= expires;
	pair->keylen	= keylen;
	pair->vallen	= vallen;

	// memcpy so we can switch to blobs later...
	memcpy(& pair->buffer[0     ], key, keylen);
	memcpy(& pair->buffer[keylen], val, vallen);

	//pair->buffer[keylen + vallen] = '\0';

	return pair;
};

inline int hm_pair_free(HMPair *pair){
	free(pair);

	return 1;
}

inline const char *hm_pair_getkey(const HMPair *pair){
	if (pair == NULL)
		return NULL;

	return & pair->buffer[0];
}

inline const char *hm_pair_getval(const HMPair *pair){
	if (pair == NULL)
		return NULL;

	return & pair->buffer[ pair->keylen ];
}

inline int hm_pair_equals(const HMPair *pair, const char *key){
	if (pair == NULL || key == NULL)
		return 0;

	if (pair->keylen != strlen(key) + 1)
		return 0;

	return ! memcmp(& pair->buffer[0], key, pair->keylen);
}

inline int hm_pair_equalp(const HMPair *pair1, const HMPair *pair2){
	if (pair1 == NULL || pair2 == NULL)
		return 0;

	if (pair1->keylen != pair2->keylen)
		return 0;

	return ! memcmp(& pair1->buffer[0], & pair2->buffer[0], pair1->keylen);
}

inline int hm_pair_valid(const HMPair *pair){
//	if (pair->key == NULL || pair->val == NULL || pair->created == 0 || ( ! hm_pair_expired(pair)) )
//		return 0;

	return 1;
}

inline int hm_pair_expired(const HMPair *pair){
	if (pair->expires)
		return pair->created + pair->expires * MICRO_TIME_MULTIPLE < _hm_pair_now();

	return 0;
}
