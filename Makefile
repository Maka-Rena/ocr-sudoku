#START MAKEFILE

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =
RM = rm 

SRC = main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

SOLVER_OBJ = ${SOLVER_SRC:.c=.o}

SOLVER_SRC = ./solver/libs/*.c main.c
SOLVER_EXE = solver
BUILD_DIR = build
EXEC_TEXT = test

all: build-solver

build-solver: $(SOVER_OBJ)
	@mkdir -p $(BUILD_DIR)
	@${CC} ${CFLAGS} -c -o ${OBJ} ${SRC_SOLVER}
	@${CC} -o $(BUILD_DIR)/${SOLVER_EXE}

build-test:
	@mkdir -p $(BUILD_DIR)
	@${CC} ${CFLAGS} -c -o ${OBJ} ${SRC}
	@${CC} -o $(BUILD_DIR)/${EXEC_TEXT}

test: build-test test-clean
	@./${BUILD_DIR}/${EXEC_TEST}

-include ${DEP}

.PHONY: clean

clean:
	${RM} -rf ${BUILD_DIR}

test-clean:
	${RM} ${TEST_OBJ}

# END
