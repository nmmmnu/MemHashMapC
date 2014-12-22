#ifndef _HM_LIST_H
#define _HM_LIST_H

#include "hm_pair.h"

int hm_list_free(hm_pair_t **bucket);

#define hm_list_exists(head, key) (hm_list_get(head, key) != NULL)

const hm_pair_t *hm_list_get(hm_pair_t * const *head, const char *key);
int hm_list_put(hm_pair_t **head, hm_pair_t *newpair);
int hm_list_remove(hm_pair_t **head, const char *key);

uint64_t hm_list_count(hm_pair_t * const *head);

void hm_list_print(hm_pair_t * const *bucket);

#endif

