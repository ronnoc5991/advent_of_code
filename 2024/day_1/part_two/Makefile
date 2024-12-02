compiler := gcc
flags := -Wall
binary := output

$(binary): main.c
	$(compiler) $^ -o $(binary) $(flags)

clean:
	rm $(binary)
