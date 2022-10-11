#Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =
SOLVER_DIR = solver

SRC = ${wildcard ./includes/*.c} ${wildcard ./libs/*.c} ${wildcard main.c}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean build all

all: build

build: ${OBJ}
#@$(CC) -o $(SRC_GRID_ANALYSIS) $^ $(LDFLAGS) $(LDLIBS)
#@$(CC) $(OBJ) -lm -o $(EXE)
	@$(CC) -o main $^ $(LDFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

#End Makefile

