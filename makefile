CC = gcc

crf.o: create_random_file.c
	$(CC) -o crf.o -c create_random_file.c 

create_random_file: create_random_file.c crf.o
	$(CC) -o bin/$@ crf.o

gh.o: get_histogram.c
	${CC} -o gh.o -c get_histogram.c

get_histogram: get_histogram.c gh.o
	${CC} -o bin/$@ gh.o

clean:
	-rm *o bin/*
