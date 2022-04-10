cmake_minimum_required(VERSION 3.22.1)

project(tripleS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} 
	src/main.cpp
)

add_subdirectory(src)

file(MAKE_DIRECTORY data/auth)

find_package(CURL REQUIRED)

include_directories(
	src/include
	"${CURL_INCLUDE_DIRS}"
)

target_link_libraries(${PROJECT_NAME}
	controlTheory
	CURL::libcurl
)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURC_DIR}/.git")
	option(BUILD_DEPS "Initialize dependencies while configuring" OFF)
	if(GIT_SUBMODULE)
	execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
	endif()
endif()
