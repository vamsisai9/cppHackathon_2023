# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\TCC\Tools\cmake\3.26.4_WIN64\bin\cmake.exe

# The command to remove a file.
RM = C:\TCC\Tools\cmake\3.26.4_WIN64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\cpp_hackathon

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\cpp_hackathon\build\GCC\Debug

# Utility rule file for ContinuousMemCheck.

# Include any custom commands dependencies for this target.
include yaml-cpp/CMakeFiles/ContinuousMemCheck.dir/compiler_depend.make

# Include the progress variables for this target.
include yaml-cpp/CMakeFiles/ContinuousMemCheck.dir/progress.make

yaml-cpp/CMakeFiles/ContinuousMemCheck:
	cd /d C:\cpp_hackathon\build\GCC\Debug\yaml-cpp && C:\TCC\Tools\cmake\3.26.4_WIN64\bin\ctest.exe -D ContinuousMemCheck

ContinuousMemCheck: yaml-cpp/CMakeFiles/ContinuousMemCheck
ContinuousMemCheck: yaml-cpp/CMakeFiles/ContinuousMemCheck.dir/build.make
.PHONY : ContinuousMemCheck

# Rule to build all files generated by this target.
yaml-cpp/CMakeFiles/ContinuousMemCheck.dir/build: ContinuousMemCheck
.PHONY : yaml-cpp/CMakeFiles/ContinuousMemCheck.dir/build

yaml-cpp/CMakeFiles/ContinuousMemCheck.dir/clean:
	cd /d C:\cpp_hackathon\build\GCC\Debug\yaml-cpp && $(CMAKE_COMMAND) -P CMakeFiles\ContinuousMemCheck.dir\cmake_clean.cmake
.PHONY : yaml-cpp/CMakeFiles/ContinuousMemCheck.dir/clean

yaml-cpp/CMakeFiles/ContinuousMemCheck.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\cpp_hackathon C:\cpp_hackathon\yaml-cpp C:\cpp_hackathon\build\GCC\Debug C:\cpp_hackathon\build\GCC\Debug\yaml-cpp C:\cpp_hackathon\build\GCC\Debug\yaml-cpp\CMakeFiles\ContinuousMemCheck.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : yaml-cpp/CMakeFiles/ContinuousMemCheck.dir/depend

