# Notes
## $@ is Output 
## $^ is the input or dependencies
## % is a wild card which will match the command and run

# Directories
INCLUDE_DIRS = 
LIB_DIRS =  
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
TEST_OBJ_DIR = $(BUILD_DIR)/obj
TEST_BIN_DIR = $(BUILD_DIR)/bin

# Toolchain
CC = g++

# Files
TARGET = $(BIN_DIR)/httpScratchServer
TEST_TARGET = $(TEST_BIN_DIR)/test

SOURCES = src/main.cpp \
		src/TcpServer/TcpServer.cpp \
		src/Router/Router.cpp \
		src/Request/Request.cpp \
		src/Response/Response.cpp \
		src/HTMLTemplate/HTMLTemplate.cpp \
		src/Response/StatusCodes.cpp \
		src/Controller/systemController.cpp \
		src/Utils/Utils.cpp \
		src/Logger/Logger.cpp \
		src/Json/Json.cpp \
		src/Json/JsonNode.cpp \
		app/Router/UserRouter.cpp \
		app/Controller/mainController.cpp

TEST_SOURCES = tests/tests.cpp \
			tests/JsonTests/JsonTests.cpp \
			tests/TrimTests/TrimTests.cpp \
			tests/HTMLTemplateTests/HTMLTemplateTests.cpp \
			src/Utils/Utils.cpp \
			src/Logger/Logger.cpp \
			src/Json/Json.cpp \
			src/Json/JsonNode.cpp \
			src/HTMLTemplate/HTMLTemplate.cpp \

# Creating objects from sources
OBJECT_NAMES = $(SOURCES:.cpp=.o)
OBJECTS = $(patsubst %,$(OBJ_DIR)/%,$(OBJECT_NAMES))

TEST_OBJECT_NAMES = $(TEST_SOURCES:.cpp=.o)
TEST_OBJECTS = $(patsubst %,$(TEST_OBJ_DIR)/%,$(TEST_OBJECT_NAMES))

# Flags
# WFLAGS = -Wall -Wextra -Werror -Wshadow
# TODO: Turn on Development when developing
# CUSTOM_MACROS = -DDEVELOPMENT_ENVIRONMENT
CUSTOM_MACROS_COMMON = -DFREQUENT_RESTART
WFLAGS = 
CFLAGS = $(WFLAGS) $(addprefix -I, $(INCLUDE_DIRS)) -Og -g $(CUSTOM_MACROS) $(CUSTOM_MACROS_COMMON)
LDFLAGS = $(addprefix -L, $(LIB_DIRS))

# Build
## Linking
$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ -o $@

# Compiling
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

# Testing
$(TEST_TARGET): ${TEST_OBJECTS}
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ -o $@

$(TEST_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

# Phonies: Are used so that make doesnot confuse it with a file
.PHONY: all clean test

all: $(TARGET)

test: $(TEST_TARGET)

clean:
	rm -r $(BUILD_DIR)