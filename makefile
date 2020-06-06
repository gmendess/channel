CC = gcc
CC_FLAGS = -c -O3 -Wall

QUEUE = queue/queue

all: channel

channel: main.o queue.o
	$(CC) main.o queue.o -o main

main.o: main.c $(QUEUE).h
	$(CC) $(CC_FLAGS) main.c -o main.o

queue.o: $(QUEUE).c $(QUEUE).h
	$(CC) $(CC_FLAGS) $(QUEUE).c -o queue.o

clean:
	rm -f *.o main