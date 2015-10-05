CC = gcc

crf.o: create_random_file.c
	$(CC) -o crf.o -c create_random_file.c 

create_random_file: create_random_file.c crf.o
	$(CC) -o bin/$@ crf.o

clean:
	-rm *o bin/*
