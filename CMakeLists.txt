cmake_minimum_required(VERSION 3.14)

# set project name
project(cwalk
  VERSION 1.2.7
  DESCRIPTION "A simple path library"
  HOMEPAGE_URL "https://likle.github.io/cwalk/"
  LANGUAGES C)

message(STATUS "CMake ${CMAKE_VERSION} ${PROJECT_NAME} ${PROJECT_VERSION}
Install Prefix: ${CMAKE_INSTALL_PREFIX}
Generator: ${CMAKE_GENERATOR} ${CMAKE_GENERATOR_PLATFORM}")

# include utilities
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GNUInstallDirs)
include(EnableWarnings)
include(CreateTestList)
include(CMakePackageConfigHelpers)
enable_testing()

option(ENABLE_COVERAGE "Coverage testing")
option(ENABLE_SANITIZER "Sanitzier testing")
option(BUILD_SHARED_LIBS "Build shared library")
option(ENABLE_TESTS "Build test executables")

# configure requirements
set(CMAKE_C_STANDARD 11)

# enable coverage if requested
if(ENABLE_COVERAGE)
  message(STATUS "Coverage enabled")
  add_compile_options(-fprofile-arcs -ftest-coverage)
  add_link_options(--coverage)
endif()

# enable sanitizer
if(ENABLE_SANITIZER)
  message(STATUS "Sanitizer enabled")
  add_compile_options(-fno-omit-frame-pointer -fsanitize=${ENABLE_SANITIZER})
  add_link_options(-fno-omit-frame-pointer -fsanitize=${ENABLE_SANITIZER})
endif()

# add the main executable
add_library(cwalk include/cwalk.h src/cwalk.c)
enable_warnings(cwalk)
target_include_directories(cwalk PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
set_property(TARGET cwalk PROPERTY PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/cwalk.h)
target_compile_definitions(cwalk PRIVATE CWK_EXPORTS)

# add shared library macro
if(BUILD_SHARED_LIBS)
  target_compile_definitions(cwalk PUBLIC CWK_SHARED)
endif()

# enable tests
if(ENABLE_TESTS)
  message(STATUS "Tests enabled")

  create_test_list(DEFAULT cwalktest)

  foreach(t IN ITEMS simple absolute_path unix_relative_base windows_relative_base mixed
          normalization too_far check buffer_reuse)
    create_test(DEFAULT absolute ${t})
  endforeach()

  foreach(t IN ITEMS simple empty trailing_separator trailing_separators no_separators
          special_directories root windows change_simple change_empty_path change_only_root
          change_empty_basename change_relative change_trim change_trim_only_root)
    create_test(DEFAULT basename ${t})
  endforeach()

  foreach(t IN ITEMS simple empty trailing_separator trailing_separators no_separators
          special_directories root three_segments relative)
    create_test(DEFAULT dirname ${t})
  endforeach()

  foreach(t IN ITEMS get_simple get_without get_first get_last get_multiple check_simple
          check_empty check_without change_simple change_no_basename change_no_extension
          change_with_dot change_overlap change_overlap_long change_hidden_file change_with_trailing_slash)
    create_test(DEFAULT extension ${t})
  endforeach()

  foreach(t IN ITEMS empty_string windows_root unix_root windows_separator unix_separator
          hidden_file extension unguessable)
    create_test(DEFAULT guess ${t})
  endforeach()

  foreach(t IN ITEMS simple trailing_separator double_separator empty unequal_roots relative_absolute_mix
          same_roots one_root_only relative_base relative_other skipped_end)
    create_test(DEFAULT intersection ${t})
  endforeach()

  foreach(t IN ITEMS absolute unc device_unc device_dot device_question_mark relative windows_backslash
          windows_slash unix_backslash unix_drive absolute_drive relative_drive relative_windows root dir)
    create_test(DEFAULT is_absolute ${t})
  endforeach()

  foreach(t IN ITEMS absolute unc device_unc device_dot device_question_mark relative windows_backslash
          windows_slash unix_backslash unix_drive absolute_drive relative_drive relative_windows)
    create_test(DEFAULT is_relative ${t})
  endforeach()

  foreach(t IN ITEMS simple navigate_back empty two_absolute two_unc with_two_roots back_after_root
          relative_back_after_root multiple)
    create_test(DEFAULT join ${t})
  endforeach()

  foreach(t IN ITEMS do_nothing navigate_back relative_too_far absolute_too_far terminated double_separator
          remove_current mixed overlap empty only_separators back_after_root forward_slashes)
    create_test(DEFAULT normalize ${t})
  endforeach()

  foreach(t IN ITEMS simple relative long_base long_target equal same_base base_skipped_end
          target_skipped_end base_div_skipped_end target_div_skipped_end skip_all different_roots
          relative_and_absolute check root_path_unix root_path_windows root_forward_slashes)
    create_test(DEFAULT relative ${t})
  endforeach()

  foreach(t IN ITEMS absolute unc device_unc device_dot device_question_mark relative windows_backslash
          windows_slash unix_backslash unix_drive absolute_drive relative_drive relative_windows
          change_simple change_empty change_separators change_overlapping change_without_root)
    create_test(DEFAULT root ${t})
  endforeach()

  foreach(t IN ITEMS first last next next_too_far previous_absolute previous_relative
          previous_absolute_one_char_first previous_relative_one_char_first previous_too_far
          previous_too_far_root type back_with_root change_simple change_first change_last change_trim
          change_empty change_with_separator change_overlap)
    create_test(DEFAULT segment ${t})
  endforeach()

  foreach(t IN ITEMS change_style get_root get_unc_root get_root_separator get_root_relative
          intersection_case root_backslash root_empty)
    create_test(DEFAULT windows ${t})
  endforeach()

  write_test_file(DEFAULT ${CMAKE_CURRENT_BINARY_DIR}/tests.h)

  add_executable(cwalktest
    test/main.c
    test/absolute_test.c
    test/basename_test.c
    test/dirname_test.c
    test/extension_test.c
    test/guess_test.c
    test/intersection_test.c
    test/is_absolute_test.c
    test/is_relative_test.c
    test/join_test.c
    test/normalize_test.c
    test/relative_test.c
    test/root_test.c
    test/segment_test.c
    test/windows_test.c)
  target_include_directories(cwalktest PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  enable_warnings(cwalktest)

  target_link_libraries(cwalktest PRIVATE cwalk)
endif()

write_basic_package_version_file("CwalkConfigVersion.cmake"
  VERSION ${cwalk_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_file(cwalk.pc.in ${PROJECT_BINARY_DIR}/cwalk.pc @ONLY)

install(TARGETS cwalk EXPORT CwalkTargets)

install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CwalkConfig.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cwalk)

install(FILES
  ${PROJECT_BINARY_DIR}/cwalk.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/)

install(EXPORT CwalkTargets
  FILE CwalkTargets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cwalk)

# --- auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()
