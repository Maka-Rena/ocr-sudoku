#Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =
SOLVER_DIR = solver

SRC = ${wildcard ./includes/*.c} ${wildcard ./libs/*.c} ${wildcard solverMain.c}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean build all

all: build

build: ${OBJ}
	@$(CC) -o solver $^ $(LDFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} solver
#End Makefile
