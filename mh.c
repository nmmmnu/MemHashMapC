#include "mh.h"

#include <stdlib.h>
#include <string.h>

#define hash(a) mh_hash(a)

#define BUFFER_SIZE 1024

unsigned long mh_hash(const char *str){
	unsigned long hash = 5381;

	unsigned int c;
	// double () is to avoid C warning...
	while( (c = *str++) ) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

struct mh *mh_create(unsigned int capacity){
	struct mh *table = malloc( sizeof(struct mh) + sizeof(struct mhbucket) * capacity);

	if (table == NULL)
		return NULL;

	table->capacity = capacity;

	memset(table->buckets, 0, sizeof(struct mhbucket) * capacity);

	return table;
};

int mh_free(struct mh *table){
	// giant memory leak here :)

	free(table);

	return 1;
}

inline unsigned int _mh_getbucketforkey(struct mh *table, const char *key){
	if (key == NULL)
		return 0;

	return hash(key) % table->capacity;
}

int mh_put(struct mh *table, struct mhpair *pair){
	if (pair == NULL)
		return 0;

	if (! mhpair_valid(pair))
		return 0;

	char buffer[BUFFER_SIZE];

	unsigned int index = _mh_getbucketforkey(table, mhpair_getkey(pair, buffer, BUFFER_SIZE) );

	if (index == 0)
		return 0;

	index--;

	if (table->buckets[index].count == 0){
		// make new bucket

		struct mhpair **pair = malloc(sizeof(struct mhpair *));

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
	const unsigned int CAPACITY = 1000 * 1000;

	struct mh *table = mh_create(CAPACITY);

	printf("%lu\n", table->buckets[123].count);
//	printf("%u\n", _mh_getbucketforkey(table, "hello1"));

	mh_free(table);

	return 0;
}
