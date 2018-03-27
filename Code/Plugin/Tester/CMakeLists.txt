cmake_minimum_required(VERSION 3.4)

if(TARGET KandVST_VST3)
    
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    # VST header files
    include_directories(${CMAKE_SOURCE_DIR}/../VST/)
    include_directories(${CMAKE_SOURCE_DIR}/../VST/pluginterfaces/)
    
    #To include folders_*.h  
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    
	#Tests
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Tests/)

    SET ( SRCS  main.cpp folders.h wrapperVST.cpp util.cpp hostVST.cpp IID_defs.cpp TestHost.cpp)
    
    #Generate files containing paths to our VST3 dll
    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/folders_debug.h"  
        CONTENT 
			"#define LIB_PATH_D \"$<TARGET_FILE:KandVST_VST3>\" 
			 #define TEST_PATH_D \"${CMAKE_CURRENT_SOURCE_DIR}/Tests/\" " 
        CONDITION $<CONFIG:Debug>)

    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/folders_release.h"  
        CONTENT "#define LIB_PATH_R \"$<TARGET_FILE:KandVST_VST3>\"
				 #define TEST_PATH_R \"${CMAKE_CURRENT_SOURCE_DIR}/Tests/\" "
        CONDITION $<NOT:$<CONFIG:Debug>>)
    
    add_executable(tester ${SRCS} )
    target_compile_definitions(tester 
        PRIVATE $<$<CONFIG:Debug>:VST_DEBUG> 
                $<$<CONFIG:Release>:VST_RELEASE>)

    add_dependencies(tester KandVST_VST3)
else(TARGET KandVST_VST3)
    message(WARNING "Skipping tester tool as VST3 is not available")
endif(TARGET KandVST_VST3)
