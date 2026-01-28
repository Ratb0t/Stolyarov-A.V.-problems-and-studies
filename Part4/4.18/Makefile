
Executable = main.exe
Executable_test = test.exe

HEADERS_DIR = Src/Headers
UTILS_HEADERS_DIR = /workspaces/Stolyarov-A.V.-problems-and-studies/Part4/Utilities/Headers
SRC_DIR = Src
UTILS_SRC_DIR = /workspaces/Stolyarov-A.V.-problems-and-studies/Part4/Utilities/Src
OBJ_DIR = Build/objects
BIN_DIR = Build/bin

CC = gcc
ASM = nasm
HEADERS_CFLAGS = -I$(HEADERS_DIR)/ -I$(UTILS_HEADERS_DIR)/
CFLAGS = -g -Wall -MD -c $(HEADERS_CFLAGS)/
ASMFLAGS = -f elf64 -g -F dwarf
DEFINITIONS = #-DDEBUG

SRC_C = $(wildcard $(SRC_DIR)/*.c)
SRC_UTILS_C = $(wildcard $(UTILS_SRC_DIR)/*.c)
SRC_ASM = $(wildcard $(SRC_DIR)/*.asm)

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_C))\
$(patsubst $(UTILS_SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_UTILS_C))\
$(patsubst $(SRC_DIR)/%.asm, $(OBJ_DIR)/%.o, $(SRC_ASM))\

TEST_OBJ = $(filter-out $(OBJ_DIR)/main.o, $(OBJ))
MAIN_OBJ = $(filter-out $(OBJ_DIR)/test.o, $(OBJ))


all : $(BIN_DIR)/$(Executable) $(BIN_DIR)/$(Executable_test) test.exe run.exe 

run.exe: $(BIN_DIR)/$(Executable) 
	ln -sf $(BIN_DIR)/$(Executable) run.exe

test.exe: $(BIN_DIR)/$(Executable_test) 
	ln -sf $(BIN_DIR)/$(Executable_test) test.exe

suite_tests : $(BIN_DIR)/$(Executable_test)
	./$(BIN_DIR)/$(Executable_test)

$(BIN_DIR)/$(Executable) : $(MAIN_OBJ) | $(OBJ_DIR) $(BIN_DIR)
	$(CC) $(MAIN_OBJ) -o $@

$(BIN_DIR)/$(Executable_test) : $(TEST_OBJ) | $(OBJ_DIR) $(BIN_DIR)
	$(CC) $(TEST_OBJ) -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR) 
	$(CC) $(DEFINITIONS) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : $(UTILS_SRC_DIR)/%.c | $(OBJ_DIR) 
	$(CC) $(DEFINITIONS) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.asm | $(OBJ_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

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
	echo $(SRC_C)
	echo =====================================================================
	echo $(SRC_UTILS_C)
	echo =====================================================================
	echo $(SRC_ASM)
	echo =====================================================================
	echo main_obj == $(MAIN_OBJ)
	echo =====================================================================
	echo test_obj == $(TEST_OBJ)
	echo =====================================================================
	echo obj == $(OBJ)
	echo =====================================================================

rebuild : clean all