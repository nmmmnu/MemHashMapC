#include "hm_pair.h"
#include "hm_bucket.h"
#include "hm.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>


#define PRINTF_TEST(module, test, func) \
	printf( "%-10s Testing %-20s %s\n", module, test, func ? "OK" : "Fail")

void hm_pair_test(const int delay){

	const char *key = "1234567890abc";
	const char *val = "1234567890xyz";

	HMPair *p1 = hm_pair_create(key, val, 0);
	HMPair *p2 = hm_pair_create(key, val, 1);

	if (p1 == NULL || p2 == NULL){
		printf("Not enought memory!\n");
		return;
	}

	const unsigned short int max = 1024;

	char buffer[max];

	PRINTF_TEST("HMPair", "equals",		hm_pair_equals(p1, "1234567890abc")			);
	PRINTF_TEST("HMPair", "equals",		! hm_pair_equals(p1, "1234567890abc123") 		);
	PRINTF_TEST("HMPair", "! equals",	! hm_pair_equals(p1, "xxxxxx")				);
	PRINTF_TEST("HMPair", "key",		strcmp(hm_pair_getkey(p1, buffer, max), key) == 0	);
	PRINTF_TEST("HMPair", "val",		strcmp(hm_pair_getval(p1, buffer, max), val) == 0	);
	PRINTF_TEST("HMPair", "valid",		hm_pair_valid(p1)					);
	PRINTF_TEST("HMPair", "expired",	hm_pair_expired(p1) == 0				);

	PRINTF_TEST("HMPair", "expired",	hm_pair_expired(p2) == 0				);
	if (delay){
	printf("sleep for 1 sec...\n");
	sleep(1);
	PRINTF_TEST("HMPair", "expired",	hm_pair_expired(p2)					);
	PRINTF_TEST("HMPair", "! valid",	hm_pair_valid(p2)					);
	}

	free(p1);
	free(p2);
}


void hm_bucket_test(){
	HMBucket bucket2 = {0, NULL};
	HMBucket *bucket = & bucket2;

	hm_bucket_put(bucket, hm_pair_create("name",	"niki",		0));
	hm_bucket_put(bucket, hm_pair_create("age",	"5",		0));
	hm_bucket_put(bucket, hm_pair_create("lang",	"C/C++",	0));

	PRINTF_TEST("HMBucket", "count",	hm_bucket_count(bucket) == 3				);
	PRINTF_TEST("HMBucket", "exists",	hm_bucket_exists(bucket, "name")			);
	PRINTF_TEST("HMBucket", "! exists",	! hm_bucket_exists(bucket, "computer")			);

	PRINTF_TEST("HMBucket", "! get",	hm_bucket_get(bucket, "computer") == NULL		);
	HMPair *pair = hm_bucket_get(bucket, "name");
	PRINTF_TEST("HMBucket", "get",		hm_pair_equals(pair, "name")				);

	hm_bucket_print(bucket);

	hm_bucket_remove(bucket, NULL);
	hm_bucket_remove(bucket, "computer");
	hm_bucket_remove(bucket, "lang");

	PRINTF_TEST("HMBucket", "count",	hm_bucket_count(bucket) == 2				);

	hm_bucket_remove(bucket, "name");
	hm_bucket_remove(bucket, "age");

	PRINTF_TEST("HMBucket", "count",	hm_bucket_count(bucket) == 0				);

	PRINTF_TEST("HMBucket", "free",		hm_bucket_freepairs(bucket) == 0			);

	// add some more pairs to test hm_bucket_free() with data
	hm_bucket_put(bucket, hm_pair_create("name",	"niki",		0));
	hm_bucket_put(bucket, hm_pair_create("age",	"5",		0));
	hm_bucket_put(bucket, hm_pair_create("lang",	"C/C++",	0));

	PRINTF_TEST("HMBucket", "free",		hm_bucket_freepairs(bucket)				);
}


void hm_table_test(){
	const uint64_t capacity = 2;
	HM *table = hm_create(capacity);

	hm_put(table, hm_pair_create("name",	"niki",		0));
	hm_put(table, hm_pair_create("age",	"5",		0));
	hm_put(table, hm_pair_create("lang",	"C/C++",	0));

	PRINTF_TEST("HM", "exists",	hm_exists(table, "name")		);
	PRINTF_TEST("HM", "! exists",	! hm_exists(table, "computer")		);

	PRINTF_TEST("HM", "! get",	hm_get(table, "computer") == NULL	);
	HMPair *pair = hm_get(table, "name");
	PRINTF_TEST("HM", "get",	hm_pair_equals(pair, "name")		);

	hm_print(table, 1);

	hm_remove(table, "name");
	hm_remove(table, "age");
	hm_remove(table, "lang");

	hm_print(table, 0);


	hm_free(table);
}


#define PRINTF_SIZE_STR(a) # a
#define PRINTF_SIZE(a) printf("size of %-14s = %3lu bytes\n", PRINTF_SIZE_STR(a), sizeof(a))

void hm_print_sizes(){
	printf("\nSizes:\n");

	PRINTF_SIZE(short int);
	PRINTF_SIZE(int);
	PRINTF_SIZE(long int);
	PRINTF_SIZE(long long int);
	PRINTF_SIZE(void *);
	PRINTF_SIZE(uint16_t);
	PRINTF_SIZE(uint32_t);
	PRINTF_SIZE(uint64_t);
	PRINTF_SIZE(HMPair);
	PRINTF_SIZE(HMBucket);
	PRINTF_SIZE(HM);
}

#undef PRINTF_SIZE
#undef PRINTF_SIZE_STR


int main(int argc, char **argv){
	hm_pair_test(0);

	hm_bucket_test();

	hm_table_test();

	hm_print_sizes();

	return 0;
}