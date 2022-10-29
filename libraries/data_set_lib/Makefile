# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS := -lm


SRC = ${wildcard ../matrix_lib/libs/*.c} ${wildcard libs/*.c} ${wildcard main_data.c}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean build all

all: build

build: ${OBJ}
	@$(CC) -o main_data $^ $(LDFLAGS) $(CFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main_data
# END
