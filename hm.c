#include "hm.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>

#define hash(a) hm_hash(a)

// DJB Hash function from CDB
static unsigned long int hm_hash(const char *str){
	unsigned long hash = 5381;

	unsigned int c;
	// double () is to avoid C warning...
	while( (c = *str++) ) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}


HM *hm_create(capacity_t capacity){
	HM *table = malloc(sizeof(HM) + sizeof(hm_pair_t *) * capacity);

	if (table == NULL)
		return NULL;

	table->capacity = capacity;

	/*
	capacity_t i;
	for(i = 0; i < capacity; i++){
		table->buckets[i] = NULL;
	}
	*/

	memset(table->buckets, 0, sizeof(hm_pair_t *) * capacity);

	return table;
};


int hm_free(HM *table){
	capacity_t i;
	for(i = 0; i < table->capacity; i++){
		hm_list_free( & table->buckets[i] );
	}

	free(table);

	return 1;
}


static inline capacity_t _hm_getbucketforkey(HM *table, const char *key){
	if (key == NULL)
		return 0;

	return hash(key) % table->capacity + 1;
}


int hm_exists(HM *table, const char *key){
	if (key == 0)
		return 0;

	const capacity_t index = _hm_getbucketforkey(table, key);

	if (index == 0)
		return 0;

	hm_pair_t **bucket = & table->buckets[index - 1];

	return hm_list_exists(bucket, key);
}


const hm_pair_t *hm_get(HM *table, const char *key){
	if (key == 0)
		return NULL;

	capacity_t index = _hm_getbucketforkey(table, key );

	if (index == 0)
		return NULL;

	hm_pair_t **bucket = & table->buckets[index - 1];

	return hm_list_get(bucket , key);
}


int hm_put(HM *table, hm_pair_t *pair){
	if (pair == NULL)
		return 0;

	capacity_t index = _hm_getbucketforkey(table, hm_pair_getkey(pair) );

	if (index == 0)
		return 0;

	hm_pair_t **bucket = & table->buckets[index - 1];

	if (hm_list_put(bucket, pair))
		return 1;

	free(pair);

	return 0;
}


int hm_remove(HM *table, const char *key){
	if (key == NULL)
		return 0;

	capacity_t index = _hm_getbucketforkey(table, key);

	if (index == 0)
		return 0;

	hm_pair_t **bucket = & table->buckets[index - 1];

	return hm_list_remove(bucket, key);
}


void hm_print(HM *table, int all){
	printf("\n");
	printf("HM Table @ %p\n", table);
	printf("\n");
	printf("Capacity = %" PRIu64 "\n", (uint64_t) table->capacity);
	printf("\n");

	if (! all)
		return;

	capacity_t i;
	for(i = 0; i < table->capacity; i++){
		hm_list_print( & table->buckets[i] );
	}

	printf("\n");
}

