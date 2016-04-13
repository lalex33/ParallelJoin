cmake_minimum_required(VERSION 3.3)
project(SortMergeJoin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Statistique.cpp Statistique.h SortMergeJoin.cpp SortMergeJoin.h)
add_executable(SortMergeJoin ${SOURCE_FILES})