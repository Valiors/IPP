CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

.PHONY: all clean

all: quantization

quantization: main.o input.o trie.o eq_class.o
	$(CC) $(LDFLAGS) -o $@ $^

eq_class.o: eq_class.c eq_class.h auxiliaries.h
	$(CC) $(CFLAGS) -c $<

trie.o: trie.c trie.h eq_class.h auxiliaries.h
	$(CC) $(CFLAGS) -c $<

input.o: input.c input.h trie.h auxiliaries.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c input.h auxiliaries.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o quantization
