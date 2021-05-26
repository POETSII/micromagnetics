TEMPLATE_DIR = templater
CFLAGS = -O3 -Wall -pedantic -std=c89 -I./$(TEMPLATE_DIR)
LFLAGS = -lm -lbsd

all: libgenerator.so generator

clean:
	rm --force generator libgenerator.so

generator: generator.c initial_conditions.c \
$(TEMPLATE_DIR)/template.c $(TEMPLATE_DIR)/template.h
	${CC} ${CFLAGS} $^ -o $@ ${LFLAGS}

libgenerator.so: generator.c initial_conditions.c \
$(TEMPLATE_DIR)/template.c $(TEMPLATE_DIR)/template.h
	${CC} ${CFLAGS} -shared -fPIC -o $@ $^ ${LFLAGS}

.PHONY: all clean
