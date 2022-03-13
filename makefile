CC := gcc
CFLAGS := -Wall -Wextra -Werror -Wfatal-errors

SOURCE := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SOURCE))
EXE := $(patsubst %.c,%,$(SOURCE))

%: %.c
	$(CC) $(CFLAGS) $^ -o $@
	
.PHONY: clean

clean:
	rm -rf $(OBJS) $(EXE)