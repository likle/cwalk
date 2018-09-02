#include <cwalk.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int join_relative_back_after_root()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);
  length = cwk_path_join("this\\", "C:\\..\\..\\is\\a\\test\\", buffer,
    sizeof(buffer));

  if (length != 9) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "is\\a\\test") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int join_back_after_root()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);
  length = cwk_path_join("C:\\this\\path", "C:\\..\\is\\a\\test\\", buffer,
    sizeof(buffer));

  if (length != 22) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "C:\\this\\path\\is\\a\\test") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int join_two_unc()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);
  length = cwk_path_join("\\\\server\\unc\\path", "\\\\server2\\unc\\path",
    buffer, sizeof(buffer));

  if (length != 34) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "\\\\server\\unc\\path\\server2\\unc\\path") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int join_two_absolute()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_join("/first", "/second", buffer, sizeof(buffer));

  if (length != 13) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/first/second") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int join_empty()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_join("hello", "..", buffer, sizeof(buffer));

  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, ".") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int join_navigate_back()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_join("hello/there", "..", buffer, sizeof(buffer));

  if (length != 5) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "hello") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int join_simple()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_join("hello", "there", buffer, sizeof(buffer));

  if (length != 11) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "hello/there") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}