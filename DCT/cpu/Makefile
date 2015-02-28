CC=g++

all: dct dct_pthread

dct: dct.cpp
	$(CC) $^ -o $@ -lm

dct_pthread: dct_pthread.cpp
	$(CC) $^ -o $@ -lpthread

.PHONY: clean
clean:
	rm dct dct_pthread
