SRC = main.c
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -lm
BIN = fouriersolver

.PHONY: all
all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

clean:
	rm -rf $(OBJ) ${BIN}

.PHONY: all install clean bin
