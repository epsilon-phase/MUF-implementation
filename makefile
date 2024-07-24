ifndef CC
CC = gcc
endif
CFLAGS += -g -MP -MD -Wall -Wextra $(shell pcre-config --cflags) 
LDFLAGS += $(shell pcre-config --libs) -lpthread
ifdef RELEASE
	CFLAGS+=-O3
	LDFLAGS+=-flto
else
	CFLAGS+=-fsanitize=leak -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -O0
	LDFLAGS+=-lasan
endif
LDFLAGS+=-lm
VPATH=./src
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXEC=terp
terp: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(CFLAGS) -o $@

-include $(SOURCES:.c=.d)
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
src/ansi.c: src/ansi.rl
	ragel -C -o $@  $<
ansi: src/ansi.c
	$(CC) -DANSI_TEST src/ansi.c $(CFLAGS) -I src -o ansi
clean:
	rm -f $(EXEC) $(OBJECTS)
