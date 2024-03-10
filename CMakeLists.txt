cmake_minimum_required(VERSION 3.15)

project(offline_lca)

set(GRAPH_DIR ${PROJECT_SOURCE_DIR}/include)
set(CMAKE_CXX_STANDARD 20)

add_executable(offline_lca ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

set_target_properties(
  offline_lca PROPERTIES
    COMPILE_FLAGS " -Wall -Wextra "
)

target_include_directories(offline_lca PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

