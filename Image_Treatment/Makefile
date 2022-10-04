# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: pre_processing

SRC = pre_processing.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grayscale: pre_processing.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END