BIN     := bin
SRC     := src
INCLUDE := include

CC       := gcc
CC_FLAG  := -Wall -ansi -pedantic -ggdb -I${INCLUDE}

SOURCES := $(shell find ${SRC} -type f -name '*.c')

EXECUTABLE := assembler

all: ${BIN}/${EXECUTABLE}

run: clean all
	clear
	@./${BIN}/${EXECUTABLE}

${BIN}/${EXECUTABLE}: ${SOURCES}
	${CC} ${CC_FLAG} -o $@ $^

clean:
	rm -f ${BIN}/*
