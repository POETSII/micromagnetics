CFLAGS = -O3 -Wall -pedantic -std=c89 -lm

all: libgenerator.so generator templater

clean:
	rm --force templater generator libgenerator.so

generator: generator.c initial_conditions.c template.c template.h
	${CC} ${CFLAGS} $^ -o $@

libgenerator.so: generator.c initial_conditions.c template.c template.h
	${CC} ${CFLAGS} -shared -fPIC -o $@ $^

templater: template_executable.c template.c template.h
	${CC} ${CFLAGS} $^ -o $@

.PHONY: all clean
