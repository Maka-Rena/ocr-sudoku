
# Makefile

CC := gcc
CPPFLAGS :=
CFLAGS := -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -Wno-unknown-pragmas
LDFLAGS := -lm
LDLIBS := `pkg-config --libs sdl2 SDL2_image`
EXEC := sudoc
EXEC_TEST := test
EXEC_SOLVER := solver

BUILD_DIR := build
#TEST_DATA_DIR := ./tests/out

SRC := ./sudoc/main.c

SOLVER_SRC :=	${wildcard ./sudoc/solver/libs/*.c} ./sudoc/solver/solverMain.c

TEST_SRC :=	${wildcard ./sudoc/solver/libs/*.c} \
			${wildcard ./sudoc/tests/libs/*.c} \
			${wildcard ./tests/libs/*.c} \
			${wildcard ./tests/*.c} \
			${wildcard ./sudoc/libraries/matrix_lib/*.c}


OBJ := ${SRC:.c=.o}
TEST_OBJ := ${TEST_SRC:.c=.o}
SOLVER_OBJ := ${SOLVER_SRC:.c=.o}

.PHONY: build all

all: build build-solver build-test build-web clean-sudoc clean-test

# BUILD
build: $(OBJ)
	@mkdir -p $(BUILD_DIR)
	@$(CC) -o $(BUILD_DIR)/$(EXEC) $^ $(LDFLAGS) $(LDLIBS)

build-test: $(TEST_OBJ)
	@mkdir -p $(BUILD_DIR)
	@$(CC) -o $(BUILD_DIR)/$(EXEC_TEST) $^ $(LDFLAGS) $(LDLIBS)

build-solver: $(SOLVER_OBJ)
	@mkdir -p $(BUILD_DIR)
	@$(CC) -o $(BUILD_DIR)/$(EXEC_SOLVER) $^ $(LDFLAGS) $(LDLIBS)

build-web:
#	@nix-shell -p nodejs-18_x
	@cd web/website/src/components/Upload/ && mkdir process && cd ../../../ && npm install && npm run start

build-api:
#	@nix-shell -p nodejs-18_x
	@cd web/api/ && npm install && clear && node index.js

# RUN
run: build clean-sudoc
	@./$(BUILD_DIR)/$(EXEC)

test: build-test
	@./$(BUILD_DIR)/$(EXEC_TEST)

web: build-web
	@echo "Starting Web"

api: build-api
	@echo "Starting API"
# CLEAN
clean-sudoc:
	${RM} ${OBJ}

clean-test:
	${RM} ${TEST_OBJ}

clean-solver:
	${RM} ${SOLVER_OBJ}

clean-data:
	${RM} -rf ${TEST_DATA_DIR}

clean-web: 
	@cd web/website/src/components/Upload && rm -rf process/

clean: clean-sudoc clean-test clean-solver clean-data clean-web
	${RM} -r $(BUILD_DIR)