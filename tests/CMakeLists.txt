cmake_minimum_required(VERSION 3.15)

project("gtest")

aux_source_directory(./unit    UNIT_TESTS)
aux_source_directory(./end2end/src END2END_TESTS)

add_executable(unit ${UNIT_TESTS})
add_executable(end2end ${END2END_TESTS} ${TESTING_FILES})
add_executable(checker ${CMAKE_CURRENT_SOURCE_DIR}/end2end/checker.cpp)

target_include_directories(end2end PRIVATE end2end/include ${INCLUDE_DIR})
target_include_directories(checker PRIVATE ${INCLUDE_DIR})
target_include_directories(unit PRIVATE ${INCLUDE_DIR})

target_link_libraries(unit PRIVATE ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
