#include "mhpair.h"

#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#define MIN(a, b) a < b ? a : b

#define MICRO_TIME_MULTIPLE 1000 * 1000

unsigned long int _mhpair_now(){
	struct timeval tv;

	gettimeofday(&tv,NULL);

	return tv.tv_sec * MICRO_TIME_MULTIPLE + tv.tv_usec;
}

int mhpair_valid(const struct mhpair *pair){
//	if (pair->key == NULL || pair->val == NULL || pair->created == 0 || ( ! mhpair_expired(pair)) )
//		return 0;

	return 1;
}

int mhpair_expired(const struct mhpair *pair){
	if (pair->expires)
		return pair->created + pair->expires * MICRO_TIME_MULTIPLE < _mhpair_now();

	return 0;
}

struct mhpair *mhpair_create(const char*key, const char*val, unsigned long int expires){
	unsigned int keylen = strlen(key);
	unsigned int vallen = strlen(val);

	struct mhpair *pair = malloc( sizeof(struct mhpair) + keylen + vallen); // + 1

	if (pair == NULL)
		return NULL;

	pair->created	= _mhpair_now();
	pair->expires	= expires;
	pair->keylen	= keylen;
	pair->vallen	= vallen;

	strncpy(& pair->buffer[0     ], key, keylen);
	strncpy(& pair->buffer[keylen], val, vallen);

	//pair->buffer[keylen + vallen] = '\0';

	return pair;
};

const char *_mhpair_sub(const struct mhpair *pair, char *buffer, unsigned int len, unsigned int start, unsigned int len2){
//	if (pair == NULL)
//		return NULL;

	len2 = MIN(len2, len - 1);

	strncpy(buffer, & pair->buffer[ start ], len2);
	buffer[len2] = '\0';

	return buffer;
}


inline const char *mhpair_getkey(const struct mhpair *pair, char *buffer, unsigned int len){
	if (pair == NULL)
		return NULL;

	return _mhpair_sub(pair, buffer, len, 0, pair->keylen);
}

inline const char *mhpair_getval(const struct mhpair *pair, char *buffer, unsigned int len){
	if (pair == NULL)
		return NULL;

	return _mhpair_sub(pair, buffer, len, pair->keylen, pair->vallen);
}

inline int mhpair_equals(const struct mhpair *pair, const char *key){
	if (pair == NULL || key == NULL)
		return 0;

	return strncmp(& pair->buffer[0], key, pair->keylen) == 0;
}

