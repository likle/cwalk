#include <cwalk.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int absolute_too_far()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute_path("/hello/there", "../../../../../", buffer,
    sizeof(buffer));

  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_normalization()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute_path("/hello//../there", "test//thing", buffer,
    sizeof(buffer));

  if (length != 17) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/there/test/thing") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_mixed()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute_path("hello/there", "/test", buffer,
    sizeof(buffer));

  if (length != 5) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/test") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_relative_base()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute_path("hello/there", "test", buffer,
    sizeof(buffer));

  if (length != 17) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/hello/there/test") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_absolute_path()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute_path("/hello/there", "/test", buffer,
    sizeof(buffer));

  if (length != 5) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/test") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_simple()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute_path("/hello/there", "..", buffer,
    sizeof(buffer));

  if (length != 6) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/hello") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}