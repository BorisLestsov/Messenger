cmake_minimum_required(VERSION 2.5)
project(Messenger)

set(SOURCE_DIR "source")
set(INCLUDE_DIR "include")
set(BIN_DIR "bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

file(GLOB SOURCE_FILES
        "${SOURCE_DIR}/*.cpp"
        "${INCLUDE_DIR}/*.h"
        "${INCLUDE_DIR}/*.hpp"
        )

include_directories(${INCLUDE_DIR})

add_executable(Messenger ${SOURCE_FILES})

set_target_properties( Messenger
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )