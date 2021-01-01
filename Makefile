CC=gcc
CCFLAGS=-Wall -O2 -Wextra -std=c11
TARGETS=jdu jutmp

jdu: jdu.c
	$(CC) $^ $(CCFLAGS) -o $@

jutmp: jutmp.c
	$(CC) $^ $(CCFLAGS) -o $@

clean:
	/bin/rm -f $(TARGETS)

all: $(TARGETS)
