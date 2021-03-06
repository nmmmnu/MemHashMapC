#ifndef _HM_H
#define _HM_H

#include "hm_pair.h"
#include "hm_list.h"

#include <stdint.h>

typedef uint64_t capacity_t;

typedef struct _HM_t{
	capacity_t	capacity;	// 8
	hm_pair_t	*buckets[];	// dynamic array of pointers to link lists
} HM;


HM *hm_create(uint64_t capacity);
int hm_free(HM *table);

int hm_exists(HM *table, const char *key);

const hm_pair_t *hm_get(HM *table, const char *key);
int hm_put(HM *table, hm_pair_t *pair);
int hm_remove(HM *table, const char *key);

void hm_print(HM *table, int all);

#endif
