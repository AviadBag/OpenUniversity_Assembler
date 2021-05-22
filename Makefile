ROOT    := .
BIN     := bin
SRC     := src
INCLUDE := include

CC       := gcc
CC_FLAG  := -Wall -ansi -pedantic -ggdb -I${INCLUDE} -I${SRC}

SOURCES := $(shell find ${SRC} -type f -name '*.c')
HEADERS := $(shell find ${ROOT} -type f -name '*.h')

EXECUTABLE := assembler

DOXYFILE       := Doxyfile
DOXYGEN_OUTPUT := html

all: ${BIN}/${EXECUTABLE}

run: clean all
	clear
	@./${BIN}/${EXECUTABLE}

docs:
	doxygen ${DOXYFILE}

${BIN}/${EXECUTABLE}: ${SOURCES} ${HEADERS}
	mkdir bin -p
	${CC} ${CC_FLAG} -o $@ ${SOURCES}

clean:
	rm -f ${BIN}/*
	rm -rf ${DOXYGEN_OUTPUT}
