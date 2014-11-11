#ifndef _MH_H
#define _MH_H

#include "mhpair.h"

struct mhbucket{
	unsigned long int	count;		// 8
	struct mhpair **	pairs;
};

struct mh{
	unsigned long int	capacity;	// 8
	struct mhbucket 	buckets[];
};

struct mh *mh_create(unsigned int capacity);
int mh_free(struct mh *table);

int mh_put(struct mh *table, struct mhpair *pair);

unsigned long mh_hash(const char *str);

#endif

