CXX := g++
AR := ar
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude

SRC_DIR := src
OBJ_DIR := build
LIB := libcalc.a
APP := app

LIB_SRC := $(SRC_DIR)/calc.cpp
APP_SRC := $(SRC_DIR)/main.cpp

LIB_OBJ := $(OBJ_DIR)/calc.o
APP_OBJ := $(OBJ_DIR)/main.o

.PHONY: all clean
all: $(APP)

$(APP): $(LIB) $(APP_OBJ)
	$(CXX) $(CXXFLAGS) $(APP_OBJ) -L. -lcalc -o $@

$(LIB): $(LIB_OBJ)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(APP) $(LIB)
