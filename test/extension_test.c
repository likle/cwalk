#include <cwalk.h>
#include <memory.h>
#include <stdlib.h>

int extension_check_without()
{
  const char *path;

  path = "/my/path";
  if (cwk_path_has_extension(path)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_check_simple()
{
  const char *path;

  path = "/my/path.txt";
  if (!cwk_path_has_extension(path)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_multiple()
{
  const char *path;
  const char *extension;
  size_t length;

  path = "/my/path.abc.txt.tests";
  if (!cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  if (length != 6) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, ".tests", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_last()
{
  const char *path;
  const char *extension;
  size_t length;

  path = "/my/path.";
  if (!cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, ".", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_first()
{
  const char *path;
  const char *extension;
  size_t length;

  path = "/my/.path";
  if (!cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  if (length != 5) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, ".path", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_without()
{
  const char *path;
  const char *extension;
  size_t length;

  path = "/my/path";
  if (cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_simple()
{
  const char *path;
  const char *extension;
  size_t length;

  path = "/my/path.txt";
  if (!cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  if (length != 4) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, ".txt", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}