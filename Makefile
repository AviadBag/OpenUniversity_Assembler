ROOT    := .
BIN     := bin
SRC     := src
INCLUDE := include

CC       := gcc
CC_FLAG  := -Wall -ansi -pedantic -ggdb -I${INCLUDE} -I${SRC}

SOURCES := $(shell find ${SRC} -type f -name '*.c')
HEADERS := $(shell find ${ROOT} -type f -name '*.h')

EXECUTABLE := assembler

all: ${BIN}/${EXECUTABLE}

run: clean all
	clear
	@./${BIN}/${EXECUTABLE}

${BIN}/${EXECUTABLE}: ${SOURCES} ${HEADERS}
	${CC} ${CC_FLAG} -o $@ ${SOURCES}

clean:
	rm -f ${BIN}/*
