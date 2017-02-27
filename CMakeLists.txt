# Copyright 2017 Bartosz Bielecki

cmake_minimum_required(VERSION 3.4.0)

project(StreamingMedian CXX)

# Deal with funny installation of nested projects
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "..." FORCE)

add_subdirectory("ext/Catch")
include_directories("ext/Catch/include")

set(
    SOURCES
    src/SingleArrayMedian.hpp
    src/SortedArray.hpp
    src/BinaryHeap.hpp
    src/TwoHeapsMedian.hpp
)

add_executable(smedian src/main.cpp ${SOURCES})
add_executable(smedian_test src/tests.cpp ${SOURCES})

include(CTest)

add_test(ApplicationRuns smedian)
add_test(UnitTestsPass smedian_test)

install(TARGETS smedian_test DESTINATION bin)
install(TARGETS smedian DESTINATION bin)

