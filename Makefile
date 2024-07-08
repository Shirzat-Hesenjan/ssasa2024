CC=gcc
CFLAGS=-Wall -std=c99

APP=sha3
# REF=sha3ref (TODO)
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=%.o)

.PHONY: all clean

all: $(APP)

$(APP): $(OBJECTS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(APP) $(OBJECTS)