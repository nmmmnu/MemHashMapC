#ifndef _HM_BUCKET_H
#define _HM_BUCKET_H

#include "hm_pair.h"

#include <stdint.h>

inline int hm_list_exists(HMPair * const *bucket, const char *key);

const HMPair *hm_list_get(HMPair * const *bucket, const char *key);

uint64_t hm_list_count(HMPair * const *bucket);

int hm_list_put(HMPair **bucket, HMPair *newpair);

int hm_list_remove(HMPair **bucket, const char *key);

void hm_list_print(HMPair * const *bucket);

int hm_list_free(HMPair **bucket);

#endif

