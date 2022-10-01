#Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =
SOLVER_DIR = solver

SRC_SOLVER = ./include/*.c main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
EXE = ${SRC:.c=}

all:
	${CC} ${CFLAGS} -c -o $ ${OBJ} ${SRC_SOLVER}
	${CC} ${OBJ} -lm -o solver

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

#End Makefile
