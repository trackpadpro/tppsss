cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_EXTENSIONS OFF)

project(tppsss)

add_executable(${PROJECT_NAME} 
	src/main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}>
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED YES
)

add_subdirectory(src)

file(MAKE_DIRECTORY data/auth)
file(MAKE_DIRECTORY data/steamData)
file(MAKE_DIRECTORY data/stockData)

find_package(CURL REQUIRED)

include_directories(
	src/include
	${CURL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	CURL::libcurl
	controlTheory
	SCM
	stocks
)

target_link_libraries(SCM PRIVATE
	CURL::libcurl
)

option(ENABLE_DEBUG "Toggle debug console outputs" OFF)
if(ENABLE_DEBUG)
	add_compile_definitions(DEBUG)
	target_compile_definitions(SCM PRIVATE DEBUG)
	target_compile_definitions(stocks PRIVATE DEBUG)
	target_compile_definitions(controlTheory PRIVATE DEBUG)
endif()

option(MULTI_THREAD "Toggle multithreading" ON)
if(MULTI_THREAD)
	if(${CURL_VERSION_STRING} VERSION_LESS 7.84.0)
		message(WARNING "libcurl v${CURL_VERSION_STRING} found; v7.84.0+ recommended for multithreading")
	endif()
	if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
		set(THREADS_PREFER_PTHREAD_FLAG ON)
		find_package(Threads REQUIRED)
		target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
	endif()
	add_compile_definitions(THREAD)
endif()
