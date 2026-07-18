.PHONY: all run clean

BIN := centi.beats
SRC := main.c

CFLAGS := -O3 -g
LDFLAGS := -static

all: run

$(BIN): $(SRC)
	gcc $(LDFLAGS) $(CFLAGS) $(SRC) -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

