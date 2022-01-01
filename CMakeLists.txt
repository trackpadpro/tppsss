cmake_minimum_required(VERSION 3.22.1)

project(tripleS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} 
	main.cpp
)

add_subdirectory(controlTheory)

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

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/curl/CMakeLists.txt")
    message(FATAL_ERROR "GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

set(MBEDTLS_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/deps/mbedtls/build/include/")
set(MBEDTLS_LIBRARY "${CMAKE_SOURCE_DIR}/deps/mbedtls/build/library/libmbedtls.a")
set(MBEDX509_LIBRARY "${CMAKE_SOURCE_DIR}/deps/mbedtls/build/library/libmbedx509.a")
set(MBEDCRYPTO_LIBRARY "${CMAKE_SOURCE_DIR}/deps/mbedtls/build/library/libmbedcrypto.a")

add_subdirectory(deps/curl)

include_directories(
	controlTheory
	${CURL_INCLUDE_DIR}
)

target_link_directories(${PROJECT_NAME}
	PRIVATE controlTheory
)

target_link_libraries(${PROJECT_NAME} 
	controlTheory
	${CURL_LIBRARIES}
)