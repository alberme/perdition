# a simple makefile for perdition build environment
# make with one of these comamands
# 		: make 
#		: make perdition
#		: make walk2

PROJECT_NAME = perdition

# define directories here
SRC_DIR = .
LIB_DIR = .
INCLUDE_DIR = ./include
BUILD_DIR = ./build

# compiler flags
CFLAGS = --std=c++11 -Wall -Wextra 
DFLAGS = -g
IFLAGS = -I$(INCLUDE_DIR)
LD_FLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

# make will find all files required for building here
SRC_FILES = $(shell find $(SRC_DIR) -name '*.cpp')
LIB_FILES = $(shell find $(LIB_DIR) -name '*.a')
OBJ_FAKE_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
OBJ_NODIR_FILES = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC_FILES:.cpp=.o)))

# implicit rule for all object files
$(BUILD_DIR)/%.o: %.cpp
	@echo "Compiling: $<"
	@g++ $(CFLAGS) $(IFLAGS) -c $< -o $(BUILD_DIR)/$(notdir $@)

.PHONY: all start walk2 clean debug checkDirs
all lol walk2: start # default rule
	@echo Successfully built ./$(PROJECT_NAME)

clean:
	rm -f ./build/*.o
	rm -f ./$(PROJECT_NAME)
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
ifneq ($(LIB_DIR),$(wildcard $(LIB_DIR)))
	@echo Warning: LIB_DIR not found
endif
ifneq ($(INCLUDE_DIR),$(wildcard $(INCLUDE_DIR)))
	@echo Warning: INCLUDE_DIR not found
endif
ifneq ($(BUILD_DIR),$(wildcard $(BUILD_DIR)))
	@echo Creating directory: $(BUILD_DIR)
	@mkdir $(BUILD_DIR)
endif

# prerequisites go here before $(PROJECT_NAME)
start: checkDirs $(PROJECT_NAME)

# linker
$(PROJECT_NAME): $(OBJ_FAKE_FILES)
	@echo "Linking: $@"
	@g++ $(LD_FLAGS) $(OBJ_NODIR_FILES) $(LIB_FILES) -o $@