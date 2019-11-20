CFLAGS = -O3 -Wall -pedantic -std=c89

all: generator templater

clean:
	rm --force templater generator

generator: generator.c template.c template.h
	${CC} ${CFLAGS} $^ -o $@

templater: template_executable.c template.c template.h
	${CC} ${CFLAGS} $^ -o $@

.PHONY: all clean
