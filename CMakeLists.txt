cmake_minimum_required(VERSION 3.22.1)

project(tppsss)

add_executable(${PROJECT_NAME} 
	src/main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}>
)

add_subdirectory(src)

file(MAKE_DIRECTORY data/auth)

find_package(CURL REQUIRED)

include_directories(
	src/include
	${CURL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
	controlTheory
	CURL::libcurl
)

option(BUILD_DEPS "Initialize dependencies while configuring" OFF)
if(BUILD_DEPS)
	if(EXISTS ${CMAKE_SOURCE_DIR}/.git)
		execute_process(COMMAND git submodule update --init --recursive
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		)
		if(EXISTS ${CMAKE_SOURCE_DIR}/deps/expect/make.cmd)
			execute_process(COMMAND ${CMAKE_SOURCE_DIR}/deps/expect/make.cmd
					WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps/expect
					RESULT_VARIABLE EXPECT_MAKE_RESULT
			)
			if(EXPECT_MAKE_RESULT EQUAL "0")
				message(STATUS "Expect.lua executable created in ${CMAKE_SOURCE_DIR}/deps/expect/")
			else()
				message(WARNING "Expect.lua executable creation failed")
			endif()
		else()
			message(WARNING "Expect.lua files corrupted")
		endif()
	else()
		message(WARNING "Dependency configuration failed")
	endif()
endif()
