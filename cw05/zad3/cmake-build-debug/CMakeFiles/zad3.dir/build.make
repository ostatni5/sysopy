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
CMAKE_SOURCE_DIR = /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/zad3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/zad3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/zad3.dir/flags.make

CMakeFiles/zad3.dir/producent.c.o: CMakeFiles/zad3.dir/flags.make
CMakeFiles/zad3.dir/producent.c.o: ../producent.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/zad3.dir/producent.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zad3.dir/producent.c.o   -c /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/producent.c

CMakeFiles/zad3.dir/producent.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zad3.dir/producent.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/producent.c > CMakeFiles/zad3.dir/producent.c.i

CMakeFiles/zad3.dir/producent.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zad3.dir/producent.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/producent.c -o CMakeFiles/zad3.dir/producent.c.s

CMakeFiles/zad3.dir/producent.c.o.requires:

.PHONY : CMakeFiles/zad3.dir/producent.c.o.requires

CMakeFiles/zad3.dir/producent.c.o.provides: CMakeFiles/zad3.dir/producent.c.o.requires
	$(MAKE) -f CMakeFiles/zad3.dir/build.make CMakeFiles/zad3.dir/producent.c.o.provides.build
.PHONY : CMakeFiles/zad3.dir/producent.c.o.provides

CMakeFiles/zad3.dir/producent.c.o.provides.build: CMakeFiles/zad3.dir/producent.c.o


CMakeFiles/zad3.dir/client.c.o: CMakeFiles/zad3.dir/flags.make
CMakeFiles/zad3.dir/client.c.o: ../client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/zad3.dir/client.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zad3.dir/client.c.o   -c /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/client.c

CMakeFiles/zad3.dir/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zad3.dir/client.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/client.c > CMakeFiles/zad3.dir/client.c.i

CMakeFiles/zad3.dir/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zad3.dir/client.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/client.c -o CMakeFiles/zad3.dir/client.c.s

CMakeFiles/zad3.dir/client.c.o.requires:

.PHONY : CMakeFiles/zad3.dir/client.c.o.requires

CMakeFiles/zad3.dir/client.c.o.provides: CMakeFiles/zad3.dir/client.c.o.requires
	$(MAKE) -f CMakeFiles/zad3.dir/build.make CMakeFiles/zad3.dir/client.c.o.provides.build
.PHONY : CMakeFiles/zad3.dir/client.c.o.provides

CMakeFiles/zad3.dir/client.c.o.provides.build: CMakeFiles/zad3.dir/client.c.o


CMakeFiles/zad3.dir/main.c.o: CMakeFiles/zad3.dir/flags.make
CMakeFiles/zad3.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/zad3.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/zad3.dir/main.c.o   -c /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/main.c

CMakeFiles/zad3.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/zad3.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/main.c > CMakeFiles/zad3.dir/main.c.i

CMakeFiles/zad3.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/zad3.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/main.c -o CMakeFiles/zad3.dir/main.c.s

CMakeFiles/zad3.dir/main.c.o.requires:

.PHONY : CMakeFiles/zad3.dir/main.c.o.requires

CMakeFiles/zad3.dir/main.c.o.provides: CMakeFiles/zad3.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/zad3.dir/build.make CMakeFiles/zad3.dir/main.c.o.provides.build
.PHONY : CMakeFiles/zad3.dir/main.c.o.provides

CMakeFiles/zad3.dir/main.c.o.provides.build: CMakeFiles/zad3.dir/main.c.o


# Object files for target zad3
zad3_OBJECTS = \
"CMakeFiles/zad3.dir/producent.c.o" \
"CMakeFiles/zad3.dir/client.c.o" \
"CMakeFiles/zad3.dir/main.c.o"

# External object files for target zad3
zad3_EXTERNAL_OBJECTS =

zad3: CMakeFiles/zad3.dir/producent.c.o
zad3: CMakeFiles/zad3.dir/client.c.o
zad3: CMakeFiles/zad3.dir/main.c.o
zad3: CMakeFiles/zad3.dir/build.make
zad3: CMakeFiles/zad3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable zad3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zad3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/zad3.dir/build: zad3

.PHONY : CMakeFiles/zad3.dir/build

CMakeFiles/zad3.dir/requires: CMakeFiles/zad3.dir/producent.c.o.requires
CMakeFiles/zad3.dir/requires: CMakeFiles/zad3.dir/client.c.o.requires
CMakeFiles/zad3.dir/requires: CMakeFiles/zad3.dir/main.c.o.requires

.PHONY : CMakeFiles/zad3.dir/requires

CMakeFiles/zad3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/zad3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/zad3.dir/clean

CMakeFiles/zad3.dir/depend:
	cd /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3 /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3 /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/cmake-build-debug /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/cmake-build-debug /mnt/h/STUDIA/SysOPy/KaniaSzymon/cw05/zad3/cmake-build-debug/CMakeFiles/zad3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/zad3.dir/depend

