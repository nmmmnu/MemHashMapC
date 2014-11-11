CC   = gcc -Wall -c
LINK = gcc -o



all: test_mhpair mh



clean:
	rm -f *.o test_*



test_mhpair: test_mhpair.o mhpair.o
	$(LINK) test_mhpair test_mhpair.o mhpair.o

test_mhpair.o: test_mhpair.c mhpair.h
	$(CC) test_mhpair.c

mhpair.o: mhpair.c mhpair.h
	$(CC) mhpair.c



mh: mh.o
	$(LINK) mh mh.o mhpair.o

mh.o: mh.c mh.c mhpair.h
	$(CC) mh.c


