
Executable = main.exe
Executable_test = test.exe

OBJ_DIR = Build/objects
BIN_DIR = Build/bin
HEADERS_DIR = Src/Headers
SRC_DIR = Src

CC = gcc
ASM = nasm
CFLAGS = -g -Wall -MD -c -I./$(HEADERS_DIR)/
ASMFLAGS = -f elf64 -g -F dwarf


MAIN_DEPS = my_string.h functions.h
FUNCTIONS_DEPS = functions.h
MY_STRING_DEPS = my_string.h

SRC_C = $(wildcard $(SRC_DIR)/*.c)
SRC_ASM = $(wildcard $(SRC_DIR)/*.asm)

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_C)) $(patsubst $(SRC_DIR)/%.asm, $(OBJ_DIR)/%.o, $(SRC_ASM))
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
	$(CC) $(CFLAGS) $< -o $@


$(OBJ_DIR)/%.o : $(SRC_DIR)/%.asm | $(OBJ_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $@
$(BIN_DIR):
	mkdir -p $@

include $(wildcard $(OBJ_DIR)/*.d)

clean : 
	rm $(BIN_DIR)/*.exe $(OBJ_DIR)/*.o
	rm	*.exe
get:
#	echo $(SRC_C)
#	echo $(SRC_ASM)
	echo main_obj == $(MAIN_OBJ)
	echo test_obj == $(TEST_OBJ)