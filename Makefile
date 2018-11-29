# a simple makefile for perdition build environment
# make with one of these commands
# 		: make 
#		: make <OUTPUT_NAME>
#		: make walk2

# name of project/output executable
OUTPUT_NAME = perdition

# place directories here
SRC_DIR = .
INCLUDE_DIR = ./include
BUILD_DIR = ./build

# compiler flags
CFLAGS = -std=c++11 -Wall -Wextra 
DFLAGS = -g
IFLAGS = -I$(INCLUDE_DIR)
LD_FLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

# place static and dynamic libraries here 
LIB_FILES = ./libggfonts.a \
	/usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/x86_64-linux-gnu/libalut.so \

# this will find all source files in root project directory
SRC_FILES = $(shell find $(SRC_DIR) -name '*.cpp')
# for compilation and linking purposes
OBJ_FAKE_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
OBJ_NODIR_FILES = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC_FILES:.cpp=.o)))

# implicit rule for all object files
$(BUILD_DIR)/%.o: %.cpp
	@echo "Compiling: $<"
	@g++ $(CFLAGS) $(DFLAGS) $(IFLAGS) -c $< -o $(BUILD_DIR)/$(notdir $@)

.PHONY: all start walk2 build clean debug checkDirs
all walk2 $(OUTPUT_NAME): start # default rule
	@echo Successfully built ./$(OUTPUT_NAME)

clean:
	rm -f ./build/*.o
	rm -f ./$(OUTPUT_NAME)
	rm -f ./walk2
	@echo Done

debug: checkDirs
	@echo Source files: $(SRC_FILES)
	@echo Library files: $(LIB_FILES)
	@echo Fake object files: $(OBJ_FAKE_FILES)
	@echo Build object files: $(OBJ_NODIR_FILES)

# checks for existence of defined directories at top
checkDirs:
ifneq ($(SRC_DIR),$(wildcard $(SRC_DIR)))
	@echo Warning: SRC_DIR not found
endif
ifneq ($(INCLUDE_DIR),$(wildcard $(INCLUDE_DIR)))
	@echo Warning: INCLUDE_DIR not found
endif
ifneq ($(BUILD_DIR),$(wildcard $(BUILD_DIR)))
	@echo Creating directory: $(BUILD_DIR)
	@mkdir $(BUILD_DIR)
endif

# prerequisites go here before $(OUTPUT_NAME)
start: checkDirs build

# linker
build: $(OBJ_FAKE_FILES)
	@echo "Linking: $(OUTPUT_NAME)"
	@g++ $(LD_FLAGS) $(OBJ_NODIR_FILES) $(LIB_FILES) -o $(OUTPUT_NAME)