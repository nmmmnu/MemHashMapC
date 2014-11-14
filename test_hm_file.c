#include "hm_pair.h"
#include "hm_bucket.h"
#include "hm.h"

#include "malloc_info.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 1024

#define BUCKETS 1 * 1000 * 1000

void processfile(HM *table, const char *filename, int op);
char *trim(char *s);


//#define getchar()

int main(int argc, char **argv){
	const char *filename = argv[1];
	const char *findkey = argv[2];

	display_mallinfo();

	HM *table = hm_create(BUCKETS);

	char buffer[MAX];

	printf("Load file\n");
	processfile(table, filename, 0);

	const HMPair *pair = hm_get(table, findkey);

	if (pair == NULL)
		printf("Key %s not found\n", findkey);
	else
		printf("Key %s found - %s\n", findkey, hm_pair_getval(pair, buffer, MAX));

	display_mallinfo();

	getchar();

	//hm_print(table, 0);


	printf("Free file\n");
	//processfile(table, filename, 1);

	hm_free(table);

	printf("Done... %p\n", table);

	display_mallinfo();

	getchar();

	return 0;
}

void processfile(HM *table, const char *filename, int op){
	char buffer[MAX];

	FILE *f = fopen(filename, "r");

	unsigned int i = 0;
	char *key;
	while( (key = fgets(buffer, MAX, f)) ){
		trim(key);

		switch(op){
		case 0:
			hm_put(table, hm_pair_create(key, key, 0));
			break;
		case 1:
			hm_remove(table, key);
			break;

		}

		i++;
		if (i % ( 10 * 1000 * 1000 ) == 0)
			printf("Processed %10u...\n", i);
	}

	fclose(f);

	printf("Done...\n");
}


char *trim(char *s){
	size_t i;
	for(i = strlen(s); i > 0; i--)
		if (isspace(s[i]) || s[i] == '\0')
			s[i] = '\0';
		else
			break;

	return s;
}

