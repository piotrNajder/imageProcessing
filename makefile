EXE = imProc
SRC_DIR = src
OBJ_DIR = bin

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CC=g++
CPPFLAGS += -Iinc
CFLAGS=-g -Wall
LDFLAGS +=
LDLIBS +=

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)