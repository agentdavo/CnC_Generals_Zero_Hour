# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/two_stage_render_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/two_stage_render_test.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/two_stage_render_test.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/two_stage_render_test.dir/flags.make

tests/CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.o: tests/CMakeFiles/two_stage_render_test.dir/flags.make
tests/CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.o: /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/tests/two_stage_render_test.c
tests/CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.o: tests/CMakeFiles/two_stage_render_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.o"
	cd /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.o -MF CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.o.d -o CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.o -c /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/tests/two_stage_render_test.c

tests/CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.i"
	cd /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/tests/two_stage_render_test.c > CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.i

tests/CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.s"
	cd /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/tests/two_stage_render_test.c -o CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.s

# Object files for target two_stage_render_test
two_stage_render_test_OBJECTS = \
"CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.o"

# External object files for target two_stage_render_test
two_stage_render_test_EXTERNAL_OBJECTS =

tests/two_stage_render_test: tests/CMakeFiles/two_stage_render_test.dir/two_stage_render_test.c.o
tests/two_stage_render_test: tests/CMakeFiles/two_stage_render_test.dir/build.make
tests/two_stage_render_test: libd3d8_to_gles.a
tests/two_stage_render_test: /usr/lib/x86_64-linux-gnu/libGLESv1_CM.so
tests/two_stage_render_test: /usr/lib/x86_64-linux-gnu/libEGL.so
tests/two_stage_render_test: tests/CMakeFiles/two_stage_render_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable two_stage_render_test"
	cd /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/two_stage_render_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/two_stage_render_test.dir/build: tests/two_stage_render_test
.PHONY : tests/CMakeFiles/two_stage_render_test.dir/build

tests/CMakeFiles/two_stage_render_test.dir/clean:
	cd /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/two_stage_render_test.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/two_stage_render_test.dir/clean

tests/CMakeFiles/two_stage_render_test.dir/depend:
	cd /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/tests /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build/tests /home/davidsmith/CnC_Generals_Z/lib/d3d8_gles/build/tests/CMakeFiles/two_stage_render_test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tests/CMakeFiles/two_stage_render_test.dir/depend

