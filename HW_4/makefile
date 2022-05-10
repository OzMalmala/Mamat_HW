CC     = gcc
CFLAGS = -Wall -g -std=c99
OBJS   = grades.o
SHARED = -llinked-list


libgrades.so: grades.o
	$(CC) -shared grades.o -o libgrades.so $(SHARED) -L.

grades.o: grades.c grades.h
	$(CC) -c -g -fpic grades.c

clean:
	rm -rf *.o 