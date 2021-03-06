CC   = gcc -Wall -c
LINK = gcc -o



all: test_hm test_hm_file



clean:
	rm -f *.o test_hm_pair hm



malloc_info.o: malloc_info.c malloc_info.h
	$(CC) malloc_info.c



test_hm: test_hm.o hm_pair.o hm_list.o hm.o malloc_info.o
	$(LINK) test_hm test_hm.o hm_pair.o hm_list.o hm.o malloc_info.o

test_hm.o: test_hm.c hm_pair.h hm_list.h hm.h malloc_info.h
	$(CC) test_hm.c



test_hm_file: test_hm_file.o hm_pair.o hm_list.o hm.o malloc_info.o
	$(LINK) test_hm_file test_hm_file.o hm_pair.o hm_list.o hm.o malloc_info.o

test_hm_file.o: test_hm_file.c hm_pair.h hm_list.h hm.h
	$(CC) test_hm_file.c



hm_pair.o: hm_pair.c hm_pair.h
	$(CC) hm_pair.c



hm_list.o: hm_list.c hm_list.h hm_pair.h
	$(CC) hm_list.c



hm.o: hm.c hm.h hm_list.h hm_pair.h
	$(CC) hm.c


