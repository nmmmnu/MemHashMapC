#include "hm.h"

#include <stdlib.h>
#include <string.h>

#define hash(a) hm_hash(a)

#define BUFFER_SIZE 1024

unsigned long hm_hash(const char *str){
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

inline unsigned int _hm_getbucketforkey(HM *table, const char *key){
	if (key == NULL)
		return 0;

	return hash(key) % table->capacity;
}

int hm_put(HM *table, HMPair *pair){
	if (pair == NULL)
		return 0;

	if (! hm_pair_valid(pair))
		return 0;

	char buffer[BUFFER_SIZE];

	unsigned int index = _hm_getbucketforkey(table, hm_pair_getkey(pair, buffer, BUFFER_SIZE) );

	if (index == 0)
		return 0;

	index--;

	if (table->buckets[index].count == 0){
		// make new bucket

		HMPair **pair = malloc(sizeof(HMPair *));

		if (pair == NULL){
			// not enought memory, but pairs were NULL anyway.
			return 0;
		}

		table->buckets[index].pairs = pair;
		table->buckets[index].count++;

		return 1;
	}

	// check if pair exists
	unsigned int i;
	for (i = 0; i < table->buckets[index].count; i++){

	}

	return 1;
}



#include <stdio.h>

int main(int argc, char **argv){
	const uint64_t CAPACITY = 1000 * 1000;

	HM *table = hm_create(CAPACITY);

	printf("%llu\n", table->buckets[123].count);
//	printf("%u\n", _hm_getbucketforkey(table, "hello1"));

	hm_free(table);

	return 0;
}
