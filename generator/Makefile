CFLAGS = -O3 -Wall -pedantic -std=c89

all: libgenerator.so generator templater

clean:
	rm --force templater generator libgenerator.so

libgenerator.so: generator.c template.c template.h
	${CC} ${CFLAGS} -shared -fPIC -o $@ $^

generator: generator.c template.c template.h
	${CC} ${CFLAGS} $^ -o $@

templater: template_executable.c template.c template.h
	${CC} ${CFLAGS} $^ -o $@

.PHONY: all clean
