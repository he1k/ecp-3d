###########################################################################
# CMakeLists.txt generated for component mod_coder
# Product type: executable
###########################################################################
cmake_minimum_required(VERSION 3.12)
project(mod_coder)

# Propagate the CMAKE_EXPORT_COMPILE_COMMANDS variable from the 
# environment if it is defined as an environment variable, but not as a 
# CMake variable. This is to work around a bug in CMake 3.19 when the 
# "NMake Makefiles" generator is selected.
if(DEFINED ENV{CMAKE_EXPORT_COMPILE_COMMANDS} AND NOT DEFINED CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS $ENV{CMAKE_EXPORT_COMPILE_COMMANDS})
endif()

# Define common variables that are used within the whole project.
set(SYSLIB_PREFIX $<IF:$<AND:$<BOOL:${WIN32}>,$<NOT:$<BOOL:${MINGW}>>>,lib,>)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

###########################################################################
## Path variables
###########################################################################
# Derive an absolute path to the code generation anchor folder.
get_filename_component(START_DIR .. ABSOLUTE)

# Special directories defined by using CACHE variables can be overridden 
# by setting the variable from the command line, e.g.,
# 
# 	cmake . -DMATLAB_ROOT=/path/to/another/matlab/root
set(MATLAB_ROOT C:/Program\ Files/MATLAB/R2023b CACHE PATH "")

###########################################################################
## System Libraries
###########################################################################
find_library(FOUND_LIBM m NO_SYSTEM_ENVIRONMENT_PATH PATHS ${CMAKE_C_IMPLICIT_LINK_DIRECTORIES} ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})

###########################################################################
## Target definition and commands
###########################################################################

# When a CMakeLists.txt defines multiple targets that have a common set of 
# tool flags and preprocessor definitions, define variables containing 
# data that is common to all targets defined in the file.
set(MOD_CODER_COMMON_DEFINES -DMODEL=mod_coder
    -DNUMST=2
    -DNCSTATES=0
    -DHAVESTDIO
    -DMODEL_HAS_DYNAMICALLY_LOADED_SFCNS=0
    -DCLASSIC_INTERFACE=0
    -DALLOCATIONFCN=0
    -DTID01EQ=1
    -DTERMFCN=1
    -DONESTEPFCN=1
    -DMAT_FILE=0
    -DMULTI_INSTANCE_CODE=0
    -DINTEGER_CODE=0
    -DMT=0)
set(MOD_CODER_COMMON_LINKER_FLAGS -static
    -m64)

# Definition of target "mod_coder_objects".
add_library(mod_coder_objects OBJECT ${START_DIR}/mod_coder_ert_rtw/mod_coder.c)

# Set properties for target "mod_coder_objects".
set_target_properties(mod_coder_objects PROPERTIES PREFIX ""
    POSITION_INDEPENDENT_CODE ON)

# Specify language features required for target "mod_coder_objects".
target_compile_features(mod_coder_objects PUBLIC c_std_99)

# Specify compiler preprocessor definitions for target 
# "mod_coder_objects".
target_compile_definitions(mod_coder_objects PRIVATE ${MOD_CODER_COMMON_DEFINES})

# Specify include directories for target "mod_coder_objects".
target_include_directories(mod_coder_objects PUBLIC $<BUILD_INTERFACE:${START_DIR}>
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>>
    $<BUILD_INTERFACE:${START_DIR}/mod_coder_ert_rtw>
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/mod_coder_ert_rtw>
    $<BUILD_INTERFACE:${MATLAB_ROOT}/extern/include>
    $<BUILD_INTERFACE:${MATLAB_ROOT}/simulink/include>
    $<BUILD_INTERFACE:${MATLAB_ROOT}/rtw/c/src>
    $<BUILD_INTERFACE:${MATLAB_ROOT}/rtw/c/src/ext_mode/common>
    $<BUILD_INTERFACE:${MATLAB_ROOT}/rtw/c/ert>)

# Specify linker flags for target "mod_coder_objects".
target_link_libraries(mod_coder_objects PRIVATE ${MOD_CODER_COMMON_LINKER_FLAGS})

# Specify library link dependencies for target "mod_coder_objects".  CMake 
# generator expressions are used to create a CMakeLists.txt file that 
# supports multiple platforms with differently named system library 
# dependencies.
target_link_libraries(mod_coder_objects PUBLIC $<$<BOOL:${FOUND_LIBM}>:m>)


# Definition of target "mod_coder".
add_executable(mod_coder  ${START_DIR}/mod_coder_ert_rtw/ert_main.c)

# Set properties for target "mod_coder".
set_target_properties(mod_coder PROPERTIES PREFIX ""
    POSITION_INDEPENDENT_CODE ON)

# Specify language features required for target "mod_coder".
target_compile_features(mod_coder PUBLIC c_std_99)

# Specify compiler preprocessor definitions for target "mod_coder".
target_compile_definitions(mod_coder PRIVATE ${MOD_CODER_COMMON_DEFINES})

# Specify linker flags for target "mod_coder".
target_link_libraries(mod_coder PRIVATE ${MOD_CODER_COMMON_LINKER_FLAGS})

# Specify library link dependencies for target "mod_coder".  CMake 
# generator expressions are used to create a CMakeLists.txt file that 
# supports multiple platforms with differently named system library 
# dependencies.
target_link_libraries(mod_coder PRIVATE mod_coder_objects
    $<$<BOOL:${WIN32}>:kernel32>
    $<$<BOOL:${WIN32}>:ws2_32>
    $<$<BOOL:${WIN32}>:mswsock>
    $<$<BOOL:${WIN32}>:advapi32>)

# Extract DWARF debug symbols into a separate file when building 
# executable programs for Apple platforms.
if(APPLE AND (CMAKE_C_COMPILER_ID STREQUAL "AppleClang"))
    string(APPEND _dsymutilcmd "$<IF:$<CONFIG:DEBUG,RELWITHDEBINFO>,"
                "xcrun;dsymutil;$<TARGET_FILE:mod_coder>;--flat,"
                "true"
        >)
    add_custom_command(TARGET mod_coder
            POST_BUILD
            COMMAND "${_dsymutilcmd}"
            COMMAND_EXPAND_LISTS
            BYPRODUCTS mod_coder.dwarf)
endif()


###########################################################################
## Target install rules
###########################################################################

# Install rule for objects of target "mod_coder_objects" to allow support 
# for SIL/PIL.
install(TARGETS mod_coder_objects
    EXPORT mod_coderTargets
    OBJECTS DESTINATION "mod_coder_ert_rtw")

# Install rule for executable target "mod_coder". The "OPTIONAL" keyword 
# is required to support SIL/PIL when build of the final executable is 
# suppressed.
install(TARGETS mod_coder
    RUNTIME DESTINATION "."
    OPTIONAL)

# Install rule for Microsoft PDB debug symbols, if required.
if(CMAKE_C_LINKER_SUPPORTS_PDB)
    install(FILES $<TARGET_PDB_FILE:mod_coder>
        DESTINATION .
        OPTIONAL
        CONFIGURATIONS Debug RelWithDebInfo)
endif()

# Install rule for Apple DWARF debug symbols, if required.
if(APPLE AND (CMAKE_C_COMPILER_ID STREQUAL "AppleClang"))
    install(FILES $<TARGET_FILE_DIR:mod_coder>/mod_coder.dwarf
        DESTINATION .
        OPTIONAL
        CONFIGURATIONS Debug RelWithDebInfo)
endif()

# Write a rule that generates a wrapper around exported targets to 
# preserve tokenization of "special" directories (e.g., MATLAB_ROOT).  
# This avoids hard-coding absolute paths in the CMake file with the code 
# used to import the targets, and avoids errors when include paths that do 
# not exist in the current filesystem are defined, for example, after 
# relocating code using PackNGo.
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/mod_coder.cmake" [=[include("${CMAKE_CURRENT_LIST_DIR}/mod_coderTargets.cmake")]=] \n)
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/mod_coder.cmake" [=[set(MOD_CODER_OBJECTS_TOK_INC_DIRS_ALL]=] \n
    [=[    ${MATLAB_ROOT}/extern/include]=] \n
    [=[    ${MATLAB_ROOT}/simulink/include]=] \n
    [=[    ${MATLAB_ROOT}/rtw/c/src]=] \n
    [=[    ${MATLAB_ROOT}/rtw/c/src/ext_mode/common]=] \n
    [=[    ${MATLAB_ROOT}/rtw/c/ert)]=] \n
    [=[foreach(TOKDIR_LOOP IN LISTS MOD_CODER_OBJECTS_TOK_INC_DIRS_ALL)]=] \n
    [=[    if(IS_DIRECTORY ${TOKDIR_LOOP})]=] \n
    [=[        list(APPEND MOD_CODER_OBJECTS_TOK_INC_DIRS ${TOKDIR_LOOP})]=] \n
    [=[    endif()]=] \n
    [=[endforeach()]=] \n
    [=[target_include_directories(mod_coder::mod_coder_objects INTERFACE ${MOD_CODER_OBJECTS_TOK_INC_DIRS})]=] \n)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mod_coder.cmake"
    DESTINATION "mod_coder_ert_rtw/export")

# Generate and install a file that allows the targets generated from this 
# CMakeLists.txt file to be imported into another project.
install(EXPORT mod_coderTargets NAMESPACE mod_coder:: DESTINATION mod_coder_ert_rtw/export)

###########################################################################
## Build success message
###########################################################################
add_custom_command(TARGET mod_coder POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "\\#\\#\\# Created executable: $<TARGET_FILE:mod_coder>")

###########################################################################
## Call toolchain hook function if defined
###########################################################################
if(COMMAND toolchain_target_hook)
    toolchain_target_hook(mod_coder)
endif()
