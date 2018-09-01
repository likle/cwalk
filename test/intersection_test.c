#include <cwalk.h>
#include <stdlib.h>

int intersection_empty()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (cwk_path_get_intersection("/", "") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_double_separator()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_get_intersection("/this///is/a//test", "/this//is/a///file") !=
      12) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_trailing_separator()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_get_intersection("/this/is/a/test", "/this/is/a/") != 10) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_truncated()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_get_intersection("/this/is/a/test", "/this/is/a") != 10) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_simple()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_get_intersection("/this/is/a/test", "/this/is/a/string") != 10) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}