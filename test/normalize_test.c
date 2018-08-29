#include <cwalk.h>
#include <limits.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int normalize_overlap()
{
  size_t count;
  char result[FILENAME_MAX];
  char *input, *expected;

  input = "/var/./logs/.//test/..//..//////";
  strcpy(result, input);
  expected = "/var";
  count = cwk_path_normalize(result, result, sizeof(result));
  if (count != strlen(expected) || strcmp(result, expected) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int normalize_mixed()
{
  size_t count;
  char result[FILENAME_MAX];
  char *input, *expected;

  input = "/var/./logs/.//test/..//..//////";
  expected = "/var";
  count = cwk_path_normalize(input, result, sizeof(result));
  if (count != strlen(expected) || strcmp(result, expected) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int normalize_remove_current()
{
  size_t count;
  char result[FILENAME_MAX];
  char *input, *expected;

  input = "/var/././././";
  expected = "/var";
  count = cwk_path_normalize(input, result, sizeof(result));
  if (count != strlen(expected) || strcmp(result, expected) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int normalize_double_separator()
{
  size_t count;
  char result[FILENAME_MAX];
  char *input, *expected;

  input = "/var////logs//test/";
  expected = "/var/logs/test";
  count = cwk_path_normalize(input, result, sizeof(result));
  if (count != strlen(expected) || strcmp(result, expected) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int normalize_terminated()
{
  size_t count;
  char result[FILENAME_MAX];
  char *input, *expected;
  size_t i;

  input = "/var/logs/test/../../";
  expected = "/var";
  for (i = 0; i < 7; ++i) {
    count = cwk_path_normalize(input, result, i);
    if (count != strlen(expected) ||
        (i > 0 &&
          (strncmp(result, expected, i - 1) != 0 || result[i - 1] != '\0'))) {
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int normalize_navigate_back()
{
  size_t count;
  char result[FILENAME_MAX];
  char *input, *expected;

  input = "/var/logs/test/../../";
  expected = "/var";
  count = cwk_path_normalize(input, result, sizeof(result));
  if (count != strlen(expected) || strcmp(result, expected) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int normalize_do_nothing()
{
  size_t count;
  char result[FILENAME_MAX];
  char *input, *expected;

  input = "/var";
  expected = "/var";
  count = cwk_path_normalize(input, result, sizeof(result));
  if (count != strlen(expected) || strcmp(result, expected) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}