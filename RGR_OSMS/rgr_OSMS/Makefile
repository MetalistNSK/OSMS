# Компилятор и флаги
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Iinclude

# Директории и файлы
SRC_DIR := io
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/rgr_program

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
MAIN_FILE := main.cpp
MAIN_OBJ := $(BUILD_DIR)/main.o

# Правило по умолчанию
all: $(TARGET)

# Создание исполняемого файла
$(TARGET): $(OBJ_FILES) $(MAIN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Компиляция объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/*.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: $(MAIN_FILE) $(INCLUDE_DIR)/*.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Пересборка
rebuild: clean all

.PHONY: all clean rebuild