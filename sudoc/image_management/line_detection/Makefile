#Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -g `pkg-config --cflags sdl2 SDL2_image `
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image ` -lm
SOLVER_DIR = grid_analysis

SRC = ${wildcard ./includes/*.c} ${wildcard ./libs/*.c} ${wildcard main_detection.c}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean build all

all: build

build: ${OBJ}

	@$(CC) -o main_detection $^ $(LDFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} final_picture.png
#End Makefile

