# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: grayscale gaussian

SRC = grayscale.c gaussian.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grayscale: grayscale.o
gaussian: gaussian.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END