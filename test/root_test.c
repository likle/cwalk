#include <cwalk.h>
#include <stdlib.h>

int root_relative_windows()
{
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);
  cwk_path_get_root("..\\hello\\world.txt", &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int root_relative_drive()
{
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);
  cwk_path_get_root("C:test.txt", &length);

  if (length != 2) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int root_unc()
{
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);
  cwk_path_get_root("\\\\server\\folder\\data", &length);

  if (length != 16) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int root_absolute_drive()
{
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);
  cwk_path_get_root("C:\\test.txt", &length);

  if (length != 3) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int root_relative()
{
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  cwk_path_get_root("test.txt", &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int root_absolute()
{
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  cwk_path_get_root("/test.txt", &length);

  if (length != 1) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}