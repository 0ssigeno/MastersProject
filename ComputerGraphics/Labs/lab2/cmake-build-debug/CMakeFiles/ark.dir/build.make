# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /opt/clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ossigeno/University/Consegna/lab2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ossigeno/University/Consegna/lab2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ark.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ark.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ark.dir/flags.make

CMakeFiles/ark.dir/arkanoid.cpp.o: CMakeFiles/ark.dir/flags.make
CMakeFiles/ark.dir/arkanoid.cpp.o: ../arkanoid.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ossigeno/University/Consegna/lab2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ark.dir/arkanoid.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ark.dir/arkanoid.cpp.o -c /home/ossigeno/University/Consegna/lab2/arkanoid.cpp

CMakeFiles/ark.dir/arkanoid.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ark.dir/arkanoid.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ossigeno/University/Consegna/lab2/arkanoid.cpp > CMakeFiles/ark.dir/arkanoid.cpp.i

CMakeFiles/ark.dir/arkanoid.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ark.dir/arkanoid.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ossigeno/University/Consegna/lab2/arkanoid.cpp -o CMakeFiles/ark.dir/arkanoid.cpp.s

# Object files for target ark
ark_OBJECTS = \
"CMakeFiles/ark.dir/arkanoid.cpp.o"

# External object files for target ark
ark_EXTERNAL_OBJECTS =

ark: CMakeFiles/ark.dir/arkanoid.cpp.o
ark: CMakeFiles/ark.dir/build.make
ark: /usr/lib/libOpenGL.so
ark: /usr/lib/libGLX.so
ark: /usr/lib/libGLU.so
ark: /usr/lib/libglut.so
ark: /usr/lib/libXmu.so
ark: /usr/lib/libXi.so
ark: /usr/lib/libGLEW.so
ark: CMakeFiles/ark.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ossigeno/University/Consegna/lab2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ark"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ark.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ark.dir/build: ark

.PHONY : CMakeFiles/ark.dir/build

CMakeFiles/ark.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ark.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ark.dir/clean

CMakeFiles/ark.dir/depend:
	cd /home/ossigeno/University/Consegna/lab2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ossigeno/University/Consegna/lab2 /home/ossigeno/University/Consegna/lab2 /home/ossigeno/University/Consegna/lab2/cmake-build-debug /home/ossigeno/University/Consegna/lab2/cmake-build-debug /home/ossigeno/University/Consegna/lab2/cmake-build-debug/CMakeFiles/ark.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ark.dir/depend

