CC=g++ $(CFLAGS)
CFLAGS=-std=c++11
inference: main.o
	$(CC) main.o -o inference.bin

main.o: main.cpp
	$(CC) -c main.cpp

clean:
	rm *.o inference.bin