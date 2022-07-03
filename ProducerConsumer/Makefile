.PHONY: all clean

CC=clang
CFLAGS+=-Wall -Werror -lpthread -DSOLUTION

all: main 

main: main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o main
