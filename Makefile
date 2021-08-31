OBJ = obj
SRC = src
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)
BIN = bin
TARGET = $(BIN)/bf
TEST = tests/run.sh

CPP = clang++
CPP_COMMON = -MMD -std=c++20 -pedantic -Wall -Wextra -Wshadow -Wunused
CPP_DEBUG = -g -fsanitize=undefined
CPP_RELEASE = -DNDEBUG -Ofast


debug: $(TARGET)
-include $(DEPS)

CPPFLAGS = $(CPP_COMMON) $(CPP_DEBUG)
LDFLAGS = -fsanitize=undefined
release: CPPFLAGS = $(CPP_COMMON) $(CPP_RELEASE)
release: LDFLAGS =
release: clean
release: $(TARGET)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CPP) $(OBJS) -o $@ $(LDFLAGS)

test: $(TARGET)
	$(TEST) $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET) $(DEPS) $(OBJS)