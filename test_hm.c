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

	hm_pair_t *p1 = hm_pair_create(key, val);
	hm_pair_t *p2 = hm_pair_createx(key, val, 1);

	if (p1 == NULL){
		printf("Not enought memory!\n");
		return;
	}

	PRINTF_TEST("hm_pair_t", "equals",		hm_pair_equalkey(p1, "1234567890abc")		);
	PRINTF_TEST("hm_pair_t", "equals",		! hm_pair_equalkey(p1, "1234567890abc123") 	);
	PRINTF_TEST("hm_pair_t", "! equals",	! hm_pair_equalkey(p1, "xxxxxx")		);
	PRINTF_TEST("hm_pair_t", "key",		strcmp(hm_pair_getkey(p1), key) == 0		);
	PRINTF_TEST("hm_pair_t", "val",		strcmp(hm_pair_getval(p1), val) == 0		);


	PRINTF_TEST("hm_pair_t", "valid",		hm_pair_valid(p1)				);
	PRINTF_TEST("hm_pair_t", "valid",		hm_pair_valid(p2)				);

	if (delay){
	printf("sleep for 1 sec...\n");
	sleep(1);
	PRINTF_TEST("hm_pair_t", "valid",		hm_pair_valid(p1)				);
	PRINTF_TEST("hm_pair_t", "valid",		hm_pair_valid(p2) == 0				);
	}

	free(p1);
	free(p2);
}


void hm_list_test(){
	hm_pair_t *list_real = NULL;
	hm_pair_t **list = & list_real;

	hm_list_put(list, hm_pair_create("name",	"niki"		));	// put element in empty list
	hm_list_put(list, hm_pair_create("age",		"5"		));	// put as first element
	hm_list_put(list, hm_pair_create("lang",	"C/C++"		));	// put as non first element

	hm_list_put(list, hm_pair_create("age",	"8"			));	// must remove existing "age"

	hm_list_put(list, hm_pair_create("_first",	"list top"	));	// put as first element

	PRINTF_TEST("HM List", "count",		hm_list_count(list) == 4			);
	PRINTF_TEST("HM List", "exists",	hm_list_exists(list, "name")			);
	PRINTF_TEST("HM List", "! exists",	! hm_list_exists(list, "computer")		);

	PRINTF_TEST("HM List", "! get",		hm_list_get(list, "computer") == NULL		);

	const hm_pair_t *pair = hm_list_get(list, "name");
	PRINTF_TEST("HM List", "get",		hm_pair_equalkey(pair, "name")			);

	hm_list_print(list);

	hm_list_remove(list, NULL	);
	hm_list_remove(list, "computer"	);	// remove something not in the list
	hm_list_remove(list, "_first"	);	// remove first element of the list
	hm_list_remove(list, "lang"	);	// remove non first element of the list

	PRINTF_TEST("HM List", "count",		hm_list_count(list) == 2			);

	hm_list_remove(list, "name"	);
	hm_list_remove(list, "age"	);	// list must be empty now

	PRINTF_TEST("HM List", "count 0",	hm_list_count(list) == 0			);

	// ==================================

	hm_pair_t *eventual_pair =  hm_pair_create("ev_pair", "one");
	hm_list_put(list, eventual_pair);
	eventual_pair->created += 3600 * 1000; // set time in the future
	hm_list_put(list, hm_pair_create("ev_pair",	"two"	));
	const hm_pair_t *eventual_pair2 = hm_list_get(list, "ev_pair");
	PRINTF_TEST("HM List", "eventual get",	strcmp(hm_pair_getval(eventual_pair2), "one") == 0	);

	// ==================================

	PRINTF_TEST("HM List", "free",		hm_list_free(list) 				);
	PRINTF_TEST("HM List", "count",		hm_list_count(list) == 0			);

	// add some more pairs to test hm_list_free() with data
	hm_list_put(list, hm_pair_create("name",	"niki"	));
	hm_list_put(list, hm_pair_create("age",	"5"		));
	hm_list_put(list, hm_pair_create("lang",	"C/C++"	));


	PRINTF_TEST("HM List", "free",		hm_list_free(list)				);
	PRINTF_TEST("HM List", "count",		hm_list_count(list) == 0			);
}


void hm_table_test(){
	const uint64_t capacity = 3;
 	HM *table = hm_create(capacity);

	hm_put(table, hm_pair_create("name",	"niki"		));
	hm_put(table, hm_pair_create("age",	"5"		));
	hm_put(table, hm_pair_create("lang",	"C/C++"		));
	hm_put(table, hm_pair_create("os",	"Linux"		));
	hm_put(table, hm_pair_create("hw",	"ASUS Zenbook"	));
	hm_put(table, hm_pair_create("hw.cpu",	"i5"		));
	hm_put(table, hm_pair_create("hw.ram",	"10 GB"		));
	hm_put(table, hm_pair_create("hw.ssd",	"24 GB"		));
	hm_put(table, hm_pair_create("hw.hdd",	"500 GB"	));

	PRINTF_TEST("HM", "exists",	hm_exists(table, "name")		);
	PRINTF_TEST("HM", "! exists",	! hm_exists(table, "computer")		);

	PRINTF_TEST("HM", "! get",	hm_get(table, "computer") == NULL	);
	const hm_pair_t *pair = hm_get(table, "name");
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
	PRINTF_SIZE(hm_pair_t);
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

