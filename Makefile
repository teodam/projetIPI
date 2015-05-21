CC=gcc
CFLAGS=-W -Wall -Wextra -ansi -g -std=c99 `sdl-config --cflags`
LDFLAGS=-lm `sdl-config --libs` -lSDL -lSDL_image
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXEC=main

all: $(EXEC)

clean:
	rm *.o $(EXEC)

run:
	make && ./$(EXEC)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
