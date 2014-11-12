#include "hm_pair.h"

#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#define MIN(a, b) a < b ? a : b

#define MICRO_TIME_MULTIPLE 1000 * 1000

unsigned long int _hm_pair_now(){
	struct timeval tv;

	gettimeofday(&tv,NULL);

	return tv.tv_sec * MICRO_TIME_MULTIPLE + tv.tv_usec;
}

int hm_pair_valid(const HMPair *pair){
//	if (pair->key == NULL || pair->val == NULL || pair->created == 0 || ( ! hm_pair_expired(pair)) )
//		return 0;

	return 1;
}

int hm_pair_expired(const HMPair *pair){
	if (pair->expires)
		return pair->created + pair->expires * MICRO_TIME_MULTIPLE < _hm_pair_now();

	return 0;
}

HMPair *hm_pair_create(const char*key, const char*val, unsigned long int expires){
	unsigned int keylen = strlen(key);
	unsigned int vallen = strlen(val);

	HMPair *pair = malloc( sizeof(HMPair) + keylen + vallen); // + 1

	if (pair == NULL)
		return NULL;

	pair->created	= _hm_pair_now();
	pair->expires	= expires;
	pair->keylen	= keylen;
	pair->vallen	= vallen;

	strncpy(& pair->buffer[0     ], key, keylen);
	strncpy(& pair->buffer[keylen], val, vallen);

	//pair->buffer[keylen + vallen] = '\0';

	return pair;
};

static const char *_hm_pair_sub(const HMPair *pair, char *buffer, unsigned int len, unsigned int start, unsigned int len2){
	len2 = MIN(len2, len - 1);

	strncpy(buffer, & pair->buffer[ start ], len2);
	buffer[len2] = '\0';

	return buffer;
}


inline const char *hm_pair_getkey(const HMPair *pair, char *buffer, unsigned int len){
	if (pair == NULL)
		return NULL;

	return _hm_pair_sub(pair, buffer, len, 0, pair->keylen);
}

inline const char *hm_pair_getval(const HMPair *pair, char *buffer, unsigned int len){
	if (pair == NULL)
		return NULL;

	return _hm_pair_sub(pair, buffer, len, pair->keylen, pair->vallen);
}

inline int hm_pair_equals(const HMPair *pair, const char *key){
	if (pair == NULL || key == NULL)
		return 0;

	return strncmp(& pair->buffer[0], key, pair->keylen) == 0;
}

