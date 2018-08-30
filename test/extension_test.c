#include <cwalk.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

int extension_check_without()
{
  if (cwk_path_has_extension("/my/path")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_check_empty()
{
  if (cwk_path_has_extension("")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_check_simple()
{
  if (!cwk_path_has_extension("/my/path.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_multiple()
{
  const char *path, *extension;
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
  const char *path, *extension;
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
  const char *path, *extension;
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
  const char *path, *extension;
  size_t length;

  path = "/my/path";
  if (cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_simple()
{
  const char *path, *extension;
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