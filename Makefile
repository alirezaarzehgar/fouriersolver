SRC = main.c
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -lm
BIN = bin/_fouriersolver

TEST_SRC = test_exp.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_CFLAGS = -shared -fPIC
TEST_BIN = bin/test_exp.so

.PHONY: all
all: $(BIN)

$(BIN): bin $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

bin:
	mkdir -p bin

test: $(TEST_BIN)

$(TEST_BIN): bin $(TEST_OBJ)
	$(CC) $(TEST_CFLAGS) $(TEST_OBJ) -o $(TEST_BIN)

clean:
	rm -rf bin $(OBJ) $(TEST_OBJ)

.PHONY: all install clean bin test
