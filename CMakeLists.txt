cmake_minimum_required(VERSION 3.22.1)

project(tripleS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} 
	main.cpp
)

add_subdirectory(controlTheory)
add_subdirectory(webScraper)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build." ON)
    if(GIT_SUBMODULE)
        message(STATUS "Updating submodules")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT
	)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/cpr/CMakeLists.txt")
    message(FATAL_ERROR "The cpr submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

# add_subdirectory(deps/cpr)

target_include_directories(${PROJECT_NAME} 
	PUBLIC controlTheory
	PUBLIC webScraper
	# PUBLIC deps/cpr
)

target_link_directories(${PROJECT_NAME} 
	PRIVATE controlTheory
	PRIVATE	webScraper
	# PRIVATE cpr::cpr
)

target_link_libraries(${PROJECT_NAME} 
	controlTheory 
	webScraper 
	# cpr::cpr
)