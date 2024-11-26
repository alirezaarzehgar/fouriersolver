SRC = main.c
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -lm
BIN = fouriersolver

.PHONY: all
all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

install:
	install -s -groot -oroot -t/usr/bin/ $(BIN)
	cp $(BIN).1 /usr/share/man/man1

uninstall:
	rm /usr/bin/$(BIN) /usr/share/man/man1/$(BIN).1

clean:
	rm -rf $(OBJ) ${BIN} doc/out

doc:
	make -C doc

.PHONY: all install uninstall clean $(BIN) doc
