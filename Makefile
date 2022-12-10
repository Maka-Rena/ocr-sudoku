# Makefile
CC := gcc
CFLAGS := -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -Wno-unknown-pragmas
LDFLAGS := -lm
LDLIBS := `pkg-config --libs sdl2 SDL2_image`
EXEC := sudoc
EXEC_TEST := test

BUILD_DIR := build

SRC :=	${wildcard ./sudoc/solver/libs/*.c} \
		${wildcard ./sudoc/tests/libs/*.c} \
		${wildcard ./sudoc/image_management/libs/*.c} \
		${wildcard ./sudoc/libraries/matrix_lib/libs/*.c} \
		${wildcard ./sudoc/libraries/data_set_lib/libs/*.c} \
		${wildcard ./sudoc/neural_network/digits_recog_nn/libs/*.c} \
		${wildcard ./sudoc/rotate_image/libs/*.c} \
		./sudoc/main.c

TEST_SRC := ${wildcard ./sudoc/solver/libs/*.c} \
			${wildcard ./sudoc/tests/libs/*.c} \
			${wildcard ./tests/libs/*.c} \
			${wildcard ./tests/*.c} \
			${wildcard ./sudoc/image_management/libs/*.c} \
			${wildcard ./sudoc/libraries/matrix_lib/libs/*.c} \
			${wildcard ./sudoc/libraries/data_set_lib/libs/*.c} \
			${wildcard ./sudoc/neural_network/digits_recog_nn/libs/*.c} \
			${wildcard ./sudoc/rotate_image/libs/*.c} \

OBJ := ${SRC:.c=.o}
TEST_OBJ := ${TEST_SRC:.c=.o}

.PHONY: build all

all: run

# BUILD
build: $(OBJ)
	@mkdir -p $(BUILD_DIR)
	@$(CC) -o $(BUILD_DIR)/$(EXEC) $^ $(LDFLAGS) $(LDLIBS)

build-test: $(TEST_OBJ)
	@mkdir -p $(BUILD_DIR)
	@$(CC) -o $(BUILD_DIR)/$(EXEC_TEST) $^ $(LDFLAGS) $(LDLIBS)

build-npm:
	@nix-shell -p nodejs-18_x
build-web:
	@cd web/website/src/components/Upload/ && mkdir process && cd ../../../ && npm install && npm run start

build-api:
	@cd web/api/ && npm install && clear && node index.js

# RUN
run: build
	@clear
	@./$(BUILD_DIR)/$(EXEC)

test: build-test
	@./$(BUILD_DIR)/$(EXEC_TEST)

npm: build-npm
	@echo "Starting NPM environment."

web: build-web
	@echo "Starting Web"

api: build-api
	@echo "Starting API"
# CLEAN
clean-sudoc:
	${RM} ${OBJ}

clean-test:
	${RM} ${TEST_OBJ}

clean-web: 
	@cd web/website/src/components/Upload && rm process/*.png

clean: clean-sudoc clean-test clean-web
	${RM} -rf $(BUILD_DIR)
	${RM} -rf neural_network/images/

#End Makefile