cmake_minimum_required(VERSION 3.22.1)

project(tripleS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} 
	main.cpp
)

add_subdirectory(controlTheory)

find_package(CURL REQUIRED)

include_directories(
	controlTheory
	"${CURL_INCLUDE_DIRS}"
)

target_link_directories(${PROJECT_NAME}
	PRIVATE controlTheory
)

target_link_libraries(${PROJECT_NAME}
	controlTheory
	CURL::libcurl
)