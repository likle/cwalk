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

size_t cwk_path_get_absolute_path(const char *base, const char *path,
  char *buffer, size_t buffer_size);

void cwk_path_get_relative_path(const char *base, const char *path,
  char *buffer, size_t buffer_size);

size_t cwk_path_join(const char *path_a, const char *path_b, char *buffer,
  size_t buffer_size);

/**
 * @brief Gets the basename from a file path.
 *
 * This function gets the basename of a file path. A pointer to the beginning of
 * the basename will be returned through the basename parameter. This pointer
 * will be positioned on the first letter after the separator. The length of the
 * file path will be returned through the length parameter. The length will be
 * set to zero and the basename to NULL if there is no basename available.
 *
 * @param path The path which will be inspected.
 * @param basename The output of the basename pointer.
 * @param length The output of the length of the basename,
 */
void cwk_path_get_basename(const char *path, const char **basename,
  size_t *length);

/**
 * @brief Gets the dirname of a file path.
 *
 * This function determines the dirname of a file path and returns the length up
 * to which character is considered to be part of it. If no dirname is found,
 * the length will be set to zero. The beginning of the dirname is always equal
 * to the submitted path pointer.
 *
 * @param path The path which will be inspected.
 * @param length The length of the dirname.
 */
void cwk_path_get_dirname(const char *path, size_t *length);

/**
 * @brief Gets the extension from a file path.
 *
 * This function extracts the extension portion from a file path. A pointer to
 * the beginning of the extension will be returned through the extension
 * parameter if an extension is found and true is returned. This pointer will be
 * positioned on the dot. The length of the extension name will be returned
 * through the length parameter. If no extension is found both parameters won't
 * be touched and false will be returned.
 *
 * @param path The path which will be inspected.
 * @param extension The output of the extension pointer.
 * @param length The output of the length of the extension.
 * @return Returns true if an extension is found or false otherwise.
 */
bool cwk_path_get_extension(const char *path, const char **extension,
  size_t *length);

/**
 * @brief Determines whether the file path has an extension.
 *
 * This function determines whether the submitted file path has an extension.
 * This will evaluate to true if the last segment of the path contains a dot.
 *
 * @param path The path which will be inspected.
 * @return Returns true if the path has an extension or false otherwise.
 */
bool cwk_path_has_extension(const char *path);

size_t cwk_path_set_extension(const char *path, const char *new_extension,
  char *buffer, size_t buffer_size);

/**
 * @brief Creates a normalized version of the path.
 *
 * This function creates a normalized version of the path within the specified
 * buffer. This function will not write out more than the specified buffer can
 * contain. However, the generated string is always null-terminated - even if
 * not the whole path is written out. The function returns the total number of
 * characters the complete buffer would have, even if it was not written out
 * completely. The path may be the same memory address as the buffer.
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

/**
 * @brief Finds common portions in two paths.
 *
 * This function finds common portions in two paths and returns the number
 * characters from the beginning of the base path which are equal to the other
 * path.
 *
 * @param path_base The base path which will be compared with the other path.
 * @param path_other The other path which will compared with the base path.
 * @return Returns the number of characters which are common in the base path.
 */
size_t cwk_path_get_intersection(const char *path_base, const char *path_other);

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
 * @brief Changes the content of a segment.
 *
 * This function overrides the content of a segment to the submitted value and
 * outputs the whole new path to the submitted buffer. The result might require
 * less or more space than before if the new value length differs from the
 * original length. The output is truncated if the new path is larger than the
 * submitted buffer size, but it is always null-terminated. The source of the
 * segment and the submitted buffer may be the same.
 *
 * @param segment The segment which will be modifier.
 * @param value The new content of the segment.
 * @param buffer The buffer where the modified path will be written to.
 * @param buffer_size The size of the output buffer.
 * @return Returns the total size which would have been written if the output
 * was not truncated.
 */
size_t cwk_path_change_segment(struct cwk_segment *segment, const char *value,
  char *buffer, size_t buffer_size);

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
