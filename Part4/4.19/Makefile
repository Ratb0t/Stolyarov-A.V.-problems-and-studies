# Исполняемые файлы
Executable := main.exe
Executable_test := test.exe

# Пути
UTILS_PATH := $(shell pwd | sed 's|/Stolyarov-A.V.-problems-and-studies/.*|/Stolyarov-A.V.-problems-and-studies|')
UTILS_HEADERS_DIR := $(UTILS_PATH)/Utilities/Headers
UTILS_SRC_DIR := $(UTILS_PATH)/Utilities/Src
OBJ_DIR := Build/objects
BIN_DIR := Build/bin

# Компилятор и флаги
CC := gcc
HEADERS_CFLAGS := -I$(UTILS_HEADERS_DIR)
CFLAGS := -g -Wall -MMD -MP -c $(HEADERS_CFLAGS)
DEFINITIONS :=

# 1. Находим все исходники в текущей директории
LOCAL_SRCS := $(wildcard *.c)
LOCAL_OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(LOCAL_SRCS))

# 2. Находим main.c
MAIN_SRC := main.c

# 3. Определяем, какие утилиты нужны (УПРОЩЕННАЯ ВЕРСИЯ)
# Сначала получаем все заголовки, на которые есть инклуды в main.c
MAIN_INCLUDES_RAW := $(shell grep -h '^#include "' $(MAIN_SRC) 2>/dev/null)
MAIN_INCLUDES := $(shell echo "$(MAIN_INCLUDES_RAW)" | sed 's/#include "\([^"]*\)"/\1/')

# Функция для поиска исходного файла утилиты
define find_util_source
$(if $(wildcard $(UTILS_HEADERS_DIR)/$(1)),\
  $(if $(wildcard $(UTILS_SRC_DIR)/$(basename $(1)).c),\
    $(UTILS_SRC_DIR)/$(basename $(1)).c,\
    $(if $(filter my_%,$(basename $(1))),\
      $(if $(wildcard $(UTILS_SRC_DIR)/c$(lastword $(subst _, ,$(basename $(1)))).c),\
        $(UTILS_SRC_DIR)/c$(lastword $(subst _, ,$(basename $(1)))).c))))
endef

# Находим нужные исходники утилит
NEEDED_UTILS := $(strip $(foreach inc,$(MAIN_INCLUDES),$(call find_util_source,$(inc))))

# 4. Формируем список объектных файлов для нужных утилит
UTILS_OBJS := $(patsubst $(UTILS_SRC_DIR)/%.c,$(OBJ_DIR)/util_%.o,$(NEEDED_UTILS))

# 5. Все объектные файлы
ALL_OBJS := $(LOCAL_OBJS) $(UTILS_OBJS)

# Основные цели
all: $(BIN_DIR)/$(Executable) run.exe

run.exe: $(BIN_DIR)/$(Executable)
	ln -sf $(BIN_DIR)/$(Executable) run.exe

# Сборка исполняемого файла
$(BIN_DIR)/$(Executable): $(ALL_OBJS) | $(OBJ_DIR) $(BIN_DIR)
	@echo "Собираем исполняемый файл..."
	@echo "Используемые утилиты: $(notdir $(NEEDED_UTILS))"
	$(CC) $(ALL_OBJS) -o $@

# Правило для локальных .c файлов (включая main.c)
$(OBJ_DIR)/%.o: ./%.c | $(OBJ_DIR)
	@echo "Компилируем $<..."
	$(CC) $(DEFINITIONS) $(CFLAGS) $< -o $@

# Правило для утилит (только нужных)
$(OBJ_DIR)/util_%.o: $(UTILS_SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Компилируем утилиту $<..."
	$(CC) $(DEFINITIONS) $(CFLAGS) $< -o $@

# Создание директорий
$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

# Включение зависимостей
-include $(wildcard $(OBJ_DIR)/*.d)

# Очистка
clean:
	-rm -f $(OBJ_DIR)/*.d 
	-rm -f $(OBJ_DIR)/*.o
	-rm -f $(BIN_DIR)/*.exe 
	-rm -f *.exe

# Отладочная информация
get:
	@echo "====================================================================="
	@echo "UTILS_PATH == $(UTILS_PATH)"
	@echo "====================================================================="
	@echo "UTILS_HEADERS_DIR == $(UTILS_HEADERS_DIR)"
	@echo "====================================================================="
	@echo "UTILS_SRC_DIR == $(UTILS_SRC_DIR)"
	@echo "====================================================================="
	@echo "LOCAL_SRCS == $(LOCAL_SRCS)"
	@echo "====================================================================="
	@echo "MAIN_SRC == $(MAIN_SRC)"
	@echo "====================================================================="
	@echo "MAIN_INCLUDES_RAW == $(MAIN_INCLUDES_RAW)"
	@echo "====================================================================="
	@echo "MAIN_INCLUDES == $(MAIN_INCLUDES)"
	@echo "====================================================================="
	@echo "NEEDED_UTILS == $(NEEDED_UTILS)"
	@echo "====================================================================="
	@echo "UTILS_OBJS == $(UTILS_OBJS)"
	@echo "====================================================================="
	@echo "ALL_OBJS == $(ALL_OBJS)"
	@echo "====================================================================="

# Пересборка
rebuild: clean all

.PHONY: all clean rebuild get run.exe