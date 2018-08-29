#pragma once

#ifndef CWK_LIBRARY_H
#define CWK_LIBRARY_H

#include <stdbool.h>
#include <stddef.h>

/**
 * A segment represents a single component of a path. For instance, on linux a
 * path might look like this "/var/log/", which consists of two segments "var"
 * and "log".
 */
struct cwk_segment
{
  const char *path;
  const char *begin;
  const char *end;
  size_t size;
};

/**
 * The segment type can be used to identify whether a segment is a special
 * segment or not.
 *
 * CWK_NORMAL - normal folder or file segment
 * CWK_CURRENT - "./" current folder segment
 * CWK_BACK - "../" relative back navigation segment
 */
enum cwk_segment_type
{
  CWK_NORMAL,
  CWK_CURRENT,
  CWK_BACK,
};

/**
 * @brief Creates a normalized version of the path.
 *
 * This function creates a normalized version of the path within the specified
 * buffer. This function will not write out more than the specified buffer can
 * contain. However, the generated string is always null-terminated - even if
 * not the whole path is written out. The function returns the total number of
 * characters the complete buffer would have, even if it was not written out
 * completely. The path may overlap buffer.
 *
 * This function is currently only available for absolute paths.
 *
 * The following will be true for the normalized path:
 * 1) "../" will be resolved.
 * 2) "./" will be removed.
 * 3) double separators will be fixed with a single separator.
 * 4) separator suffixes will be removed.
 *
 * @param path The path which will be normalized.
 * @param buffer The buffer where the new path is written to.
 * @param buffer_size The size of the buffer.
 * @return The size which the complete normalized path has.
 */
size_t cwk_path_normalize(const char *path, char *buffer, size_t buffer_size);

void cwk_path_get_absolute_path(const char *base, const char *path,
  char *buffer, size_t buffer_size);

void cwk_path_get_relative_path(const char *base, const char *path,
  char *buffer, size_t buffer_size);

size_t cwk_path_join(const char *path_a, const char *path_b, char *buffer,
  size_t buffer_size);

bool cwk_path_has_extension(const char *path);

size_t cwk_path_get_extension(const char *path, char *buffer,
  size_t buffer_size);

size_t cwk_path_set_extension(const char *path, const char *new_extension,
  char *buffer, size_t buffer_size);

size_t cwk_path_find_common(const char *path_a, const char *path_b);

/**
 * @brief Gets the first segment of a path.
 *
 * This function finds the first segment of a path. The position of the segment
 * is set to the first character after the separator, and the length counts all
 * characters until the next separator (excluding the separator).
 *
 * @param path The path which will be inspected.
 * @param segment The segment which will be extracted.
 * @return Returns true if there is a segment or false if there is none.
 */
bool cwk_path_get_first_segment(const char *path, struct cwk_segment *segment);

/**
 * @brief Gets the last segment of the path.
 *
 * This function gets the last segment of a path. This function may return false
 * if the path doesn't contain any segments, in which case the submitted segment
 * parameter is not modified. The position of the segment is set to the first
 * character after the separator, and the length counts all characters until the
 * end of the path (excluding the separator).
 *
 * @param path The path which will be inspected.
 * @param segment The segment which will be extracted.
 * @return Returns true if there is a segment or false if there is none.
 */
bool cwk_path_get_last_segment(const char *path, struct cwk_segment *segment);

/**
 * @brief Advances to the next segment.
 *
 * This function advances the current segment to the next segment. If there are
 * no more segments left, the submitted segment structure will stay unchanged
 * and false is returned.
 *
 * @param segment The current segment which will be advanced to the next one.
 * @return Returns true if another segment was found or false otherwise.
 */
bool cwk_path_get_next_segment(struct cwk_segment *segment);

/**
 * @brief Moves to the previous segment.
 *
 * This function moves the current segment to the previous segment. If the
 * current segment is the first one, the submitted segment structure will stay
 * unchanged and false is returned.
 *
 * @param segment The current segment which will be moved to the previous one.
 * @return Returns true if there is a segment before this one or false
 * otherwise.
 */
bool cwk_path_get_previous_segment(struct cwk_segment *segment);

/**
 * @brief Gets the type of the submitted path segment.
 *
 * This function inspects the contents of the segment and determines the type of
 * it. Currently, there are three types CWK_NORMAL, CWK_CURRENT and CWK_BACK. A
 * CWK_NORMAL segment is a normal folder or file entry. A CWK_CURRENT is a "./"
 * and a CWK_BACK a "../" segment.
 *
 * @param segment The segment which will be inspected.
 * @return Returns the type of the segment.
 */
enum cwk_segment_type cwk_path_get_segment_type(
  const struct cwk_segment *segment);

/**
 * @brief Checks whether the submitted pointer points to a separator.
 *
 * This function simply checks whether the submitted pointer points to a
 * separator, which has to be null-terminated (but not necessarily after the
 * separator). The function will return true if it is a separator, or false
 * otherwise.
 *
 * @param symbol A pointer to a string.
 * @return Returns true if it is a separator, or false otherwise.
 */
bool cwk_path_is_separator(const char *str);

#endif
