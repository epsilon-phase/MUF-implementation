ifndef CC
CC = gcc
endif
CFLAGS = -g -MP -MD -Wall 
ifdef DEBUG
CFLAGS+=-fsanitize=leak -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -O0
endif
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
EXEC=terp
terp: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(CFLAGS) -o $@

-include $(SOURCES:.c=.d)
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
