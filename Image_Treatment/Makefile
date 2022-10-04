# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: gray_coloring

SRC = gray_coloring.c rotate.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grayscale: gray_coloring.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END