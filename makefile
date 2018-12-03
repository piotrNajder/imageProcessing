EXE = imProc
EXE_DIR = bin
SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CC = g++
CPPFLAGS += -Iinc
CFLAGS = -Wall -Wno-unused-result
LDFLAGS +=
LDLIBS +=

# set up extra flags for explicitly setting mode
debug:   CPPFLAGS += -g
release: CPPFLAGS += -O3

all: $(EXE_DIR)/$(EXE)

$(EXE_DIR)/$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

debug:      all
release:    all

clean:
	$(RM) $(OBJ)