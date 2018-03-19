cmake_minimum_required(VERSION 3.4)
project(SYSTEM_TESTER)

# This is to force the VST api to initialize IID
# we need this as we only use the interface headers
add_definitions(-DINIT_CLASS_IID)

# VST header files
include_directories(${CMAKE_SOURCE_DIR}/../VST/)
include_directories(${CMAKE_SOURCE_DIR}/../VST/pluginterfaces/)

#To include folders_*.h  
include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET ( SRCS  main.cpp folders.h )

#Generate files containing paths to our VST3 dll
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/folders_debug.h"  CONTENT "#define LIB_PATH_D \"$<TARGET_FILE:KandVST_VST3>\"" CONDITION $<CONFIG:Debug>)
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/folders_release.h"  CONTENT "#define LIB_PATH_R \"$<TARGET_FILE:KandVST_VST3>\"" CONDITION $<NOT:$<CONFIG:Debug>>)

add_executable(tester ${SRCS} )
add_dependencies(tester KandVST_VST3)