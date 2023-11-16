# Notes
## $@ is Output 
## $^ is the input or dependencies
## % is a wild card which will match the command and run

# Directories
INCLUDE_DIRS = /usr/include
LIB_DIRS = /usr/lib/x86_64-linux-gnu

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
		src/Json/Scanner/Scanner.cpp \
		src/Json/Parser/Parser.cpp \
		src/Json/Token/Token.cpp \
		src/Json/Node/Node.cpp \
		src/SQLite/SQLite.cpp \
		app/Router/UserRouter.cpp \
		app/db/models/initializeTables.cpp \
		app/db/services/visitors.cpp \
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
# TODO: Turn on Development when developing
# WFLAGS = -Wall -Wextra -Werror -Wshadow
CUSTOM_MACROS = -DDEVELOPMENT_ENVIRONMENT
# OPTIMATION_C_FLAGS = -Og -g # Development
OPTIMATION_C_FLAGS = -O3 # Production

CUSTOM_MACROS_COMMON = -DFREQUENT_RESTART
CFLAGS = $(WFLAGS) $(addprefix -I, $(INCLUDE_DIRS)) $(OPTIMATION_C_FLAGS) $(CUSTOM_MACROS) $(CUSTOM_MACROS_COMMON)
LDFLAGS = $(addprefix -L, $(LIB_DIRS))

# Build
## Linking
$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ -lsqlite3 -o $@

# Compiling
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

# Testing
$(TEST_TARGET): ${TEST_OBJECTS}
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ -lsqlite3 -o $@

$(TEST_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

# Phonies: Are used so that make doesnot confuse it with a file
.PHONY: all clean test

all: $(TARGET)

test: $(TEST_TARGET)

clean:
	rm -r $(BUILD_DIR) || true