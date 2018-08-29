#include <cwalk.h>
#include <limits.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int normalize_simple()
{
  size_t count;
  char normalized[FILENAME_MAX];
  char *input, *expected;

  input = "/var/logs//test/../.././";
  expected = "/var";
  count = cwk_path_normalize(input, normalized, sizeof(normalized));
  if(count != strlen(expected) || strcmp(normalized, expected) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}