 #Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image `
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image ` -lm
SOLVER_DIR = grid_analysis

SRC = ${wildcard ./includes/*.c} ${wildcard ./libs/*.c} ${wildcard ../libraries/matrix_lib/matrix.c} ${wildcard main_grid.c} #only file matrix.c to have only one main
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean build all

all: build

build: ${OBJ}
#@$(CC) -o $(SRC_GRID_ANALYSIS) $^ $(LDFLAGS) $(LDLIBS)
#@$(CC) $(OBJ) -lm -o $(EXE)
	@$(CC) -o main_grid $^ $(LDFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
#End Makefile

