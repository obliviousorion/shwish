CC ?= cc
CFLAGS ?= -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Iinclude
LDFLAGS ?=

TARGET := shwish
SOURCES := src/main.c src/tokenize.c src/exec.c
OBJECTS := $(SOURCES:.c=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)
test: $(TARGET)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fsanitize=address,undefined -o tests/test_tokenize tests/test_tokenize.c src/tokenize.c
	./tests/test_tokenize
	sh tests/compare.sh
