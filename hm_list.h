#ifndef _HM_BUCKET_H
#define _HM_BUCKET_H

#include "hm_pair.h"

#include <stdint.h>

int hm_list_exists(hm_pair_t * const *bucket, const char *key);

const hm_pair_t *hm_list_get(hm_pair_t * const *bucket, const char *key);

uint64_t hm_list_count(hm_pair_t * const *bucket);

int hm_list_put(hm_pair_t **bucket, hm_pair_t *newpair);

int hm_list_remove(hm_pair_t **bucket, const char *key);

void hm_list_print(hm_pair_t * const *bucket);

int hm_list_free(hm_pair_t **bucket);

#endif

