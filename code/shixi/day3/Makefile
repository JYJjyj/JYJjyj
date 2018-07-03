.PHONY:clean

OBJS = main.o\
	   parse.o

CC = gcc
CFLAGS=-Wall -g

sxshell : $(OBJS)
	$(CC) $(CFLASGS) -o $@ $(OBJS)
%.o:%.c
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJS) sxshell
