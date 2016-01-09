BIN=bin
INPUT=army.c hashtable.c
OUTPUT=$(BIN)/army.exe
CC=gcc
CCFLAGS=-Wall -c -std=c99
LD=gcc
LDFLAGS=-Wall

all: $(BIN) $(OUTPUT)

$(BIN):
	mkdir -p $(BIN)

$(OUTPUT): $(INPUT:.c=.o)
	$(LD) $(LDFLAGS) $^ -o $(OUTPUT)

%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

clean:
	rm -rf *.o $(OUTPUT)