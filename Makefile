NAME = galaxy-fort

#Local Folders
SOURCE_DIR = source
HEADER_DIR = include
OBJECT_DIR = object

#Find all the sources (recursively)
CPP_PATHS = $(wildcard $(SOURCE_DIR)/*.cpp) $(wildcard $(SOURCE_DIR)/**/*.cpp)
CPP_FILES = $(CPP_PATHS:source/%=%)

OBJECTS = $(filter-out main.o,$(CPP_FILES:.cpp=.o))

#Globals
CXX = g++
CFLAGS = -std=gnu++11
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	LFLAGS = -I$(HEADER_DIR) -I/usr/include/SDL2 `pkg-config --libs sdl2 gl glew`
else
	LFLAGS = -Ilibraries/SDL2/include -Llibraries/SDL2/lib/x64 -lSDL2 -lGL
endif

#Debug Varibales
DEBUG_TARGET = $(NAME)_debug
DEBUG_CFLAGS = $(CFLAGS) -g
DEBUG_LFLAGS = $(LFLAGS)
DEBUG_OBJECT_DIR = $(OBJECT_DIR)/debug
DEBUG_OBJECTS = $(addprefix $(DEBUG_OBJECT_DIR)/, $(OBJECTS))

#Release Variables
RELEASE_TARGET = $(NAME)
RELEASE_CFLAGS = $(CFLAGS) -O3
RELEASE_LFLAGS = $(LFLAGS)
RELEASE_OBJECT_DIR = $(OBJECT_DIR)/release
RELEASE_OBJECTS = $(addprefix $(RELEASE_OBJECT_DIR)/, $(OBJECTS))

default: debug

clean: clean_debug clean_release
	@rm -rf $(OBJECT_DIR)/*
	@rm -rf $(OBJECT_DIR)

clean_debug:
	@rm -rf $(DEBUG_OBJECT_DIR)/*
	@rm -rf $(DEBUG_OBJECT_DIR)

clean_release:
	@rm -rf $(RELEASE_OBJECT_DIR)/*
	@rm -rf $(RELEASE_OBJECT_DIR)

debug: pre_debug $(DEBUG_OBJECTS) $(SOURCE_DIR)/main.cpp
	@echo 'Compiling debug build...'
	@$(CXX) $(DEBUG_CFLAGS) $(DEBUG_OBJECTS) $(SOURCE_DIR)/main.cpp -o $(DEBUG_TARGET) $(DEBUG_LFLAGS)

release: pre_release $(RELEASE_OBJECTS) $(SOURCE_DIR)/main.cpp
	@echo 'Compiling release build...'
	@$(CXX) $(RELEASE_CFLAGS) $(RELEASE_OBJECTS) $(SOURCE_DIR)/main.cpp -o $(RELEASE_TARGET) $(RELEASE_LFLAGS)

pre_debug: pre_pre
	@[ -d $(DEBUG_OBJECT_DIR) ] || mkdir $(DEBUG_OBJECT_DIR)
	@[ "$(ls -A temp)" ] && cp -r temp/* $(DEBUG_OBJECT_DIR) || :

pre_release: pre_pre
	@[ -d $(RELEASE_OBJECT_DIR) ] || mkdir $(RELEASE_OBJECT_DIR)
	@[ "$(ls -A temp)" ] && cp -r temp/* $(DEBUG_OBJECT_DIR) || :

%-test: test/%.cpp pre_debug $(DEBUG_OBJECTS)
	@echo 'Compiling test build...'
	@$(CXX) $(DEBUG_CFLAGS) $(DEBUG_OBJECTS) $< -o $@ $(DEBUG_LFLAGS)

pre_pre:
	@[ -d $(OBJECT_DIR) ] || mkdir $(OBJECT_DIR)
	@cd $(SOURCE_DIR)
	@find -type d -links 2 -exec mkdir -p "../$(OBJECT_DIR)" \;

$(DEBUG_OBJECT_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	@echo 'Compiling '$@'...'
	@$(CXX) $(DEBUG_CFLAGS) -c $< -o $@ $(DEBUG_LFLAGS)

$(RELEASE_OBJECT_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	@echo 'Compiling '$@'...'
	@$(CXX) $(RELEASE_CFLAGS) -c $< -o $@ $(RELEASE_LFLAGS)
