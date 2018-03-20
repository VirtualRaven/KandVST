cmake_minimum_required(VERSION 3.4)

if(TARGET KandVST_VST3)
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
    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/folders_debug.h"  
        CONTENT "#define LIB_PATH_D \"$<TARGET_FILE:KandVST_VST3>\"" 
        CONDITION $<CONFIG:Debug>)

    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/folders_release.h"  
        CONTENT "#define LIB_PATH_R \"$<TARGET_FILE:KandVST_VST3>\"" 
        CONDITION $<NOT:$<CONFIG:Debug>>)
    
    add_executable(tester ${SRCS} )
    target_compile_definitions(tester 
        PRIVATE $<$<CONFIG:Debug>:VST_DEBUG> 
                $<$<CONFIG:Release>:VST_RELEASE>)

    add_dependencies(tester KandVST_VST3)
else(TARGET KandVST_VST3)
    message(WARNING "Skipping tester tool as VST3 is not available")
endif(TARGET KandVST_VST3)
