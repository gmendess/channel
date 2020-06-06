CC = gcc
CC_FLAGS = -c -O3 -Wall

QUEUE = queue/queue
CHAN = chan/chan

all: channel

channel: main.o queue.o chan.o
	$(CC) main.o queue.o chan.o -lpthread -o main

main.o: main.c $(QUEUE).h
	$(CC) $(CC_FLAGS) main.c -o main.o

queue.o: $(QUEUE).c $(QUEUE).h
	$(CC) $(CC_FLAGS) $(QUEUE).c -o queue.o

chan.o: $(CHAN).c $(CHAN).h
	$(CC) $(CC_FLAGS) $(CHAN).c -o chan.o

clean:
	rm -f *.o main