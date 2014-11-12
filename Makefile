CC   = gcc -Wall -c
LINK = gcc -o



all: test_hm



clean:
	rm -f *.o test_hm_pair hm



test_hm: test_hm.o hm_pair.o hm_bucket.o
	$(LINK) test_hm test_hm.o hm_pair.o hm_bucket.o

test_hm.o: test_hm.c hm_pair.h hm_bucket.h
	$(CC) test_hm.c



hm_pair.o: hm_pair.c hm_pair.h
	$(CC) hm_pair.c



hm_bucket.o: hm_bucket.c hm_bucket.h hm_pair.h
	$(CC) hm_bucket.c



hm: hm.o
	$(LINK) hm hm.o hm_pair.o

hm.o: hm.c hm.h hm_pair.h
	$(CC) hm.c


