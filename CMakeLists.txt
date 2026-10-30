cmake_minimum_required(VERSION 3.14)
project(TIVIDY CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

# Engine = every .cpp in src except main.cpp
file(GLOB ENGINE_SRC src/*.cpp)
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_library(tividy_engine ${ENGINE_SRC})

add_executable(tividy src/main.cpp)
target_link_libraries(tividy tividy_engine)

# GoogleTest 1.12.1 = last version that supports C++11
include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(GLOB TEST_SRC tests/*.cpp)
add_executable(tividy_tests ${TEST_SRC})
target_link_libraries(tividy_tests tividy_engine gtest_main)
include(GoogleTest)
gtest_discover_tests(tividy_tests)
