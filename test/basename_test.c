#include <cwalk.h>
#include <memory.h>
#include <stdlib.h>

int basename_special_directories()
{
  const char *path, *basename;
  size_t length;

  path = "..";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 2) {
    return EXIT_FAILURE;
  }

  if (strncmp(basename, "..", length) != 0) {
    return EXIT_FAILURE;
  }

  path = ".";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strncmp(basename, ".", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_no_separators()
{
  const char *path, *extension;
  size_t length;

  path = "file_name";
  cwk_path_get_basename(path, &extension, &length);

  if (length != 9) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, "file_name", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_trailing_separators()
{
  const char *path, *extension;
  size_t length;

  path = "/my/path.txt////";
  cwk_path_get_basename(path, &extension, &length);

  if (length != 8) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, "path.txt", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_trailing_separator()
{
  const char *path, *extension;
  size_t length;

  path = "/my/path.txt/";
  cwk_path_get_basename(path, &extension, &length);

  if (length != 8) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, "path.txt", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_empty()
{
  const char *path, *basename;
  size_t length;

  path = "";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  if (basename != NULL) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_simple()
{
  const char *path, *basename;
  size_t length;

  path = "/my/path.txt";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 8) {
    return EXIT_FAILURE;
  }

  if (strncmp(basename, "path.txt", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}