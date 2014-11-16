#include "hm_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define PRINT_FORMAT "| %10" PRIu64 " | %-20s | %-20s | %20p | %20p |\n"


const HMPair *hm_list_get(HMPair * const *bucket, const char *key){
	if (key == NULL)
		return NULL;

	const HMPair *pair;
	for(pair = *bucket; pair; pair = pair->next){
		// Check if this is what we are looking for
		if (hm_pair_equalkey(pair, key))
			return pair;
	}

	return NULL;
}


inline int hm_list_exists(HMPair * const *bucket, const char *key){
	return hm_list_get(bucket, key) ? 1 : 0;
}


uint64_t hm_list_count(HMPair * const *bucket){
	uint64_t count = 0;

	const HMPair *pair;
	for(pair = *bucket; pair; pair = pair->next){
		count++;
	}

	return count;
}


int hm_list_put(HMPair **bucket, HMPair *newpair){
	if (newpair == NULL)
		return 0;

	// delete first
	// tests shows this is not expencive operation.
	const char *key = hm_pair_getkey(newpair);
	hm_list_remove(bucket, key);

	// add at the beginning...
	newpair->next = *bucket;

	*bucket = newpair;

	return 1;
}


int hm_list_remove(HMPair **bucket, const char *key){
	if (key == NULL)
		return 0;

	HMPair *prev = NULL;
	HMPair *pair;
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


void hm_list_print(HMPair * const *bucket){
	printf("\n");
	printf("Print bucket %p\n", bucket);
	printf("\n");

//	printf("Pairs count = %" PRIu64 "\n", bucket->count);
//	printf("\n");

	uint64_t i = 0;
	const HMPair *pair;
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


int hm_list_free(HMPair **bucket){
	HMPair *copy;
	HMPair *pair;
	for(pair = *bucket; pair; ){
		copy = pair;

		pair = pair->next;

		free(copy);
	}

	*bucket = NULL;

	return 1;
}


