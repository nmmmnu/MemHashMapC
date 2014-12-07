#include "hm_pair.h"
#include "hm_list.h"
#include "hm.h"

#include "malloc_info.h"

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

	HMPair *p1 = hm_pair_create(key, val);
	HMPair *p2 = hm_pair_createx(key, val, 1);

	if (p1 == NULL){
		printf("Not enought memory!\n");
		return;
	}

	PRINTF_TEST("HMPair", "equals",		hm_pair_equalkey(p1, "1234567890abc")		);
	PRINTF_TEST("HMPair", "equals",		! hm_pair_equalkey(p1, "1234567890abc123") 	);
	PRINTF_TEST("HMPair", "! equals",	! hm_pair_equalkey(p1, "xxxxxx")		);
	PRINTF_TEST("HMPair", "key",		strcmp(hm_pair_getkey(p1), key) == 0		);
	PRINTF_TEST("HMPair", "val",		strcmp(hm_pair_getval(p1), val) == 0		);


	PRINTF_TEST("HMPair", "valid",		hm_pair_valid(p1)				);
	PRINTF_TEST("HMPair", "valid",		hm_pair_valid(p2)				);

	if (delay){
	printf("sleep for 1 sec...\n");
	sleep(1);
	PRINTF_TEST("HMPair", "valid",		hm_pair_valid(p1)				);
	PRINTF_TEST("HMPair", "valid",		hm_pair_valid(p2) == 0				);
	}

	free(p1);
	free(p2);
}


void hm_list_test(){
	HMPair *bucket[1];
	bucket[0] = NULL;

	hm_list_put(bucket, hm_pair_create("name",	"niki"		));
	hm_list_put(bucket, hm_pair_create("age",	"5"		));
	hm_list_put(bucket, hm_pair_create("lang",	"C/C++"		));

	// must remove existing "age"
	hm_list_put(bucket, hm_pair_create("age",	"8"		));

	// must be placed in front of the list.
	hm_list_put(bucket, hm_pair_create("first",	"list top"	));

	PRINTF_TEST("HM List", "count",		hm_list_count(bucket) == 4			);
	PRINTF_TEST("HM List", "exists",	hm_list_exists(bucket, "name")			);
	PRINTF_TEST("HM List", "! exists",	! hm_list_exists(bucket, "computer")		);

	PRINTF_TEST("HM List", "! get",		hm_list_get(bucket, "computer") == NULL		);
	const HMPair *pair = hm_list_get(bucket, "name");
	PRINTF_TEST("HM List", "get",		hm_pair_equalkey(pair, "name")			);

	hm_list_print(bucket);

	hm_list_remove(bucket, NULL);
	hm_list_remove(bucket, "computer");	// remove something not in the list
	hm_list_remove(bucket, "first");	// remove first element of the list
	hm_list_remove(bucket, "lang");		// remove non first element of the list

	PRINTF_TEST("HM List", "count",		hm_list_count(bucket) == 2			);

	hm_list_remove(bucket, "name");
	hm_list_remove(bucket, "age");

	PRINTF_TEST("HM List", "count",		hm_list_count(bucket) == 0			);

	// ==================================

	HMPair *eventual_pair =  hm_pair_create("ev_pair", "one");
	hm_list_put(bucket, eventual_pair);
	eventual_pair->created += 3600 * 1000; // set time in the future
	hm_list_put(bucket, hm_pair_create("ev_pair",	"two"	));
	const HMPair *eventual_pair2 = hm_list_get(bucket, "ev_pair");
	PRINTF_TEST("HM List", "eventual get",	strcmp(hm_pair_getval(eventual_pair2), "one") == 0	);

	// ==================================

	PRINTF_TEST("HM List", "free",		hm_list_free(bucket) 				);
	PRINTF_TEST("HM List", "count",		hm_list_count(bucket) == 0			);

	// add some more pairs to test hm_list_free() with data
	hm_list_put(bucket, hm_pair_create("name",	"niki"	));
	hm_list_put(bucket, hm_pair_create("age",	"5"	));
	hm_list_put(bucket, hm_pair_create("lang",	"C/C++"	));


	PRINTF_TEST("HM List", "free",		hm_list_free(bucket)				);
	PRINTF_TEST("HM List", "count",		hm_list_count(bucket) == 0			);
}


void hm_table_test(){
	const uint64_t capacity = 3;
 	HM *table = hm_create(capacity);

	hm_put(table, hm_pair_create("name",	"niki"		));
	hm_put(table, hm_pair_create("age",	"5"		));
	hm_put(table, hm_pair_create("lang",	"C/C++"		));
	hm_put(table, hm_pair_create("os",	"Linux"		));

	PRINTF_TEST("HM", "exists",	hm_exists(table, "name")		);
	PRINTF_TEST("HM", "! exists",	! hm_exists(table, "computer")		);

	PRINTF_TEST("HM", "! get",	hm_get(table, "computer") == NULL	);
	const HMPair *pair = hm_get(table, "name");
	PRINTF_TEST("HM", "get",	hm_pair_equalkey(pair, "name")		);

	hm_remove(table, "name");
	hm_remove(table, "age");

	hm_print(table, 1);

	hm_free(table);
}


#define PRINTF_SIZE_STR(a) # a
#define PRINTF_SIZE(a) printf("size of %-14s = %3zu bytes\n", PRINTF_SIZE_STR(a), sizeof(a))

void hm_print_sizes(){
	printf("\n");
	printf("Sizes:\n");

	PRINTF_SIZE(short int);
	PRINTF_SIZE(int);
	PRINTF_SIZE(long int);
	PRINTF_SIZE(long long int);
	PRINTF_SIZE(void *);
	PRINTF_SIZE(uint16_t);
	PRINTF_SIZE(uint32_t);
	PRINTF_SIZE(uint64_t);
	PRINTF_SIZE(HMPair);
	PRINTF_SIZE(HM);
}

#undef PRINTF_SIZE
#undef PRINTF_SIZE_STR


int main(int argc, char **argv){
	hm_pair_test(argc > 1);

	hm_list_test();

	hm_table_test();

	hm_print_sizes();

	display_mallinfo();

	return 0;
}

