# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS := -lm


SRC = ${wildcard *.c} ${wildcard main_matrix.c}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean build all

all: build

build: ${OBJ}
	@$(CC) -o main_matrix $^ $(LDFLAGS) $(CFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main_matrix
# END
