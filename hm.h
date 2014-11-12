#ifndef _HM_H
#define _HM_H

#include "hm_pair.h"

#include <stdint.h>


typedef struct HM{
	uint64_t	capacity;	// 8
	HMBucket 	buckets[];
} HM;


HM *hm_create(uint64_t capacity);
int hm_free(HM *table);

int hm_put(HM *table, HMPair *pair);


unsigned long mh_hash(const char *str);

#endif

