cmake_minimum_required(VERSION 2.8)

project(final)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")


set(LIBS "-pthread")

#aux_source_directory(src SRCS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set(SOURCE_EXE main.cpp)
add_library(settings STATIC settings.cpp)
add_library(server STATIC server.cpp)
add_executable(final ${SOURCE_EXE})
target_link_libraries(final settings server ${LIBS})
