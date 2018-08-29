cmake_minimum_required(VERSION 3.9.2)

# set project name
project(cwalk
  VERSION 0.1.0
  DESCRIPTION "A simple path library"
  LANGUAGES C)

# include utilities
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(EnableWarnings)
include(CTest)
include(CreateTestList)

# configure requirements
set(CMAKE_C_STANDARD 11)

# setup target and directory names
set(LIBRARY_TARGET "cwalk")
set(TEST_TARGET "cwalktest")
set(INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")

# generate constants
# configure_file("${SOURCE_DIRECTORY}/constants.h.in" "${SOURCE_DIRECTORY}/constants.h")

# enable coverage if requested
if(ENABLE_COVERAGE)
  message("-- Coverage enabled")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# add the main executable
add_library(${LIBRARY_TARGET}
  "${INCLUDE_DIRECTORY}/cwalk.h"
  "${SOURCE_DIRECTORY}/path.c")
target_include_directories(${LIBRARY_TARGET} PUBLIC "${INCLUDE_DIRECTORY}")

# add tests
enable_testing()
create_test(DEFAULT extension get_simple)
create_test(DEFAULT extension get_without)
create_test(DEFAULT extension get_first)
create_test(DEFAULT extension get_last)
create_test(DEFAULT extension get_multiple)
create_test(DEFAULT extension check_simple)
create_test(DEFAULT extension check_without)
create_test(DEFAULT normalize do_nothing)
create_test(DEFAULT normalize navigate_back)
create_test(DEFAULT normalize navigate_too_far)
create_test(DEFAULT normalize terminated)
create_test(DEFAULT normalize double_separator)
create_test(DEFAULT normalize remove_current)
create_test(DEFAULT normalize mixed)
create_test(DEFAULT normalize overlap)
create_test(DEFAULT segment first)
create_test(DEFAULT segment last)
create_test(DEFAULT segment next)
create_test(DEFAULT segment previous)
create_test(DEFAULT segment type)
create_test_list(DEFAULT "${TEST_DIRECTORY}/tests.h")

add_executable(${TEST_TARGET}
  "${TEST_DIRECTORY}/main.c"
  "${TEST_DIRECTORY}/extension_test.c"
  "${TEST_DIRECTORY}/normalize_test.c"
  "${TEST_DIRECTORY}/segment_test.c")
target_link_libraries(${TEST_TARGET} PUBLIC ${LIBRARY_TARGET})
target_include_directories(${TEST_TARGET} PUBLIC "${INCLUDE_DIRECTORY}")

# enable warnings
enable_warnings(${LIBRARY_TARGET})
