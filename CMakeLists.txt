cmake_minimum_required(VERSION 3.22.1)

project(tripleS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(controlTheory)
add_subdirectory(webScraper)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/cpr/CMakeLists.txt")
    message(FATAL_ERROR "The cpr submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

add_subdirectory(deps/cpr)

target_link_libraries(${PROJECT_NAME} controlTheory webScraper)