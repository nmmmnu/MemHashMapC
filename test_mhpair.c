#include "mhpair.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void mhpair_test(){
	const char *TEST_FORMAT = "%-30s %s\n";

	const char *key = "1234567890abc";
	const char *val = "1234567890xyz";

	struct mhpair *p1 = mhpair_create(key, val, 0);
	struct mhpair *p2 = mhpair_create(key, val, 1);

	if (p1 == NULL || p2 == NULL){
		printf("Not enought memory!\n");
		return;
	}

	const unsigned short int max = 1024;

	char buffer[max];

	printf(TEST_FORMAT, "Test equals",	mhpair_equals(p1, key) 					? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test ! equals",	! mhpair_equals(p1, "xxxxxx")				? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test key",		strcmp(mhpair_getkey(p1, buffer, max), key) == 0	? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test val",		strcmp(mhpair_getval(p1, buffer, max), val) == 0	? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test valid",	mhpair_valid(p1)					? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test expired",	mhpair_expired(p1) == 0					? "OK" : "Fail"	);

	printf(TEST_FORMAT, "Test expired",	mhpair_expired(p2) == 0					? "OK" : "Fail"	);
	printf("sleep for 1 sec...\n");
	sleep(1);
	printf(TEST_FORMAT, "Test expired",	mhpair_expired(p2)					? "OK" : "Fail"	);
	printf(TEST_FORMAT, "Test ! valid",	mhpair_valid(p2)					? "OK" : "Fail"	);
}


int main(int argc, char **argv){
	mhpair_test();

	return 0;
}
