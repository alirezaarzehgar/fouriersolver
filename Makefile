SRC = main.c
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra
BIN = bin/_fouriersolver

.PHONY: all
all: $(BIN)

$(BIN): bin $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

bin:
	mkdir bin

install:

clean:
	rm -rf bin $(OBJ)

.PHONY: all install clean
