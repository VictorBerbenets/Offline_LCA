cmake_minimum_required(VERSION 3.15)

project(offline_lca)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(CMAKE_CXX_STANDARD 20)

add_executable(offline_lca ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

set_target_properties(
  offline_lca PROPERTIES
    COMPILE_FLAGS " -Wall -Wextra "
)

find_package(GTest REQUIRED)
enable_testing()

find_package(Threads REQUIRED)

target_include_directories(offline_lca PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(tests)
