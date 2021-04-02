#!/usr/bin/env bash

#set -eou pipefail

RELATIVE_PATH_TO_EXE=$1
EXPECTED_PATH_TO_EXE=$(realpath -s $1)
ALIAS_TO_TEST="alias_to_cwalktest_exe_path"
SYMLINK_TO_TEST="symlink_to_cwalktest_exe_path"
NUM_FAILS=0

alias "${ALIAS_TO_TEST}=${EXPECTED_PATH_TO_EXE}"
shopt -s expand_aliases

function record_test_result() {
  case "$1" in
   0) TEST_RESULT="PASSED" ;;
   *) TEST_RESULT="FAILED" && NUM_FAILS=$((${NUM_FAILS}+1)) ;;
  esac
  printf "${TEST_RESULT}\n"
}

function print_running_testname() {
  pad=$(printf '%0.1s' "."{1..30})
  padlength=30
  testname=$1
  printf "Running 'executable_path/%s' %*.*s " \
         "${testname}" 0 $((padlength - ${#testname})) "${pad}"
}
function simple_test() {
  print_running_testname 'simple_test'
  "${RELATIVE_PATH_TO_EXE}" "${EXPECTED_PATH_TO_EXE}"
  record_test_result $?
}

function alias_test() {
  print_running_testname 'alias_test'
  eval "${ALIAS_TO_TEST}" "${EXPECTED_PATH_TO_EXE}"
  record_test_result $?
}

function symlink_test() {
  print_running_testname 'symlink_test'
  # Make symlink
  ln -s "${EXPECTED_PATH_TO_EXE}" "${SYMLINK_TO_TEST}"

  # Test symlink
  ./"${SYMLINK_TO_TEST}" "${EXPECTED_PATH_TO_EXE}"
  record_test_result $?

  # Remove symlink
  rm "${SYMLINK_TO_TEST}"
}

simple_test
alias_test
symlink_test

[[ ${NUM_FAILS} -ne 0 ]] && echo "Num failures=${NUM_FAILS}" && exit 1

echo "All tests passed"
exit 0
