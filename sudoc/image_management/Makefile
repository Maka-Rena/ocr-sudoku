 #Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image `
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image ` -lm
SOLVER_DIR = line_detection

SRC = ${wildcard ./includes/*.c} ${wildcard ./libs/*.c} ${wildcard ../libraries/matrix_lib/libs/matrix.c} ${wildcard play_treatment.c} #only file matrix.c to have only one main
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}


.PHONY: clean build all

all: build

build: ${OBJ}
#@$(CC) -o $(SRC_GRID_ANALYSIS) $^ $(LDFLAGS) $(LDLIBS)
#@$(CC) $(OBJ) -lm -o $(EXE)
	@$(CC) -o play_treatment $^ $(LDFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} *.png
#End Makefile

