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
	size_t keylen = strlen(key);
	size_t vallen = strlen(val);

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

static const char *_hm_pair_sub(const HMPair *pair, char *buffer, size_t len, size_t data_start, uint32_t data_len){
	data_len = MIN(data_len, len - 1);

	memcpy(buffer, & pair->buffer[ data_start ], data_len);
	buffer[data_len] = '\0';

	return buffer;
}


inline const char *hm_pair_getkey(const HMPair *pair, char *buffer, uint32_t len){
	if (pair == NULL)
		return NULL;

	return _hm_pair_sub(pair, buffer, len, 0, pair->keylen);
}

inline const char *hm_pair_getval(const HMPair *pair, char *buffer, uint32_t len){
	if (pair == NULL)
		return NULL;

	return _hm_pair_sub(pair, buffer, len, pair->keylen, pair->vallen);
}

inline int hm_pair_equals(const HMPair *pair, const char *key){
	if (pair == NULL || key == NULL)
		return 0;

	if (pair->keylen != strlen(key))
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
