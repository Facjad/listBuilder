CC ?= gcc
CFLAGS := $(CFLAGS) -std=c99 -Wall

SRC = src/csvParser.c src/listGenerator.c src/miniesAndRules.c
OBJ = $(subst src/,bin/,$(SRC:.c=.o))

.PHONY: all clean

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) src/main.c -o listBuilder -g

bin/csvParser.o: src/csvParser.c bin/miniesAndRules.o
	$(CC) $(CFLAGS) -c $< -o $@ -g

bin/listGenerator.o: src/listGenerator.c bin/miniesAndRules.o
	$(CC) $(CFLAGS) -c $< -o $@ -g

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -g



bin/%.d: src/%.c
	@set -e; rm -f $@; \
    $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,bin/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


include $(OBJ:.o=.d)



clean:
	rm -f bin/*.o
	rm -f bin/*.d