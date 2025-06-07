CC = gcc
CFLAGS = -Wall -Wextra -O0 -pedantic -g -ggdb3 -fsanitize=address

all: main

main: main.c
	$(CC) $(CFLAGS) -o main main.c src/c_tokenizer.c

run: main
	./main test_source.c

clean:
	rm -f main

.PHONY: all run clean