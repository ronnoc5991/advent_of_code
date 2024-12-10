binary := out
compiler := gcc
flags := -Wall

$(binary): main.o linkedlist.o vector.o
	$(compiler) $^ -o $(binary) $(flags)

main.o: main.c
	$(compiler) -c main.c -o main.o $(flags)

linkedlist.o: linkedlist.c linkedlist.h
	$(compiler) -c linkedlist.c -o linkedlist.o $(flags)

vector.o: vector.c vector.h
	$(compiler) -c vector.c -o vector.o $(flags)

clean:
	rm $(binary) main.o linkedlist.o vector.o
