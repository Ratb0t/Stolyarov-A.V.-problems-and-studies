BIN_DIR := Build/Bin
OBJ_DIR := Build/Obj

EXECUTABLE := main.exe

UTILS_PATH := ../Utilities
UTILS_SRC_PATH := $(UTILS_PATH)/Src
UTILS_HEADERS_DIR := $(UTILS_PATH)/Headers
SRC_DIR := Src
HEADERS_DIR := $(SRC_DIR)/Headers

INCLUDE_DIRS = -I$(UTILS_HEADERS_DIR) -I$(HEADERS_DIR)

CC_FLAGS = -g -Wall -Werror=vla -Wextra -Wfloat-equal $(INCLUDE_DIRS)
CC := gcc

SRC_C := $(wildcard $(SRC_DIR)/*.c)
SRC_UTILS = $(UTILS_SRC_PATH)/my_list.c $(UTILS_SRC_PATH)/my_string.c $(UTILS_SRC_PATH)/utilities_functions.c
OBJECTS = $(patsubst $(UTILS_SRC_PATH)/%.c, $(OBJ_DIR)/%.o, $(SRC_UTILS))\
$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_C))


run.exe : $(BIN_DIR)/$(EXECUTABLE) 
	ln -sf $(BIN_DIR)/$(EXECUTABLE) run.exe

$(BIN_DIR)/$(EXECUTABLE) : $(OBJECTS) | $(OBJ_DIR) $(BIN_DIR)
	gcc $(OBJECTS) -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR) 
	$(CC) -c $(CC_FLAGS) $< -o $@
	
$(OBJ_DIR)/%.o : $(UTILS_SRC_PATH)/%.c | $(OBJ_DIR) 
	$(CC) -c $(CC_FLAGS) $< -o $@
	

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

get:
	@echo $(UTILS_HEADERS_DIR)
	@echo gcc -g -Wall -I$(UTILS_HEADERS_DIR) $< -o $@
	@echo
	@echo $(OBJECTS)
	@echo
	@echo $(SRC_UTILS)
	@echo
	@echo OBJ_DIR == $(OBJ_DIR)
	@echo
	@echo $(OBJ_DIR)/%.o : $(SRC_UTILS)/%.c
	@echo
	@echo "SRC_C = $(SRC_C)"


clear :
	@rm -rf Build

rebuild : clear run.exe