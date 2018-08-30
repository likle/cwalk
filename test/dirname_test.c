#include <cwalk.h>
#include <memory.h>
#include <stdlib.h>

int dirname_root()
{
  const char *path;
  size_t length;

  path = "/";
  cwk_path_get_dirname(path, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_special_directories()
{
  const char *path;
  size_t length;

  path = "..";
  cwk_path_get_dirname(path, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  path = ".";
  cwk_path_get_dirname(path, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_no_separators()
{
  const char *path;
  size_t length;

  path = "file_name";
  cwk_path_get_dirname(path, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_trailing_separators()
{
  const char *path;
  size_t length;

  path = "/my/path.txt////";
  cwk_path_get_dirname(path, &length);

  if (length != 4) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_trailing_separator()
{
  const char *path;
  size_t length;

  path = "/my/path.txt/";
  cwk_path_get_dirname(path, &length);

  if (length != 4) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_empty()
{
  const char *path;
  size_t length;

  path = "";
  cwk_path_get_dirname(path, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_simple()
{
  const char *path;
  size_t length;

  path = "/my/path.txt";
  cwk_path_get_dirname(path, &length);

  if (length != 4) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}