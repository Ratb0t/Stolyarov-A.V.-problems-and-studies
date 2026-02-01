Executable := main.exe
Executable_test := test.exe

UTILS_PATH := $(shell pwd | sed 's|/Part4/.*|/Part4|')
#UTILS_PATH := $(subst $(space),\$(space), $(UTILS_NO_ESCAPED_PATH))

UTILS_HEADERS_DIR := $(UTILS_PATH)/Utilities/Headers
UTILS_SRC_DIR := $(UTILS_PATH)/Utilities/Src
OBJ_DIR := Build/objects
BIN_DIR := Build/bin

CC := gcc
HEADERS_CFLAGS = -I$(UTILS_HEADERS_DIR)
CFLAGS := -g -Wall -MD -c $(HEADERS_CFLAGS)
DEFINITIONS :=

SRC_C = $(wildcard *.c)
SRC_UTILS_C = $(wildcard $(UTILS_SRC_DIR)/*.c)

OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_C))\
$(patsubst $(UTILS_SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_UTILS_C))


all : $(BIN_DIR)/$(Executable) run.exe

run.exe : $(BIN_DIR)/$(Executable)
	ln -sf $(BIN_DIR)/$(Executable) run.exe

$(BIN_DIR)/$(Executable) : $(OBJ) | $(OBJ_DIR) $(BIN_DIR)
	@echo OBJ == $(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJ_DIR)/%.o : ./%.c | $(OBJ_DIR)
	$(CC) $(DEFINITIONS) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : $(UTILS_SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(DEFINITIONS) $(CFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

include $(wildcard $(OBJ_DIR)/*.d)

clean : 
	-rm $(OBJ_DIR)/*.d 
	-rm $(OBJ_DIR)/*.o
	-rm $(BIN_DIR)/*.exe 
	-rm	*.exe


get:
	@echo UTILS_PATH == $(UTILS_PATH)
	@echo =====================================================================
	@echo UTILS_HEADERS_DIR == $(UTILS_HEADERS_DIR)
	@echo =====================================================================
	@echo UTILS_SRC_DIR == $(UTILS_SRC_DIR)
	@echo =====================================================================
	@echo SRC_C == $(SRC_C)
	@echo =====================================================================
	@echo SRC_UTILS_C == $(SRC_UTILS_C)
	@echo =====================================================================
	@echo SRC_ASM == $(SRC_ASM)
	@echo =====================================================================
	@echo MAIN_OBJ == $(MAIN_OBJ)
	@echo =====================================================================
	@echo TEST_OBJ == $(TEST_OBJ)
	@echo =====================================================================
	@echo OBJ == $(OBJ)
	@echo =====================================================================

rebuild : clean all