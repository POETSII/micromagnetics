STE_DIR = ste
CFLAGS = -O3 -Wall -pedantic -std=c89 -I./$(TEMPLATE_DIR)
LFLAGS = -lm -lbsd

STE_FILES = $(TEMPLATE_DIR)/ste.c \
            $(TEMPLATE_DIR)/ste.h \
            $(TEMPLATE_DIR)/string_helpers.c \
            $(TEMPLATE_DIR)/string_helpers.h

all: libgenerator.so generator

clean:
	rm --force generator libgenerator.so

generator: generator.c initial_conditions.c $(STE_FILES)
	${CC} ${CFLAGS} $(filter %.c, $^) -o $@ ${LFLAGS}

libgenerator.so: generator.c initial_conditions.c $(STE_FILES)
	${CC} ${CFLAGS} -shared -fPIC $(filter %.c, $^) -o $@ ${LFLAGS}

.PHONY: all clean
