# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chengyongtao/VScode/ChatServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chengyongtao/VScode/ChatServer/build

# Include any dependencies generated for this target.
include src/client/CMakeFiles/chatclient.dir/depend.make

# Include the progress variables for this target.
include src/client/CMakeFiles/chatclient.dir/progress.make

# Include the compile flags for this target's objects.
include src/client/CMakeFiles/chatclient.dir/flags.make

src/client/CMakeFiles/chatclient.dir/main.cpp.o: src/client/CMakeFiles/chatclient.dir/flags.make
src/client/CMakeFiles/chatclient.dir/main.cpp.o: ../src/client/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chengyongtao/VScode/ChatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/client/CMakeFiles/chatclient.dir/main.cpp.o"
	cd /home/chengyongtao/VScode/ChatServer/build/src/client && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chatclient.dir/main.cpp.o -c /home/chengyongtao/VScode/ChatServer/src/client/main.cpp

src/client/CMakeFiles/chatclient.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chatclient.dir/main.cpp.i"
	cd /home/chengyongtao/VScode/ChatServer/build/src/client && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chengyongtao/VScode/ChatServer/src/client/main.cpp > CMakeFiles/chatclient.dir/main.cpp.i

src/client/CMakeFiles/chatclient.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chatclient.dir/main.cpp.s"
	cd /home/chengyongtao/VScode/ChatServer/build/src/client && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chengyongtao/VScode/ChatServer/src/client/main.cpp -o CMakeFiles/chatclient.dir/main.cpp.s

# Object files for target chatclient
chatclient_OBJECTS = \
"CMakeFiles/chatclient.dir/main.cpp.o"

# External object files for target chatclient
chatclient_EXTERNAL_OBJECTS =

../bin/chatclient: src/client/CMakeFiles/chatclient.dir/main.cpp.o
../bin/chatclient: src/client/CMakeFiles/chatclient.dir/build.make
../bin/chatclient: src/client/CMakeFiles/chatclient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chengyongtao/VScode/ChatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bin/chatclient"
	cd /home/chengyongtao/VScode/ChatServer/build/src/client && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chatclient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/client/CMakeFiles/chatclient.dir/build: ../bin/chatclient

.PHONY : src/client/CMakeFiles/chatclient.dir/build

src/client/CMakeFiles/chatclient.dir/clean:
	cd /home/chengyongtao/VScode/ChatServer/build/src/client && $(CMAKE_COMMAND) -P CMakeFiles/chatclient.dir/cmake_clean.cmake
.PHONY : src/client/CMakeFiles/chatclient.dir/clean

src/client/CMakeFiles/chatclient.dir/depend:
	cd /home/chengyongtao/VScode/ChatServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chengyongtao/VScode/ChatServer /home/chengyongtao/VScode/ChatServer/src/client /home/chengyongtao/VScode/ChatServer/build /home/chengyongtao/VScode/ChatServer/build/src/client /home/chengyongtao/VScode/ChatServer/build/src/client/CMakeFiles/chatclient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/client/CMakeFiles/chatclient.dir/depend

