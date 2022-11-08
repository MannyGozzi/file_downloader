CC = gcc
CFLAGS = -O0 -g -Wall -std=c99 -pedantic -g
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXECS = $(SRCS:%.c=%)

downloader: $(OBJS)

clean:
	@rm -f $(EXECS) $(OBJS)
