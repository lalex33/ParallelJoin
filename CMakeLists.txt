cmake_minimum_required(VERSION 2.8)
project(SortMergeJoin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread -O3")

set(SOURCE_FILES main.cpp Statistic.cpp Statistic.h SortMergeJoin.cpp SortMergeJoin.h ParallelSMJ.cpp ParallelSMJ.h
        SMJUtility.h SMJUtility.cpp Test.h Test.cpp ThreadPool.cpp ThreadPool.h ThreadWork.cpp ThreadWork.h PartitionedArray.hpp ParallelHashJoin.cpp ParallelHashJoin.hpp)
add_executable(SortMergeJoin ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(PAPI)

if (PAPI_FOUND)
    include_directories(${PAPI_INCLUDE_DIRS})
    target_link_libraries (SortMergeJoin ${PAPI_LIBRARIES})
endif (PAPI_FOUND)