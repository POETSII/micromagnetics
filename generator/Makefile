CFLAGS = -O3 -Wall -pedantic -std=c89
LFLAGS = -lm

all: libgenerator.so generator templater

clean:
	rm --force templater generator libgenerator.so

generator: generator.c initial_conditions.c template.c template.h
	${CC} ${CFLAGS} $^ -o $@ ${LFLAGS}

libgenerator.so: generator.c initial_conditions.c template.c template.h
	${CC} ${CFLAGS} -shared -fPIC -o $@ $^ ${LFLAGS}

templater: template_executable.c template.c template.h
	${CC} ${CFLAGS} $^ -o $@

.PHONY: all clean
