#Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS := -lm
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

SRC = ${wildcard ./libs/*.c}  ${wildcard main_rotate.c} 
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean build all

all: build

build: ${OBJ}
	@$(CC) -o main $^ $(LDFLAGS) $(CFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main
	${RM} out.png

#End Makefile
