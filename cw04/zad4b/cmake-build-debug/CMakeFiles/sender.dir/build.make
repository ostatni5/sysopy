# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/sender.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sender.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sender.dir/flags.make

CMakeFiles/sender.dir/sender.c.o: CMakeFiles/sender.dir/flags.make
CMakeFiles/sender.dir/sender.c.o: ../sender.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sender.dir/sender.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sender.dir/sender.c.o   -c /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b/sender.c

CMakeFiles/sender.dir/sender.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sender.dir/sender.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b/sender.c > CMakeFiles/sender.dir/sender.c.i

CMakeFiles/sender.dir/sender.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sender.dir/sender.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b/sender.c -o CMakeFiles/sender.dir/sender.c.s

CMakeFiles/sender.dir/sender.c.o.requires:

.PHONY : CMakeFiles/sender.dir/sender.c.o.requires

CMakeFiles/sender.dir/sender.c.o.provides: CMakeFiles/sender.dir/sender.c.o.requires
	$(MAKE) -f CMakeFiles/sender.dir/build.make CMakeFiles/sender.dir/sender.c.o.provides.build
.PHONY : CMakeFiles/sender.dir/sender.c.o.provides

CMakeFiles/sender.dir/sender.c.o.provides.build: CMakeFiles/sender.dir/sender.c.o


# Object files for target sender
sender_OBJECTS = \
"CMakeFiles/sender.dir/sender.c.o"

# External object files for target sender
sender_EXTERNAL_OBJECTS =

sender: CMakeFiles/sender.dir/sender.c.o
sender: CMakeFiles/sender.dir/build.make
sender: CMakeFiles/sender.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable sender"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sender.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sender.dir/build: sender

.PHONY : CMakeFiles/sender.dir/build

CMakeFiles/sender.dir/requires: CMakeFiles/sender.dir/sender.c.o.requires

.PHONY : CMakeFiles/sender.dir/requires

CMakeFiles/sender.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sender.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sender.dir/clean

CMakeFiles/sender.dir/depend:
	cd /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b/cmake-build-debug /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b/cmake-build-debug /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw04/zad4b/cmake-build-debug/CMakeFiles/sender.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sender.dir/depend

