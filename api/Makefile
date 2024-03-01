SRC	 = $(wildcard main/*.c http/*.c routes/*.c)
OBJ	 = $(SRC:.c=.o)

CC	  := gcc
CFLAGS  +=   -Wall -Werror -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter \
			-I.

TARGET  := rinha-api
PREFIX  := ./test-root

all: build

build: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS) $(LFLAGS)

install:
	install -s ./$(TARGET) $(PREFIX)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
