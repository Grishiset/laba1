
CC = gcc


CFLAGS = -Wall -Wextra -std=c99


TARGET = main


OBJS = main.o string_collection.o


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)


main.o: main.c string_collection.h
	$(CC) $(CFLAGS) -c main.c


string_collection.o: string_collection.c string_collection.h
	$(CC) $(CFLAGS) -c string_collection.c


clean:
	rm -f $(OBJS) $(TARGET)
