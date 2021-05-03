ROOT    := .
BIN     := bin
SRC     := src
INCLUDE := include

CC       := gcc
CC_FLAG  := -Wall -ansi -pedantic -ggdb -I${INCLUDE}

SOURCES := $(shell find ${SRC} -type f -name '*.c')
HEADERS := $(shell find ${ROOT} -type f -name '*.h')

EXECUTABLE := assembler

all: ${BIN}/${EXECUTABLE}

run: clean all
	clear
	@./${BIN}/${EXECUTABLE}

${BIN}/${EXECUTABLE}: ${HEADERS} ${SOURCES}
	${CC} ${CC_FLAG} -o $@ $^

clean:
	rm -f ${BIN}/*
