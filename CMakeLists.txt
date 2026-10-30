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

include(FetchContent)

# FTXUI 6.1.9 for the TUI (needs C++17); only the libraries, no examples/docs/tests
set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(FTXUI_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(FTXUI_QUIET ON CACHE BOOL "" FORCE)
FetchContent_Declare(ftxui
  URL https://github.com/ArthurSonzogni/FTXUI/archive/refs/tags/v6.1.9.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(ftxui)

# Only the executable uses FTXUI (component pulls in dom and screen); the engine stays UI-free
target_link_libraries(tividy ftxui::component)

# GoogleTest 1.17.0 (needs C++17)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(GLOB TEST_SRC tests/*.cpp)
add_executable(tividy_tests ${TEST_SRC})
target_link_libraries(tividy_tests tividy_engine gtest_main)
include(GoogleTest)
gtest_discover_tests(tividy_tests)
