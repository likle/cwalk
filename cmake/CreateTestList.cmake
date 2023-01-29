function(create_test_list list_name target)
  set("TEST_LIST_TARGET_${list_name}" ${target} PARENT_SCOPE)
endfunction()

function(write_test_file list_name file)
  set("TEST_LIST_FILE_${list_name}" ${file} PARENT_SCOPE)
  file(WRITE ${file} "#define UNIT_TESTS(XX) \\
${TEST_LIST_CONTENT_${list_name}}
")
endfunction()

function(create_test list_name unit_name test_name)
  set(TEST_LIST_CONTENT_${list_name} "${TEST_LIST_CONTENT_${list_name}}  XX(${unit_name},${test_name}) \\\n" PARENT_SCOPE)
  add_test(NAME "${unit_name}_${test_name}" COMMAND ${TEST_LIST_TARGET_${list_name}} ${unit_name} ${test_name})
endfunction()
