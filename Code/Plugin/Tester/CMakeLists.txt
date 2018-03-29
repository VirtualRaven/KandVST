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
	
	#Do a crude search for the test headers
	file(GLOB TEST_CANDIDATE ${CMAKE_CURRENT_SOURCE_DIR}/Tests/*/*.h)

	#Filter all files not matching Tests/TestName/TestName.h
	list(APPEND TEST_CLASSES "")
	list(APPEND TEST_HEADERS "")
	list(APPEND TEST_NEW "")
	FOREACH(item ${TEST_CANDIDATE})
		STRING(REGEX MATCH ".*\/([^\/\\]*)\/.*\.h" str_match ${item})
		set(tmp ${CMAKE_MATCH_1})
		if(${str_match} MATCHES "${CMAKE_MATCH_1}\.h$")
			add_test(NAME ${tmp} COMMAND tester -T ${tmp})
			list(APPEND TEST_CLASSES ${tmp})
			list(APPEND TEST_HEADERS "${tmp}/${tmp}.h")
			list(APPEND TEST_NEW "new ${tmp}")
			#Also create the test folder when we are at it
			file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Tests/${tmp}/")
		endif()
	ENDFOREACH()
    

	set(TEST_H "${CMAKE_CURRENT_BINARY_DIR}/Tests.h")
	#Generate test file
	file(WRITE  ${TEST_H} "#ifndef TEST_H\n#define TEST_H\n#include <vector>\n" )
	#Write includes
	foreach(item  ${TEST_HEADERS})
		file(APPEND ${TEST_H} "#include \"${item}\"\n")
	ENDFOREACH()
	file(APPEND ${TEST_H} "std::vector<TestHost::Test*> test_list = {\n" )
	string(REPLACE ";" "," TEST_NEW_TMP "${TEST_NEW}")
	file(APPEND ${TEST_H} "${TEST_NEW_TMP}")
	file(APPEND ${TEST_H} "};\n#endif TEST_H")



    #Generate files containing paths to our VST3 dll
    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/folders_debug.h"  
        CONTENT 
			"#define LIB_PATH_D  \"$<TARGET_FILE:KandVST_VST3>\" 
			 #define TEST_PATH_D \"${CMAKE_CURRENT_SOURCE_DIR}/Tests/\" 
			 #define TEST_BUILD_PATH_D \"${CMAKE_BINARY_DIR}/Tests/\"
			 #define PY_PATH_D   \"${PYTHON_EXECUTABLE}\"" 
        CONDITION $<CONFIG:Debug>)

    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/folders_release.h"  
        CONTENT "#define LIB_PATH_R  \"$<TARGET_FILE:KandVST_VST3>\"
				 #define TEST_BUILD_PATH_R \"${CMAKE_BINARY_DIR}/Tests/\"
				 #define TEST_PATH_R \"${CMAKE_CURRENT_SOURCE_DIR}/Tests/\" 
				 #define PY_PATH_R   \"${PYTHON_EXECUTABLE}\""
        CONDITION $<NOT:$<CONFIG:Debug>>)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Tests/)

	if(EXISTS ${PYTHON_EXECUTABLE})
		#All is fine :)
	else()
		add_definitions(-DNO_PY_FOUND)
		if(${ALLOW_PARITAL_TEST})
			message(WARNING "Did not find any python intepretor, skipping tests requiring python")
		else()
			message(FATAL_ERROR " Did not find any python intepretor, but option forces full tests")
		endif()
	endif()

    SET ( SRCS  main.cpp folders.h wrapperVST.cpp util.cpp hostVST.cpp IID_defs.cpp TestHost.cpp)
    add_executable(tester ${SRCS} )
    target_compile_definitions(tester 
        PRIVATE $<$<CONFIG:Debug>:VST_DEBUG> 
                $<$<CONFIG:Release>:VST_RELEASE>)

    add_dependencies(tester KandVST_VST3)

	add_test(NAME TEST_ALL COMMAND tester)

else(TARGET KandVST_VST3)
    message(WARNING "Skipping tester tool as VST3 is not available")
endif(TARGET KandVST_VST3)
