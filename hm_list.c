#include "hm_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define PRINT_FORMAT "| %10" PRIu64 " | %-20s | %-20s | %20p | %20p |\n"



static inline int _hm_list_put(hm_pair_t **bucket, hm_pair_t *newpair);

static inline int _hm_list_put_dumb(hm_pair_t **bucket, hm_pair_t *newpair);
static inline int _hm_list_put_eventual(hm_pair_t **bucket, hm_pair_t *newpair);

static inline const hm_pair_t *_hm_list_get_dumb(hm_pair_t * const *bucket, const char *key);
static inline const hm_pair_t *_hm_list_get_eventual(hm_pair_t * const *bucket, const char *key);

static inline uint64_t _hm_list_count_dumb(hm_pair_t * const *bucket);
static inline uint64_t _hm_list_count_eventual(hm_pair_t * const *bucket);

static inline int _hm_list_remove_dumb(hm_pair_t **bucket, const char *key);
static inline int _hm_list_remove_eventual(hm_pair_t **bucket, const char *key);





const hm_pair_t *hm_list_get(hm_pair_t * const *bucket, const char *key){
	return _hm_list_get_eventual(bucket, key);
}

int hm_list_put(hm_pair_t **bucket, hm_pair_t *newpair){
	return _hm_list_put_eventual(bucket, newpair);
}

uint64_t hm_list_count(hm_pair_t * const *bucket){
	return _hm_list_count_eventual(bucket);
}

int hm_list_remove(hm_pair_t **bucket, const char *key){
	return _hm_list_remove_eventual(bucket, key);
}





int hm_list_exists(hm_pair_t * const *bucket, const char *key){
	return hm_list_get(bucket, key) ? 1 : 0;
}

void hm_list_print(hm_pair_t * const *bucket){
	printf("\n");
	printf("Print bucket %p\n", bucket);
	printf("\n");

//	printf("Pairs count = %" PRIu64 "\n", bucket->count);
//	printf("\n");

	uint64_t i = 0;
	const hm_pair_t *pair;
	for(pair = *bucket; pair; pair = pair->next){
		printf(PRINT_FORMAT,
			i,
			hm_pair_getkey(pair),
			hm_pair_getval(pair),
			pair,
			pair->next
		);

		i++;
	}

	if (i == 0){
		printf("No pairs found...\n");
	}

	printf("\n");
}


int hm_list_free(hm_pair_t **bucket){
	hm_pair_t *copy;
	hm_pair_t *pair;
	for(pair = *bucket; pair; ){
		copy = pair;

		pair = pair->next;

		free(copy);
	}

	*bucket = NULL;

	return 1;
}





static inline int _hm_list_put(hm_pair_t **bucket, hm_pair_t *newpair){
	if (newpair == NULL)
		return 0;

	// do not check for duplicates at all...

	// add at the beginning...
	newpair->next = *bucket;

	*bucket = newpair;

	return 1;
}





/*
 * DUMP solution do not take care of "eventual" things
 *
 */

static inline int _hm_list_put_dumb(hm_pair_t **bucket, hm_pair_t *newpair){
	// delete first
	// tests shows this is not expencive operation.
	const char *key = hm_pair_getkey(newpair);
	hm_list_remove(bucket, key);

	// go ahead with basic put()
	_hm_list_put(bucket, newpair);

	return 1;
}

static inline const hm_pair_t *_hm_list_get_dumb(hm_pair_t * const *bucket, const char *key){
	if (key == NULL)
		return NULL;

	const hm_pair_t *pair;
	for(pair = *bucket; pair; pair = pair->next){
		// Check if this is what we are looking for
		if (hm_pair_equalkey(pair, key))
			return pair;
	}

	return NULL;
}

static inline uint64_t _hm_list_count_dumb(hm_pair_t * const *bucket){
	uint64_t count = 0;

	const hm_pair_t *pair;
	for(pair = *bucket; pair; pair = pair->next){
		count++;
	}

	return count;
}

static inline int _hm_list_remove_dumb(hm_pair_t **bucket, const char *key){
	if (key == NULL)
		return 0;

	hm_pair_t *prev = NULL;
	hm_pair_t *pair;
	for(pair = *bucket; pair; pair = pair->next){
		// Check if this is what we are looking for
		if (hm_pair_equalkey(pair, key)){
			if (prev){
				prev->next = pair->next;
			}else{
				// First node...
				*bucket = pair->next;
			}

			free(pair);

			return 1;
		}

		prev = pair;
	}

	return 1;
}





/*
 * EVENTUAL solution takes care of "eventual" things,
 * such when Pair is made and when it expires
 *
 */

static inline const hm_pair_t *_hm_list_get_eventual(hm_pair_t * const *bucket, const char *key){
	const hm_pair_t *pair = _hm_list_get_dumb(bucket, key);

	if (pair == NULL)
		return NULL;

	if ( hm_pair_valid(pair) )
		return pair;

	return NULL;
}

static inline int _hm_list_put_eventual(hm_pair_t **bucket, hm_pair_t *newpair){
	// this needs to be completly rewritten for speed.
	// however then code became very complicated.

	if (newpair == NULL)
		return 0;

	const char *key = hm_pair_getkey(newpair);
	const hm_pair_t *pair = _hm_list_get_eventual(bucket, key);

	if (pair != NULL){
		// check if the data in database is newer than "newpair"
		if (pair->created > newpair->created){
			// prevent memory leak
			free(newpair);
			return 0;
		}
	}

	// go ahead with dumb solution
	return _hm_list_put_dumb(bucket, newpair);
}

static inline uint64_t _hm_list_count_eventual(hm_pair_t * const *bucket){
	uint64_t count = 0;

	const hm_pair_t *pair;
	for(pair = *bucket; pair; pair = pair->next){
		if (hm_pair_valid(pair))
			count++;
	}

	return count;
}

static inline int _hm_list_remove_eventual(hm_pair_t **bucket, const char *key){
	return _hm_list_remove_dumb(bucket, key);
}


