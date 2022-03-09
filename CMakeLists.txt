cmake_minimum_required(VERSION 3.22.1)

project(tripleS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} 
	src/main.cpp
)

add_subdirectory(src)

find_package(CURL REQUIRED)

include_directories(
	src/include
	"${CURL_INCLUDE_DIRS}"
)

target_link_libraries(${PROJECT_NAME}
	controlTheory
	CURL::libcurl
)