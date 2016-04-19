cmake_minimum_required(VERSION 3.3)
project(SortMergeJoin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp Statistique.cpp Statistique.h SortMergeJoin.cpp SortMergeJoin.h Table.cpp Table.h ParallelSMJ.cpp ParallelSMJ.h SMJUtility.h SMJUtility.cpp Test.h Test.cpp)
add_executable(SortMergeJoin ${SOURCE_FILES})