#include "hm.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>

#define hash(a) hm_hash(a)

#define BUFFER_SIZE 1024


unsigned long int hm_hash(const char *str){
	unsigned long hash = 5381;

	unsigned int c;
	// double () is to avoid C warning...
	while( (c = *str++) ) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}


HM *hm_create(uint64_t capacity){
	HM *table = malloc( sizeof(HM) + sizeof(HMBucket) * capacity);

	if (table == NULL)
		return NULL;

	table->capacity = capacity;

	memset(table->buckets, 0, sizeof(HMBucket) * capacity);

	return table;
};


int hm_free(HM *table){
	// giant memory leak here :)

	free(table);

	return 1;
}


inline uint64_t _hm_getbucketforkey(HM *table, const char *key){
	if (key == NULL)
		return 0;

	return hash(key) % table->capacity + 1;
}


int hm_exists(HM *table, const char *key){
	if (key == 0)
		return 0;

	uint64_t index = _hm_getbucketforkey(table, key );

	if (index == 0)
		return 0;

	HMBucket *bucket = & table->buckets[index - 1];

	return hm_bucket_exists(bucket, key);
}


HMPair *hm_get(HM *table, const char *key){
	if (key == 0)
		return NULL;

	uint64_t index = _hm_getbucketforkey(table, key );

	if (index == 0)
		return NULL;

	HMBucket *bucket = & table->buckets[index - 1];

	return hm_bucket_get(bucket, key);
}


int hm_put(HM *table, HMPair *pair){
	if (pair == NULL)
		return 0;

	if (! hm_pair_valid(pair)){
		free(pair);
		return 0;
	}

	char buffer[BUFFER_SIZE];

	uint64_t index = _hm_getbucketforkey(table, hm_pair_getkey(pair, buffer, BUFFER_SIZE) );

	if (index == 0)
		return 0;

	HMBucket *bucket = & table->buckets[index - 1];

	if (hm_bucket_put(bucket, pair))
		return 1;

	free(pair);
	return 0;
}


int hm_remove(HM *table, const char *key){
	if (key == NULL)
		return 0;

	uint64_t index = _hm_getbucketforkey(table, key);

	if (index == 0)
		return 0;

	HMBucket *bucket = & table->buckets[index - 1];

	return hm_bucket_remove(bucket, key);
}


void hm_print(const HM *table, int all){
	printf("\n");
	printf("HM Table %p\n", table);
	printf("\n");
	printf("Capacity = %" PRIu64 "\n", table->capacity);
	printf("\n");

	uint64_t sum = 0;
	uint64_t i;
	for(i = 0; i < table->capacity; i++){
		const HMBucket *bucket = & table->buckets[i];

		printf("| Bucket # %10" PRIu64 " | Pairs: %10" PRIu64 " |\n", i, bucket->count);
		sum += bucket->count;

		if (all)
			hm_bucket_print(bucket);
	}

	printf("\n");

	printf("Total %" PRIu64 " (eventual) pairs...\n", sum);
}

