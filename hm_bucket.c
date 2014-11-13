#include "hm_bucket.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define PRINT_BUFFER_SIZE 20
#define PRINT_FORMAT "| %10u | %-20s | %-20s |\n"

static uint64_t _hm_bucket_getpairid(const HMBucket *bucket, const HMPair *chkpair){
	if (chkpair == NULL)
		return 0;

	uint64_t i;
	for(i = 0; i < bucket->count; i++){
		HMPair *pair = bucket->pairs[i];

		if (pair)
			if (hm_pair_equalp(pair, chkpair))
				return i + 1;
	}

	return 0;
}


static uint64_t _hm_bucket_getpairids(const HMBucket *bucket, const char *key){
	if (key == NULL)
		return 0;

	uint64_t i;
	for(i = 0; i < bucket->count; i++){
		HMPair *pair = bucket->pairs[i];

		if (pair)
			if ( hm_pair_equals(pair, key) )
				return i + 1;
	}

	return 0;
}


inline int hm_bucket_exists(const HMBucket *bucket, const char *key){
	return _hm_bucket_getpairids(bucket, key) ? 1 : 0;
}

HMPair *hm_bucket_get(HMBucket *bucket, const char *key){
	if (key == 0)
		return NULL;

	// is key already in the bucket?
	const uint64_t pos = _hm_bucket_getpairids(bucket, key);

	if (pos == 0)
		return NULL;

	return bucket->pairs[pos - 1];
}

int hm_bucket_put(HMBucket *bucket, HMPair *newpair){
	if (newpair == NULL)
		return 0;


	if (bucket->count == 0){
		// make new bucket
		HMPair **pairs = malloc(sizeof(HMPair *));

		if (pairs == NULL){
			// not enought memory, but pairs were NULL anyway.
			return 0;
		}

		bucket->pairs = pairs;

		bucket->pairs[0] = newpair;
		bucket->count = 1;

		return 1;
	}


	// is key already in the bucket?
	const uint64_t pos = _hm_bucket_getpairid(bucket, newpair);

	if (pos){
		// replace the pair
		free( bucket->pairs[pos - 1] );
		bucket->pairs[pos - 1] = newpair;

		return 1;
	}

	// seems, the bucket is non empty and pair is not in the bucket
	HMPair **relocated_pairs = realloc(bucket->pairs, (bucket->count + 1) * sizeof(HMPair *));

	if (relocated_pairs == NULL) {
		// error, but pairs are untouched
		return 0;
	}

	bucket->pairs = relocated_pairs;
	bucket->count++;

	bucket->pairs[bucket->count - 1] = newpair;

	return 1;
}


int hm_bucket_remove(HMBucket *bucket, const char *key){
	if (key == NULL)
		return 0;

	// is key already in the bucket?
	uint64_t pos = _hm_bucket_getpairids(bucket, key);

	if (pos == 0){
		// no problem to remove something that does not exists
		return 1;
	}

	free( bucket->pairs[pos - 1] );

	if (bucket->count == 1){
		// delete everything...

		free(bucket->pairs);
		bucket->count = 0;
		return 1;
	}

	if (pos < bucket->count){
		// "swap" with last one
		bucket->pairs[pos - 1] = bucket->pairs[bucket->count - 1];
		// clean up in case realloc fails...
		bucket->pairs[bucket->count - 1] = NULL;
	}

	// remove last element
	HMPair **relocated_pairs = realloc(bucket->pairs, (bucket->count - 1) * sizeof(HMPair *));
	if (relocated_pairs == NULL) {
		// error, but pairs are untouched
		// last one is NULL, but this is relatively OK.
		return 0;
	}

	bucket->pairs = relocated_pairs;
	bucket->count--;

	return 1;
}


uint64_t hm_bucket_count(const HMBucket *bucket){
	uint64_t count = 0;

	uint64_t i;
	for(i = 0; i < bucket->count; i++){
		HMPair *pair = bucket->pairs[i];
		if (hm_pair_valid(pair))
			count++;
	}

	return count;
}


int hm_bucket_freepairs(HMBucket *bucket){
	if (bucket->count == 0)
		return 0;

	uint64_t i;
	for(i = 0; i < bucket->count; i++)
		free( bucket->pairs[i] );

	free(bucket->pairs);

	bucket->count = 0;

	return 1;
}


void hm_bucket_print(const HMBucket *bucket){
	char buffer1[PRINT_BUFFER_SIZE];
	char buffer2[PRINT_BUFFER_SIZE];

	printf("\n");
	printf("Print bucket %p\n", bucket);
	printf("\n");

	printf("Pairs count = %" PRIu64 "\n", bucket->count);
	printf("\n");

	if (bucket->count == 0){
		printf("No pairs available...\n");
		return;
	}

	uint32_t i;
	for(i = 0; i < bucket->count; i++){
		HMPair *pair = bucket->pairs[i];

		if (pair == NULL){
			printf(PRINT_FORMAT, i, "NULL", "NULL");
			continue;
		}

		printf(PRINT_FORMAT,
			i,
			hm_pair_getkey(pair, buffer1, PRINT_BUFFER_SIZE),
			hm_pair_getval(pair, buffer2, PRINT_BUFFER_SIZE)
		);
	}
}

