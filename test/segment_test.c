#include <cwalk.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

int segment_back_with_root()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  path = "C:\\this\\path";
  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "path", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "this", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_type()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/a/./../.folder/..folder";

  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_segment_type(&segment) != CWK_NORMAL) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_segment_type(&segment) != CWK_CURRENT) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_segment_type(&segment) != CWK_BACK) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_segment_type(&segment) != CWK_NORMAL) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_segment_type(&segment) != CWK_NORMAL) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_previous_too_far()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/now/hello_world/abc/";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_previous_relative()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "now/hello_world/abc/";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "now", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_previous_absolute()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/now/hello_world/abc/";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "now", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_next_too_far()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/hello_world/abc/";

  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_next()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/hello_world/abc/";

  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_last()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/hello_world/abc";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  path = "hello_world/abc";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_first()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/hello_world/abc";

  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  path = "hello_world/abc";

  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
