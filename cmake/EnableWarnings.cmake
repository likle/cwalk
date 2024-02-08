# enable warnings
function(enable_warnings target)
  if(MSVC)
    target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(${target} PRIVATE /W4)
  elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${target} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wno-gnu-zero-variadic-macro-arguments
    )
  elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${target} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    )
  endif()

  if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24")
    if(NOT IGNORE_WARNINGS)
      set_property(TARGET ${target} PROPERTY COMPILE_WARNING_AS_ERROR true)
    endif()
  endif()
endfunction()
