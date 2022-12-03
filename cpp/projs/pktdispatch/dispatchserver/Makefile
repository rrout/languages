# This is a Makefile for separated multiple sources to build with VSCode on mac
# Thanks, Job Vranish.
# (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
# Reference: Makefile Tutorial
# (https://makefiletutorial.com/)
# Reference: @yagiyuki from Qiita
# (https://qiita.com/yagiyuki/items/ff343d381d9477e89f3b)
# Reference: simonsso from Github
# (https://github.com/simonsso/empty-cpp-project/blob/master/Makefile)
# Reference: Chinese Website blogger CDNS
# (https://blog.csdn.net/qq_22073849/article/details/88893201)

# (1)Compiler
# clang++
CXX = clang++
# (2)Compile options
# -Wall -Wextra -std=c++11 -g
CXX_FLAGS = -Wall -Wextra -std=c++20 -g
# (3)Build task directory path
# I do care about out-of-source builds
# ./build
BUILD_DIR ?= ./build
# (4)Source files directory path
# ./src
SRC_DIRS ?= ./src
# (5)Library files directory path
LIBDIR :=
# (6)Add library files
LIBS := -lpthread -lrt -lzmq -lzmqpp
# (7)Target file, excutable file.
# main
TARGET ?= dispatchServer
# (8)Source files(code), to be compiled
# Find source files we want to compile
# *expression must around by single quotos
# ./src/bank.cpp ./src/main.cpp
SRCS := $(shell find $(SRC_DIRS) -name '*.cc' -or -name '*.c' -or -name '*.s')
# (9)Object files
# String substituion for every C/C++ file
# e.g: ./src/bank.cpp turns into ./build/bank.cpp.o
# ./build/bank.cpp.o  ./build/main.cpp.o
OBJS := $(patsubst %.cc, ${BUILD_DIR}/%.cc.o, $(notdir $(SRCS)))
# (10)Dependency files
# which will generate a .d file next to the .o file. Then to use the .d files,
# you just need to find them all:
#
DEPS := $(OBJS:.o=.d)
# (11)Include files directory path
# Every folder in ./src find include files to be passed via clang
# ./include
INC_DIRS := ./hdr ../hdr
# (12)Include files add together a prefix, clang make sense that -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
# (13)Make Makefiles output Dependency files
# That -MMD and -MP flags together to generate Makefiles
# That generated Makefiles will take .o as .d to the output
# That "-MMD" and "-MP" To generate the dependency files, all you have to do is
# add some flags to the compile command (supported by both Clang and GCC):
CPP_FLAGS ?= $(INC_FLAGS) -MMD -MP
# (14)Link: Generate executable file from object file
# make your target depend on the objects files:
${BUILD_DIR}/${TARGET} : $(OBJS)
	@echo LINKING..... $(CC) -O0 -o $@ $(OBJS) $(LIBS)
	@$(CXX) $(OBJS) $(LIBS) -O0 -o $@
	@echo BUILDING..... ${BUILD_DIR}/${TARGET} Complete.....

# (15)Compile: Generate object files from source files
# $@ := {TARGET}
# $< := THE first file
# $^ all the dependency
# C++ Sources
$(BUILD_DIR)/%.cc.o: $(SRC_DIRS)/%.cc
	$(MKDIR_P) $(dir $@)
	@echo COMPILING... $(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -c "$<" -O0 -o "$@"
	@$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -c $< -O0 -o $@

#(16)Delete dependence files, object files, and the target file
.PHONY: all clean
all: ${BUILD_DIR}/${TARGET}


clean:
	$(RM) $(DEPS) $(OBJS) ${BUILD_DIR}/${TARGET}

-include $(DEPS)

MKDIR_P ?= mkdir -p

