dictionary: dictionary.c dictionary.h
	gcc -c dictionary.c

cross: cross-ref.c
	gcc -c cross-ref.c

build: dictionary cross
	gcc dictionary.o cross-ref.o -o cross-ref

debug: dictionary cross
	gcc -g dictionary.o cross-ref.o -o cross-ref

run: build
	./cross-ref irving-little-573.txt

valgrind: build	
	valgrind --leak-check=yes ./cross-ref irving-london-598.txt