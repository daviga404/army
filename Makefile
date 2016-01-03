BIN=bin
INPUT=army.c
OUTPUT=$(BIN)/army.exe
CC=gcc
CCFLAGS=-Wall -c
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