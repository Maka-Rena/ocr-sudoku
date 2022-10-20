# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS := -lm


SRC = ${wildcard ../../libraries/matrix_lib/*.c} ${wildcard main_xor.c}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean build all

all: build

build: ${OBJ}
	@$(CC) -o main_xor $^ $(LDFLAGS) $(CFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main_xor
# END
