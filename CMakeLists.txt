cmake_minimum_required(VERSION 2.8)
project(SortMergeJoin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread -O3")

set(SOURCE_FILES main.cpp Statistic.cpp Statistic.h SortMergeJoin.cpp SortMergeJoin.h ParallelSMJ.cpp ParallelSMJ.h SMJUtility.h SMJUtility.cpp Test.h Test.cpp ThreadPool.cpp ThreadPool.h ThreadWork.cpp ThreadWork.h)
add_executable(SortMergeJoin ${SOURCE_FILES})