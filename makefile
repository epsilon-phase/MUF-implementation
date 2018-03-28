CC = gcc
CFLAGS = -g -MP -MD -Wall 
#CFLAGS+=-fsanitize=leak -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined
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
