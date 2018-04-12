ifndef CC
CC = gcc
endif
CFLAGS = -g -MP -MD -Wall -Wextra
ifdef DEBUG
CFLAGS+=-fsanitize=leak -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -O0
LDFLAGS+=-lasan
endif
VPATH=./src
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXEC=terp
terp: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(CFLAGS) -o $@

-include $(SOURCES:.c=.d)
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
