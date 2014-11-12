#include "hm_pair.h"
#include "hm_bucket.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>


const char *TEST_FORMAT = "%-20s %-30s %s\n";


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

	printf(TEST_FORMAT, "HMPair", "Test equals",	hm_pair_equals(p1, "1234567890abc") 			? "OK" : "Fail"	);
	printf(TEST_FORMAT, "HMPair", "Test equals",	! hm_pair_equals(p1, "1234567890abc123") 		? "OK" : "Fail"	);
	printf(TEST_FORMAT, "HMPair", "Test ! equals",	! hm_pair_equals(p1, "xxxxxx")				? "OK" : "Fail"	);
	printf(TEST_FORMAT, "HMPair", "Test key",	strcmp(hm_pair_getkey(p1, buffer, max), key) == 0	? "OK" : "Fail"	);
	printf(TEST_FORMAT, "HMPair", "Test val",	strcmp(hm_pair_getval(p1, buffer, max), val) == 0	? "OK" : "Fail"	);
	printf(TEST_FORMAT, "HMPair", "Test valid",	hm_pair_valid(p1)					? "OK" : "Fail"	);
	printf(TEST_FORMAT, "HMPair", "Test expired",	hm_pair_expired(p1) == 0				? "OK" : "Fail"	);

	printf(TEST_FORMAT, "HMPair", "Test expired",	hm_pair_expired(p2) == 0				? "OK" : "Fail"	);
	if (delay){
	printf("sleep for 1 sec...\n");
	sleep(1);
	printf(TEST_FORMAT, "HMPair", "Test expired",	hm_pair_expired(p2)					? "OK" : "Fail"	);
	printf(TEST_FORMAT, "HMPair", "Test ! valid",	hm_pair_valid(p2)					? "OK" : "Fail"	);
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

	printf(TEST_FORMAT, "HMBucket", "Test count",		hm_bucket_count(bucket) == 3		? "OK" : "Fail"	);
	printf(TEST_FORMAT, "HMBucket", "Test exists",		hm_bucket_exists(bucket, "name")	? "OK" : "Fail"	);
	printf(TEST_FORMAT, "HMBucket", "Test ! exists",	! hm_bucket_exists(bucket, "computer")	? "OK" : "Fail"	);

	hm_bucket_print(bucket);

	hm_bucket_remove(bucket, NULL);
	hm_bucket_remove(bucket, "computer");
	hm_bucket_remove(bucket, "lang");

	printf(TEST_FORMAT, "HMBucket", "Test count",		hm_bucket_count(bucket) == 2		? "OK" : "Fail"	);

	hm_bucket_remove(bucket, "name");
	hm_bucket_remove(bucket, "age");

	printf(TEST_FORMAT, "HMBucket", "Test count",		hm_bucket_count(bucket) == 0		? "OK" : "Fail"	);

	printf(TEST_FORMAT, "HMBucket", "Test free",		hm_bucket_free(bucket) == 0		? "OK" : "Fail"	);

	// add some more pairs to test hm_bucket_free() with data
	hm_bucket_put(bucket, hm_pair_create("name",	"niki",		0));
	hm_bucket_put(bucket, hm_pair_create("age",	"5",		0));
	hm_bucket_put(bucket, hm_pair_create("lang",	"C/C++",	0));

	printf(TEST_FORMAT, "HMBucket", "Test free",		hm_bucket_free(bucket)			? "OK" : "Fail"	);
}


void hm_print_sizes(){
	const char *format = "size of %-10s %u\n";

	printf("\nSizes:\n");

	printf(format,	"int",		sizeof(int)		);
	printf(format,	"long int",	sizeof(long int)	);
	printf(format,	"void *",	sizeof(void *)		);

	printf(format,	"uint32_t",	sizeof(uint32_t)	);
	printf(format,	"uint64_t",	sizeof(uint64_t)	);

	printf(format,	"HMPair",	sizeof(HMPair)		);
	printf(format,	"HMBucket",	sizeof(HMBucket)	);

}


int main(int argc, char **argv){
	hm_pair_test(0);

	hm_bucket_test();

	hm_print_sizes();

	return 0;
}
