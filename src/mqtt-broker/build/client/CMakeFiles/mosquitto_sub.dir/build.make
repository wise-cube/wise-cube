# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/di3go/Documents/wise-cube/src/mqtt/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/di3go/Documents/wise-cube/src/mqtt/build

# Include any dependencies generated for this target.
include client/CMakeFiles/mosquitto_sub.dir/depend.make

# Include the progress variables for this target.
include client/CMakeFiles/mosquitto_sub.dir/progress.make

# Include the compile flags for this target's objects.
include client/CMakeFiles/mosquitto_sub.dir/flags.make

client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o: client/CMakeFiles/mosquitto_sub.dir/flags.make
client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o: /home/di3go/Documents/wise-cube/src/mqtt/src/client/sub_client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/di3go/Documents/wise-cube/src/mqtt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mosquitto_sub.dir/sub_client.c.o   -c /home/di3go/Documents/wise-cube/src/mqtt/src/client/sub_client.c

client/CMakeFiles/mosquitto_sub.dir/sub_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mosquitto_sub.dir/sub_client.c.i"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/di3go/Documents/wise-cube/src/mqtt/src/client/sub_client.c > CMakeFiles/mosquitto_sub.dir/sub_client.c.i

client/CMakeFiles/mosquitto_sub.dir/sub_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mosquitto_sub.dir/sub_client.c.s"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/di3go/Documents/wise-cube/src/mqtt/src/client/sub_client.c -o CMakeFiles/mosquitto_sub.dir/sub_client.c.s

client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o: client/CMakeFiles/mosquitto_sub.dir/flags.make
client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o: /home/di3go/Documents/wise-cube/src/mqtt/src/client/sub_client_output.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/di3go/Documents/wise-cube/src/mqtt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o   -c /home/di3go/Documents/wise-cube/src/mqtt/src/client/sub_client_output.c

client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mosquitto_sub.dir/sub_client_output.c.i"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/di3go/Documents/wise-cube/src/mqtt/src/client/sub_client_output.c > CMakeFiles/mosquitto_sub.dir/sub_client_output.c.i

client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mosquitto_sub.dir/sub_client_output.c.s"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/di3go/Documents/wise-cube/src/mqtt/src/client/sub_client_output.c -o CMakeFiles/mosquitto_sub.dir/sub_client_output.c.s

client/CMakeFiles/mosquitto_sub.dir/client_shared.c.o: client/CMakeFiles/mosquitto_sub.dir/flags.make
client/CMakeFiles/mosquitto_sub.dir/client_shared.c.o: /home/di3go/Documents/wise-cube/src/mqtt/src/client/client_shared.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/di3go/Documents/wise-cube/src/mqtt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object client/CMakeFiles/mosquitto_sub.dir/client_shared.c.o"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mosquitto_sub.dir/client_shared.c.o   -c /home/di3go/Documents/wise-cube/src/mqtt/src/client/client_shared.c

client/CMakeFiles/mosquitto_sub.dir/client_shared.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mosquitto_sub.dir/client_shared.c.i"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/di3go/Documents/wise-cube/src/mqtt/src/client/client_shared.c > CMakeFiles/mosquitto_sub.dir/client_shared.c.i

client/CMakeFiles/mosquitto_sub.dir/client_shared.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mosquitto_sub.dir/client_shared.c.s"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/di3go/Documents/wise-cube/src/mqtt/src/client/client_shared.c -o CMakeFiles/mosquitto_sub.dir/client_shared.c.s

client/CMakeFiles/mosquitto_sub.dir/client_props.c.o: client/CMakeFiles/mosquitto_sub.dir/flags.make
client/CMakeFiles/mosquitto_sub.dir/client_props.c.o: /home/di3go/Documents/wise-cube/src/mqtt/src/client/client_props.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/di3go/Documents/wise-cube/src/mqtt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object client/CMakeFiles/mosquitto_sub.dir/client_props.c.o"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mosquitto_sub.dir/client_props.c.o   -c /home/di3go/Documents/wise-cube/src/mqtt/src/client/client_props.c

client/CMakeFiles/mosquitto_sub.dir/client_props.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mosquitto_sub.dir/client_props.c.i"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/di3go/Documents/wise-cube/src/mqtt/src/client/client_props.c > CMakeFiles/mosquitto_sub.dir/client_props.c.i

client/CMakeFiles/mosquitto_sub.dir/client_props.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mosquitto_sub.dir/client_props.c.s"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/di3go/Documents/wise-cube/src/mqtt/src/client/client_props.c -o CMakeFiles/mosquitto_sub.dir/client_props.c.s

# Object files for target mosquitto_sub
mosquitto_sub_OBJECTS = \
"CMakeFiles/mosquitto_sub.dir/sub_client.c.o" \
"CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o" \
"CMakeFiles/mosquitto_sub.dir/client_shared.c.o" \
"CMakeFiles/mosquitto_sub.dir/client_props.c.o"

# External object files for target mosquitto_sub
mosquitto_sub_EXTERNAL_OBJECTS =

client/mosquitto_sub: client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o
client/mosquitto_sub: client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o
client/mosquitto_sub: client/CMakeFiles/mosquitto_sub.dir/client_shared.c.o
client/mosquitto_sub: client/CMakeFiles/mosquitto_sub.dir/client_props.c.o
client/mosquitto_sub: client/CMakeFiles/mosquitto_sub.dir/build.make
client/mosquitto_sub: lib/libmosquitto.so.1.6.10
client/mosquitto_sub: /usr/lib/libssl.so
client/mosquitto_sub: /usr/lib/libcrypto.so
client/mosquitto_sub: client/CMakeFiles/mosquitto_sub.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/di3go/Documents/wise-cube/src/mqtt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable mosquitto_sub"
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mosquitto_sub.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
client/CMakeFiles/mosquitto_sub.dir/build: client/mosquitto_sub

.PHONY : client/CMakeFiles/mosquitto_sub.dir/build

client/CMakeFiles/mosquitto_sub.dir/clean:
	cd /home/di3go/Documents/wise-cube/src/mqtt/build/client && $(CMAKE_COMMAND) -P CMakeFiles/mosquitto_sub.dir/cmake_clean.cmake
.PHONY : client/CMakeFiles/mosquitto_sub.dir/clean

client/CMakeFiles/mosquitto_sub.dir/depend:
	cd /home/di3go/Documents/wise-cube/src/mqtt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/di3go/Documents/wise-cube/src/mqtt/src /home/di3go/Documents/wise-cube/src/mqtt/src/client /home/di3go/Documents/wise-cube/src/mqtt/build /home/di3go/Documents/wise-cube/src/mqtt/build/client /home/di3go/Documents/wise-cube/src/mqtt/build/client/CMakeFiles/mosquitto_sub.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : client/CMakeFiles/mosquitto_sub.dir/depend
