# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/177/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/177/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/berkan/betriebssysteme/Blatt_5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/berkan/betriebssysteme/Blatt_5/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Blatt_5.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Blatt_5.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Blatt_5.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Blatt_5.dir/flags.make

CMakeFiles/Blatt_5.dir/main.cpp.o: CMakeFiles/Blatt_5.dir/flags.make
CMakeFiles/Blatt_5.dir/main.cpp.o: ../main.cpp
CMakeFiles/Blatt_5.dir/main.cpp.o: CMakeFiles/Blatt_5.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/berkan/betriebssysteme/Blatt_5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Blatt_5.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Blatt_5.dir/main.cpp.o -MF CMakeFiles/Blatt_5.dir/main.cpp.o.d -o CMakeFiles/Blatt_5.dir/main.cpp.o -c /home/berkan/betriebssysteme/Blatt_5/main.cpp

CMakeFiles/Blatt_5.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Blatt_5.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/berkan/betriebssysteme/Blatt_5/main.cpp > CMakeFiles/Blatt_5.dir/main.cpp.i

CMakeFiles/Blatt_5.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Blatt_5.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/berkan/betriebssysteme/Blatt_5/main.cpp -o CMakeFiles/Blatt_5.dir/main.cpp.s

CMakeFiles/Blatt_5.dir/MiniShell.cpp.o: CMakeFiles/Blatt_5.dir/flags.make
CMakeFiles/Blatt_5.dir/MiniShell.cpp.o: ../MiniShell.cpp
CMakeFiles/Blatt_5.dir/MiniShell.cpp.o: CMakeFiles/Blatt_5.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/berkan/betriebssysteme/Blatt_5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Blatt_5.dir/MiniShell.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Blatt_5.dir/MiniShell.cpp.o -MF CMakeFiles/Blatt_5.dir/MiniShell.cpp.o.d -o CMakeFiles/Blatt_5.dir/MiniShell.cpp.o -c /home/berkan/betriebssysteme/Blatt_5/MiniShell.cpp

CMakeFiles/Blatt_5.dir/MiniShell.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Blatt_5.dir/MiniShell.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/berkan/betriebssysteme/Blatt_5/MiniShell.cpp > CMakeFiles/Blatt_5.dir/MiniShell.cpp.i

CMakeFiles/Blatt_5.dir/MiniShell.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Blatt_5.dir/MiniShell.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/berkan/betriebssysteme/Blatt_5/MiniShell.cpp -o CMakeFiles/Blatt_5.dir/MiniShell.cpp.s

# Object files for target Blatt_5
Blatt_5_OBJECTS = \
"CMakeFiles/Blatt_5.dir/main.cpp.o" \
"CMakeFiles/Blatt_5.dir/MiniShell.cpp.o"

# External object files for target Blatt_5
Blatt_5_EXTERNAL_OBJECTS =

Blatt_5: CMakeFiles/Blatt_5.dir/main.cpp.o
Blatt_5: CMakeFiles/Blatt_5.dir/MiniShell.cpp.o
Blatt_5: CMakeFiles/Blatt_5.dir/build.make
Blatt_5: CMakeFiles/Blatt_5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/berkan/betriebssysteme/Blatt_5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Blatt_5"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Blatt_5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Blatt_5.dir/build: Blatt_5
.PHONY : CMakeFiles/Blatt_5.dir/build

CMakeFiles/Blatt_5.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Blatt_5.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Blatt_5.dir/clean

CMakeFiles/Blatt_5.dir/depend:
	cd /home/berkan/betriebssysteme/Blatt_5/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/berkan/betriebssysteme/Blatt_5 /home/berkan/betriebssysteme/Blatt_5 /home/berkan/betriebssysteme/Blatt_5/cmake-build-debug /home/berkan/betriebssysteme/Blatt_5/cmake-build-debug /home/berkan/betriebssysteme/Blatt_5/cmake-build-debug/CMakeFiles/Blatt_5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Blatt_5.dir/depend

