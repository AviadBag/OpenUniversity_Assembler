ROOT    := .
BIN     := bin
SRC     := src
INCLUDE := include

CC       := gcc
CC_FLAG  := -Wall -ansi -pedantic -ggdb -I${INCLUDE} -I${SRC} -lm 

SOURCES := $(shell find ${SRC} -type f -name '*.c')
HEADERS := $(shell find ${ROOT} -type f -name '*.h')

EXECUTABLE := assembler

DOXYFILE       := Doxyfile
DOXYGEN_OUTPUT := html

all: ${BIN}/${EXECUTABLE}

run: clean all
	clear
	@./${BIN}/${EXECUTABLE} ${ARGS}

docs:
	doxygen ${DOXYFILE}

${BIN}/${EXECUTABLE}: ${SOURCES} ${HEADERS}
	mkdir bin -p
	${CC} ${SOURCES} ${CC_FLAG} -o $@ 

clean:
	rm -f ${BIN}/*
	rm -rf ${DOXYGEN_OUTPUT}
	rm -f *.ob *.ext *.ent
