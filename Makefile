CC   = gcc -Wall -c
LINK = gcc -o



all: test_hm_pair hm



clean:
	rm -f *.o test_hm_pair



test_hm_pair: test_hm_pair.o hm_pair.o
	$(LINK) test_hm_pair test_hm_pair.o hm_pair.o

test_hm_pair.o: test_hm_pair.c hm_pair.h
	$(CC) test_hm_pair.c

hm_pair.o: hm_pair.c hm_pair.h
	$(CC) hm_pair.c



hm: hm.o
	$(LINK) hm hm.o hm_pair.o

hm.o: hm.c hm.h hm_pair.h
	$(CC) hm.c


