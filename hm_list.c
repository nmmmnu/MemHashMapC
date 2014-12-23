#include "hm_list.h"

#include <stdlib.h>	// free
#include <stdio.h>
#include <inttypes.h>	// PRIu64


#define PRINT_FORMAT "| %10" PRIu64 " | %-20s | %-20s | %20p | %20p |\n"


static int _hm_list_put_eventual(hm_pair_t **head, hm_pair_t *newpair);
static const hm_pair_t *_hm_list_get_eventual(hm_pair_t * const *head, const char *key);
static int _hm_list_remove_eventual(hm_pair_t **head, const char *key);
static inline uint64_t _hm_list_count_eventual(hm_pair_t * const *head);



int hm_list_put(hm_pair_t **head, hm_pair_t *newpair){
	return _hm_list_put_eventual(head, newpair);
}

const hm_pair_t *hm_list_get(hm_pair_t * const *head, const char *key){
	return _hm_list_get_eventual(head, key);
}

int hm_list_remove(hm_pair_t **head, const char *key){
	return _hm_list_remove_eventual(head, key);
}

uint64_t hm_list_count(hm_pair_t * const *head){
	return _hm_list_count_eventual(head);
}

int hm_list_free(hm_pair_t **head){
	hm_pair_t *copy;
	hm_pair_t *pair;
	for(pair = *head; pair; ){
		copy = pair;

		pair = pair->next;

		free(copy);
	}

	*head = NULL;

	return 1;
}



void hm_list_print(hm_pair_t * const *head){
	printf("\n");
	printf("HM List @ %p\n", head);
	printf("\n");

//	printf("Pairs count = %" PRIu64 "\n", bucket->count);
//	printf("\n");

	uint64_t i = 0;
	const hm_pair_t *pair;
	for(pair = *head; pair; pair = pair->next){
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



/*
 *
 * PUT a pair into the list
 *
 */

static int _hm_list_put_basic(hm_pair_t **head, hm_pair_t *newpair){
	if (newpair == NULL)
		return 0;

	const char *key = hm_pair_getkey(newpair);

	hm_pair_t *prev = NULL;
	hm_pair_t *pair;
	for(pair = *head; pair; pair = pair->next){
		int cmp = hm_pair_cmpkey(pair, key);

		/*
		if (cmp == 0){
			// handle delete somehow
		}
		*/

		if (cmp >= 0)
			break;

		prev = pair;
	}

	// put new pair here...
	if (prev){
		// we are at the middle
		newpair-> next = prev->next;
		prev->next = newpair;
	}else{
		newpair->next = *head;
		*head = newpair;
	}

	return 1;
}

static int _hm_list_put_dumb(hm_pair_t **head, hm_pair_t *newpair){
	// delete first
	// tests shows this is not expencive operation.
	const char *key = hm_pair_getkey(newpair);
	hm_list_remove(head, key);

	// go ahead with basic put()
	_hm_list_put_basic(head, newpair);

	return 1;
}

static int _hm_list_put_eventual(hm_pair_t **head, hm_pair_t *newpair){
	// this needs to be completly rewritten for speed.
	// however then code became very complicated.

	if (newpair == NULL)
		return 0;

	const char *key = hm_pair_getkey(newpair);
	const hm_pair_t *pair = _hm_list_get_eventual(head, key);

	if (pair != NULL){
		// check if the data in database is newer than "newpair"
		if (pair->created > newpair->created){
			// prevent memory leak
			free(newpair);
			return 0;
		}
	}

	// go ahead with dumb solution
	return _hm_list_put_dumb(head, newpair);
}

/*
 *
 * GET a pair from the list
 *
 */

static const hm_pair_t *_hm_list_get_dumb(hm_pair_t * const *head, const char *key){
	if (key == NULL)
		return NULL;

	const hm_pair_t *pair;
	for(pair = *head; pair; pair = pair->next){
		int cmp = hm_pair_cmpkey(pair, key);

		if (cmp == 0)
			return pair;

		if (cmp > 0)
			break;
	}

	return NULL;
}

static const hm_pair_t *_hm_list_get_eventual(hm_pair_t * const *head, const char *key){
	const hm_pair_t *pair = _hm_list_get_dumb(head, key);

	if (pair == NULL)
		return NULL;

	if ( hm_pair_valid(pair) )
		return pair;

	return NULL;
}

/*
 *
 * REMOVE a pair from the list
 *
 */

static int _hm_list_remove_dumb(hm_pair_t **head, const char *key){
	if (key == NULL)
		return 0;

	hm_pair_t *prev = NULL;
	hm_pair_t *pair;
	for(pair = *head; pair; pair = pair->next){
		int cmp = hm_pair_cmpkey(pair, key);

		if (cmp == 0){
			if (prev){
				prev->next = pair->next;
			}else{
				// First node...
				*head = pair->next;
			}

			free(pair);

			return 1;
		}

		if (cmp >= 0)
			break;

		prev = pair;
	}

	return 1;
}

static int _hm_list_remove_eventual(hm_pair_t **head, const char *key){
	return _hm_list_remove_dumb(head, key);
}

/*
 *
 * COUNT pairs in the list
 *
 */


static uint64_t _hm_list_count_dumb(hm_pair_t * const *head){
	uint64_t count = 0;

	const hm_pair_t *pair;
	for(pair = *head; pair; pair = pair->next){
		if (hm_pair_valid(pair))
			count++;
	}

	return count;
}


static uint64_t _hm_list_count_eventual(hm_pair_t * const *head){
	return _hm_list_count_dumb(head);
}

