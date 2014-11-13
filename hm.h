#ifndef _HM_H
#define _HM_H

#include "hm_pair.h"
#include "hm_bucket.h"

#include <stdint.h>


typedef struct HM{
	uint64_t	capacity;	// 8
	HMBucket 	buckets[];
} HM;


HM *hm_create(uint64_t capacity);
int hm_free(HM *table);

int hm_exists(HM *table, const char *key);

HMPair *hm_get(HM *table, const char *key);
int hm_put(HM *table, HMPair *pair);
int hm_remove(HM *table, const char *key);

void hm_print(const HM *table, int all);

unsigned long int mh_hash(const char *str);

#endif

