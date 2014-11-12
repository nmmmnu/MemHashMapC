#include "hm_pair.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>


void hm_print_sizes(){
	const char *format = "size of %-10s %u\n";
	
	printf("\nSizes:\n");
	
	printf(format,	"int",		sizeof(int)		);
	printf(format,	"long int",	sizeof(long int)	);
	printf(format,	"void *",	sizeof(void *)		);
	       
	printf(format,	"uint32_t",	sizeof(uint32_t)	);
	printf(format,	"uint64_t",	sizeof(uint64_t)	);
               
	printf(format,	"HMPair",	sizeof(HMPair)		);
}


void hm_pair_test(){
	const char *TEST_FORMAT = "%-30s %s\n";

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

	printf(TEST_FORMAT, "Test equals",	hm_pair_equals(p1, key) 				? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test ! equals",	! hm_pair_equals(p1, "xxxxxx")				? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test key",		strcmp(hm_pair_getkey(p1, buffer, max), key) == 0	? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test val",		strcmp(hm_pair_getval(p1, buffer, max), val) == 0	? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test valid",	hm_pair_valid(p1)					? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test expired",	hm_pair_expired(p1) == 0				? "OK" : "Fail"	);

	printf(TEST_FORMAT, "Test expired",	hm_pair_expired(p2) == 0				? "OK" : "Fail"	);
	printf("sleep for 1 sec...\n");
	sleep(1);
	printf(TEST_FORMAT, "Test expired",	hm_pair_expired(p2)					? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test ! valid",	hm_pair_valid(p2)					? "OK" : "Fail"	);
	
	hm_print_sizes();
}


int main(int argc, char **argv){
	hm_pair_test();

	return 0;
}
