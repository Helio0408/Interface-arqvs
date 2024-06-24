
all:
	gcc src/*.c -std='c99' -Wall -Wextra -pedantic -o programaTrab
debug:
	gcc -g src/*.c -std='c99' -Wall -Wextra -pedantic -o programaTrab
run:
	./programaTrab
