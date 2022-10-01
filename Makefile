#START MAKEFILE

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

SRC_SOLVER = ./solver/libs/*.c main.c
SOLVER_EXE = solver

all: solver-build

solver-build:
	@${CC} ${CFLAGS} -c -o ${OBJ} ${SRC_SOLVER}
	@${CC} ${OBJ} -lm -o ${SOLVER_EXE}


-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} solver

# END
