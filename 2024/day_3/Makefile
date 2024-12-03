binary := out
compiler := gcc
flags := -Wall

$(binary): main.c
	$(compiler) $^ -o $(binary) $(flags)

clean:
	rm $(binary)
