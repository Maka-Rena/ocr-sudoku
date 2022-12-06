# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -g -O3 `pkg-config --cflags sdl2 SDL2_image` 
LDFLAGS := -lm
LDLIBS = `pkg-config --libs sdl2 SDL2_image ` -lm


SRC = ${wildcard ../../libraries/matrix_lib/libs/*.c} ${wildcard ../../libraries/data_set_lib/libs/*.c} ${wildcard ./libs/*.c} ${wildcard main_neuralnetwork.c}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean build all

all: build

build: ${OBJ}
	@$(CC) -o main_neuralnetwork $^ $(LDFLAGS) $(CFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main_neuralnetwork
	${RM} -r testing_net
# END
