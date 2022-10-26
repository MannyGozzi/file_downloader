CC = gcc -g -Wall
CFLAGS = -std=c99 -pedantic -g
OBJS = downloader

SOURCES = $(wildcard *.c)
EXECS = $(SOURCES:%.c=%)

all: $(EXECS)

clean :
	rm *.o $(MAIN) core
