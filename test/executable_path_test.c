#include <cwalk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define adequate_buffer_len 4096
#define inadequate_buffer_len 2

int executable_path_adequate_buffer(const char * expected_exe_path)
{
  char buffer[adequate_buffer_len];
  bool is_trunc = false;
  if (cwk_path_get_executable_path(buffer, adequate_buffer_len, &is_trunc) == 0) {
    return EXIT_FAILURE;
  }
  if (strcmp(buffer, expected_exe_path) != 0) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int executable_path_inadequate_buffer()
{
  char buffer[inadequate_buffer_len];
  bool is_trunc = false;
  if (cwk_path_get_executable_path(buffer, inadequate_buffer_len, &is_trunc) != 0 ||
      !is_trunc) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int executable_path_borderline_buffer(const char * expected_exe_path)
{
  // If the buffer is just long enough to fit the path, but not long enough to
  // fit a null terminal at the end, the buffer could contain an unterminated
  // string. This test makes sure that this will not happen.

  size_t borderline_length = strlen(expected_exe_path);
  char* buffer = (char*)malloc(borderline_length * sizeof(char));
  bool is_trunc = false;

  if (cwk_path_get_executable_path(buffer, borderline_length, &is_trunc) != 0 ||
      buffer[0] != '\0' ||
      !is_trunc) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

bool report_failure(int result, const char * test_name, const char * expected_path)
{
  if (EXIT_FAILURE == result) {
    fprintf(stderr,
            "Failed 'executable_path' test '%s' with expected_path='%s'\n",
            test_name,
            expected_path);
    return true;
  }
  return false;
}

int main(int argc, char *argv[])
{
  const char * expected_path;
  bool has_failed;

  if (argc != 2) {
    fprintf(stderr,"Usage: %s <absolute_path_to_program_executable>\n", argv[0]);
    return EXIT_FAILURE;
  }
  expected_path = argv[1];
  has_failed = false;

  has_failed |= report_failure(executable_path_adequate_buffer(expected_path),
                               "adequate_buffer", expected_path);
  has_failed |= report_failure(executable_path_borderline_buffer(expected_path),
                               "borderline_buffer", expected_path);
  has_failed |= report_failure(executable_path_inadequate_buffer(),
                               "inadequate_buffer", expected_path);
  return has_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
