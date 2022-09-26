STE_DIR = ste
CFLAGS = -O3 -Wall -pedantic -std=c89 -I./$(STE_DIR)
LFLAGS = -lm

STE_FILES = $(STE_DIR)/ste.c \
            $(STE_DIR)/ste.h \
            $(STE_DIR)/string_helpers.c \
            $(STE_DIR)/string_helpers.h

all: libgenerator.so generator

clean:
	rm --force generator libgenerator.so

generator: generator.c initial_conditions.c $(STE_FILES)
	${CC} ${CFLAGS} $(filter %.c, $^) -o $@ ${LFLAGS}

libgenerator.so: generator.c initial_conditions.c $(STE_FILES)
	${CC} ${CFLAGS} -shared -fPIC $(filter %.c, $^) -o $@ ${LFLAGS}

.PHONY: all clean
