#ifndef _HM_BUCKET_H
#define _HM_BUCKET_H

#include "hm_pair.h"

#include <stdint.h>


typedef struct HMBucket{
	uint64_t	count;	// 8
	HMPair **	pairs;	// pointer to array of pointers
} HMBucket;

int hm_bucket_free(HMBucket *bucket);

inline int hm_bucket_exists(const HMBucket *bucket, const char *key);

int hm_bucket_put(HMBucket *bucket, HMPair *newpair);

int hm_bucket_remove(HMBucket *bucket, const char *key);

uint64_t hm_bucket_count(const HMBucket *bucket);

void hm_bucket_print(const HMBucket *bucket);

#endif

