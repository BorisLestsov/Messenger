cmake_minimum_required(VERSION 2.8)
project(Messenger)

set(SOURCE_DIR "source")
set(CLIENT_SOURCE_DIR "${SOURCE_DIR}/client")
set(SERVER_SOURCE_DIR "${SOURCE_DIR}/server")
set(LIB_SOURCE_DIR "${SOURCE_DIR}/lib")
set(INCLUDE_DIR "include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package(Boost COMPONENTS system REQUIRED)

file(GLOB CLIENT_SOURCE_FILES
        "${CLIENT_SOURCE_DIR}/*.cpp"
        "${INCLUDE_DIR}/*.h"
        "${INCLUDE_DIR}/*.hpp"
        )
file(GLOB SERVER_SOURCE_FILES
        "${SERVER_SOURCE_DIR}/*.cpp"
        "${INCLUDE_DIR}/*.h"
        "${INCLUDE_DIR}/*.hpp"
        )
file(GLOB LIB_SOURCE_FILES
        "${LIB_SOURCE_DIR}/*.cpp"
        "${INCLUDE_DIR}/*.h"
        "${INCLUDE_DIR}/*.hpp"
        )

include_directories(${INCLUDE_DIR})



add_library(Lib STATIC ${LIB_SOURCE_FILES})
add_executable(Main ${SOURCE_DIR}/main.cpp)
add_executable(Client ${CLIENT_SOURCE_FILES})
add_executable(Server ${SERVER_SOURCE_FILES})

set_target_properties( Server Client Main
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
        )

add_custom_target(_clean COMMAND rm -f CMakeCache.txt cmake_install.cmake
						COMMAND	rm -rf CMakeFiles/*
						COMMAND	rmdir CMakeFiles
						COMMAND rm -f Makefile
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

target_link_libraries(Client ${Boost_LIBRARIES} Lib pthread boost_thread)
target_link_libraries(Server ${Boost_LIBRARIES} Lib pthread boost_thread)
target_link_libraries(Main ${Boost_LIBRARIES} Lib pthread boost_thread)