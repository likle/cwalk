#include <assert.h>
#include <cwalk.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/**
 * We try to default to a different path style depending on the operating
 * system. So this should detect whether we should use windows or unix paths.
 */
#if defined(WIN32) || defined(_WIN32) ||                                       \
  defined(__WIN32) && !defined(__CYGWIN__)
static enum cwk_path_style path_style = CWK_STYLE_WINDOWS;
#else
static enum cwk_path_style path_style = CWK_STYLE_UNIX;
#endif

/**
 * This is a list of separators used in different styles. Windows can read
 * multiple separators, but it generally outputs just a backslash. The output
 * will always use the first character for the output.
 */
static const char *separators[] = {[CWK_STYLE_WINDOWS] = "\\/",
  [CWK_STYLE_UNIX] = "/"};

/**
 * A joined path represents multiple path strings which are concatenated, but
 * not (necessarily) stored in contiguous memory. The joined path allows to
 * iterate over the segments as if it was one piece of path.
 */
struct cwk_segment_joined
{
  struct cwk_segment segment;
  const char **paths;
  size_t path_index;
};

static size_t cwk_path_output_sized(char *buffer, size_t buffer_size,
  size_t position, const char *str, size_t length)
{
  size_t amount_written;

  // First we determine the amount which we can write to the buffer. There are
  // three cases. In the first case we have enough to store the whole string in
  // it. In the second one we can only store a part of it, and in the third we
  // have no space left.
  if (buffer_size > position + length) {
    amount_written = length;
  } else if (buffer_size > position) {
    amount_written = buffer_size - position;
  } else {
    amount_written = 0;
  }

  // If we actually want to write out something we will do that here. We will
  // always append a '\0', this way we are guaranteed to have a valid string at
  // all times.
  if (amount_written > 0) {
    memmove(&buffer[position], str, amount_written);
  }

  // Return the theoretical length which would have been written when everything
  // would have fit in the buffer.
  return length;
}

static size_t cwk_path_output_separator(char *buffer, size_t buffer_size,
  size_t position)
{
  // We output a separator, which is a single character..
  return cwk_path_output_sized(buffer, buffer_size, position,
    separators[path_style], 1);
}

static const char *cwk_path_find_next_stop(const char *c)
{
  // We just move forward until we find a '\0' or a separator, which will be our
  // next "stop".
  while (*c != '\0' && !cwk_path_is_separator(c)) {
    ++c;
  }

  // Return the pointer of the next stop.
  return c;
}

static const char *cwk_path_find_previous_stop(const char *begin, const char *c)
{
  // We just move back until we find a separator or reach the beginning of the
  // path, which will be our previous "stop".
  while (c > begin && !cwk_path_is_separator(c)) {
    --c;
  }

  // Return the pointer to the previous stop.
  return c;
}

static bool cwk_path_get_first_segment_joined(const char **paths,
  struct cwk_segment_joined *sj)
{
  bool result;

  // Prepare the first segment. We position the joined segment on the first path
  // and assign the path array to the struct.
  sj->path_index = 0;
  sj->paths = paths;

  // We loop through all paths until we find one which has a segment. The result
  // is stored in a variable, so we can let the caller know whether we found one
  // or not.
  result = false;
  while (paths[sj->path_index] != NULL &&
         (result = cwk_path_get_first_segment(paths[sj->path_index],
            &sj->segment)) == false) {
    ++sj->path_index;
  }

  return result;
}

static bool cwk_path_get_next_segment_joined(struct cwk_segment_joined *sj)
{
  bool result;

  if (sj->paths[sj->path_index] == NULL) {
    // We reached already the end of all paths, so there is no other segment
    // left.
    return false;
  } else if (cwk_path_get_next_segment(&sj->segment)) {
    // There was another segment on the current path, so we are good to
    // continue.
    return true;
  }

  // We try to move to the next path which has a segment available. We must at
  // least move one further since the current path reached the end.
  result = false;

  do {
    ++sj->path_index;

    // And we obviously have to stop this loop if there are no more paths left.
    if (sj->paths[sj->path_index] == NULL) {
      break;
    }

    // Grab the first segment of the next path and determine whether this path
    // has anything useful in it.
    result = cwk_path_get_first_segment(sj->paths[sj->path_index],
      &sj->segment);

  } while (!result);

  // Finally, report the result back to the caller.
  return result;
}

static bool cwk_path_get_previous_segment_joined(struct cwk_segment_joined *pj)
{
  bool result;

  if (*pj->paths == NULL) {
    // It's possible that there is no initialized segment available in the
    // struct since there are no paths. In that case we can return false, since
    // there is no previous segment.
    return false;
  } else if (cwk_path_get_previous_segment(&pj->segment)) {
    // Now we try to get the previous segment from the current path. If we can
    // do that successfully, we can let the caller know that we found one.
    return true;
  }

  result = false;

  do {
    if (pj->path_index == 0) {
      break;
    }

    result = cwk_path_get_last_segment(pj->paths[--pj->path_index],
      &pj->segment);
  } while (!result);

  return result;
}

static bool cwk_path_segment_back_will_be_removed(struct cwk_segment_joined *pj)
{
  enum cwk_segment_type type;
  int counter;

  // We are handling back segments here. We must verify how many back segments
  // and how many normal segments come before this one to decide whether we keep
  // or remove it.

  // The counter determines how many normal segments are our current segment,
  // which will popped off before us. If the counter goes above zero it means
  // that our segment will be popped as well.
  counter = 0;

  // We loop over all previous segments until we either reach the beginning,
  // which means our segment will not be dropped or the counter goes above zero.
  while (cwk_path_get_previous_segment_joined(pj)) {

    // Now grab the type. The type determines whether we will increase or
    // decrease the counter. We don't handle a CWK_CURRENT frame here since it
    // has no influence.
    type = cwk_path_get_segment_type(&pj->segment);
    if (type == CWK_NORMAL) {
      // This is a normal segment. The normal segment will increase the counter
      // since it neutralizes one back segment. If we go above zero we can
      // return immediately.
      ++counter;
      if (counter > 0) {
        return true;
      }
    } else if (type == CWK_BACK) {
      // A CWK_BACK segment will reduce the counter by one. We can not remove a
      // back segment as long we are not above zero since we don't have the
      // opposite normal segment which we would remove.
      --counter;
    }
  }

  // We never got a count larger than zero, so we will keep this segment alive.
  return false;
}

static bool cwk_path_segment_normal_will_be_removed(
  struct cwk_segment_joined *pj)
{
  enum cwk_segment_type type;
  int counter;

  // The counter determines how many segments are above our current segment,
  // which will popped off before us. If the counter goes below zero it means
  // that our segment will be popped as well.
  counter = 0;

  // We loop over all following segments until we either reach the end, which
  // means our segment will not be dropped or the counter goes below zero.
  while (cwk_path_get_next_segment_joined(pj)) {

    // First, grab the type. The type determines whether we will increase or
    // decrease the counter. We don't handle a CWK_CURRENT frame here since it
    // has no influence.
    type = cwk_path_get_segment_type(&pj->segment);
    if (type == CWK_NORMAL) {
      // This is a normal segment. The normal segment will increase the counter
      // since it will be removed by a "../" before us.
      ++counter;
    } else if (type == CWK_BACK) {
      // A CWK_BACK segment will reduce the counter by one. If we are below zero
      // we can return immediately.
      --counter;
      if (counter < 0) {
        return true;
      }
    }
  }

  // We never got a negative count, so we will keep this segment alive.
  return false;
}

static bool cwk_path_segment_will_be_removed(
  const struct cwk_segment_joined *pj)
{
  enum cwk_segment_type type;
  struct cwk_segment_joined pjc;

  // We copy the joined path so we don't need to modify it.
  pjc = *pj;

  // First we check whether this is a CWK_CURRENT or CWK_BACK segment, since
  // those will always be dropped.
  type = cwk_path_get_segment_type(&pj->segment);
  if (type == CWK_CURRENT) {
    return true;
  } else if (type == CWK_BACK) {
    return cwk_path_segment_back_will_be_removed(&pjc);
  } else {
    return cwk_path_segment_normal_will_be_removed(&pjc);
  }
}

static void cwk_path_get_root_windows(const char *path, size_t *length)
{
  const char *c;

  // We can not determine the root if this is an empty string. So we set the
  // root to NULL and the length to zero and cancel the whole thing.
  c = path;
  *length = 0;
  if (!*c) {
    return;
  }

  // Now we have to verify whether this is a windows network path (UNC), which
  // we will consider our root.
  if (cwk_path_is_separator(c)) {
    ++c;

    // Check whether the path starts with a single back slash, which means this
    // is not a network path - just a normal path starting with a backslash.
    if (!cwk_path_is_separator(c)) {
      // Okay, this is not a network path but we still use the backslash as a
      // root.
      ++(*length);
      return;
    }

    // Yes, this is a network path. Skip the previous separator. We will grab
    // anything up to the next stop. The next top might be a '\0' or another
    // separator.
    ++c;
    c = cwk_path_find_next_stop(c);

    // If this is a separator and not the end of a string we wil have to include
    // it. However, if this is a '\0' we must not skip it.
    if (cwk_path_is_separator(c)) {
      ++c;
    }

    // Finally, calculate the size of the root.
    *length = c - path;
    return;
  }

  // Move to the next and check whether this is a colon.
  if (*++c == ':') {
    *length = 2;
  }

  // Now check whether this is a backslash (or slash). If it is not, we could
  // assume that the next character is a '\0' if it is a valid path. However,
  // we will not assume that - since ':' is not valid in a path it must be a
  // mistake by the caller than. We will try to understand it anyway.
  if (cwk_path_is_separator(++c)) {
    *length = 3;
  }
}

static void cwk_path_get_root_unix(const char *path, size_t *length)
{
  // The slash of the unix path represents the root. There is no root if there
  // is no slash.
  if (cwk_path_is_separator(path)) {
    *length = 1;
  } else {
    *length = 0;
  }
}

void cwk_path_get_root(const char *path, size_t *length)
{
  // We use a different implementation here based on the configuration of the
  // library.
  if (path_style == CWK_STYLE_WINDOWS) {
    cwk_path_get_root_windows(path, length);
  } else {
    cwk_path_get_root_unix(path, length);
  }
}

bool cwk_path_is_absolute(const char *path)
{
  size_t length;

  // We grab the root of the path. This root does not include the first
  // separator of a path.
  cwk_path_get_root(path, &length);

  // This is definitely no root if there is no root.
  if (length == 0) {
    return false;
  }

  // If there is a separator at the end of the root, we can safely consider this
  // to be an absolute path.
  return cwk_path_is_separator(&path[length - 1]);
}

bool cwk_path_is_relative(const char *path)
{
  // The path is relative if it is not absolute.
  return !cwk_path_is_absolute(path);
}

void cwk_path_get_basename(const char *path, const char **basename,
  size_t *length)
{
  struct cwk_segment segment;

  // We get the last segment of the path. The last segment will contain the
  // basename if there is any. If there are no segments we will set the basename
  // to NULL and the length to 0.
  if (!cwk_path_get_last_segment(path, &segment)) {
    *basename = NULL;
    *length = 0;
    return;
  }

  // Now we can just output the segment contents, since that's our basename.
  // There might be trailing separators after the basename, but the size does
  // not include those.
  *basename = segment.begin;
  *length = segment.size;
}

void cwk_path_get_dirname(const char *path, size_t *length)
{
  struct cwk_segment segment;

  // We get the last segment of the path. The last segment will contain the
  // basename if there is any. If there are no segments we will set the length
  // to 0.
  if (!cwk_path_get_last_segment(path, &segment)) {
    *length = 0;
    return;
  }

  // We can now return the length from the beginning of the string up to the
  // beginning of the last segment.
  *length = segment.begin - path;
}

bool cwk_path_get_extension(const char *path, const char **extension,
  size_t *length)
{
  struct cwk_segment segment;
  const char *c;

  // We get the last segment of the path. The last segment will contain the
  // extension if there is any.
  if (!cwk_path_get_last_segment(path, &segment)) {
    return false;
  }

  // Now we search for a dot within the segment. If there is a dot, we consider
  // the rest of the segment the extension. We do this from the end towards the
  // beginning, since we want to find the last dot.
  for (c = segment.end; c >= segment.begin; --c) {
    if (*c == '.') {
      // Okay, we found an extension. We can stop looking now.
      *extension = c;
      *length = segment.end - c;
      return true;
    }
  }

  // We couldn't find any extension.
  return false;
}

bool cwk_path_has_extension(const char *path)
{
  const char *extension;
  size_t length;

  // We just wrap the get_extension call which will then do the work for us.
  return cwk_path_get_extension(path, &extension, &length);
}

size_t cwk_path_normalize(const char *path, char *buffer, size_t buffer_size)
{
  size_t pos;
  struct cwk_segment_joined sj;
  const char *paths[2];

  // We initialize the position after the root, which should get us started.
  cwk_path_get_root(path, &pos);

  // First copy the root to the output. We will not modify the root.
  cwk_path_output_sized(buffer, buffer_size, 0, path, pos);

  // Now we initialize the paths which we will normalize. Since this function
  // only supports submitting a single path, we will only add that one.
  paths[0] = path;
  paths[1] = NULL;

  // So we just grab the first segment. If there is no segment we will always
  // output a "/", since we currently only support absolute paths here.
  if (!cwk_path_get_first_segment_joined(paths, &sj)) {
    return pos;
  }

  do {
    // Check whether we have to drop this segment because of resolving a
    // relative path or because it is a CWK_CURRENT segment.
    if (cwk_path_segment_will_be_removed(&sj)) {
      continue;
    }

    // Write out the segment but keep in mind that we need to follow the
    // buffer size limitations. That's why we use the path output functions
    // here.
    pos += cwk_path_output_sized(buffer, buffer_size, pos, sj.segment.begin,
      sj.segment.size);
    pos += cwk_path_output_separator(buffer, buffer_size, pos);
  } while (cwk_path_get_next_segment_joined(&sj));

  // Remove the trailing slash. At this point we must have at least one slash
  // from a segment, which we will remove.
  --pos;

  // We must append a '\0' in any case, unless the buffer size is zero. If the
  // buffer size is zero, which means we can not.
  if (buffer_size > 0) {
    if (pos >= buffer_size) {
      buffer[buffer_size - 1] = '\0';
    } else {
      buffer[pos] = '\0';
    }
  }

  // And finally let our caller know about the total size of the normalized
  // path.
  return pos;
}

size_t cwk_path_get_intersection(const char *path_base, const char *path_other)
{
  const char *end;
  struct cwk_segment base, other;

  // So we get the first segment of both paths. If one of those paths don't have
  // any segment, we will return 0.
  if (!cwk_path_get_first_segment(path_base, &base) ||
      !cwk_path_get_first_segment(path_other, &other)) {
    return 0;
  }

  // We must keep track of the end of the previous segment. Initially, this is
  // set to the beginning of the path. This means that 0 is returned if the
  // first segment is not equal.
  end = path_base;

  // Now we loop over both segments until one of them reaches the end or their
  // contents are not equal.
  do {
    if (strncmp(base.begin, other.begin, base.size) != 0) {
      // So the content of those two segments are not equal. We will return the
      // size up to the beginning.
      return end - path_base;
    }

    // Remember the end of the previous segment before we go to the next one.
    end = base.end;
  } while (cwk_path_get_next_segment(&base) &&
           cwk_path_get_next_segment(&other));

  return end - path_base;
}

bool cwk_path_get_first_segment(const char *path, struct cwk_segment *segment)
{
  // Let's remember the path. We will move the path pointer afterwards, that's
  // why this has to be done first.
  segment->path = path;

  // TODO skip root

  // Now let's check whether this is an empty string. An empty string has no
  // segment it could use.
  if (*path == '\0') {
    return false;
  }

  // If the string starts with separators, we will jump over those. If there is
  // only a slash and a '\0' after it, we can't determine the first segment
  // since there is none.
  while (cwk_path_is_separator(path)) {
    ++path;
    if (*path == '\0') {
      return false;
    }
  }

  // So this is the beginning of our segment.
  segment->begin = path;

  // Now let's determine the end of the segment, which we do by moving the path
  // pointer further until we find a separator.
  path = cwk_path_find_next_stop(path);

  // And finally, calculate the size of the segment by subtracting the position
  // from the end.
  segment->size = path - segment->begin;
  segment->end = path;

  // Tell the caller that we found a segment.
  return true;
}

bool cwk_path_get_last_segment(const char *path, struct cwk_segment *segment)
{
  // We first grab the first segment. This might be our last segment as well,
  // but we don't know yet. There is no last segment if there is no first
  // segment, so we return false in that case.
  if (!cwk_path_get_first_segment(path, segment)) {
    return false;
  }

  // Now we find our last segment. The segment struct of the caller
  // will contain the last segment, since the function we call here will not
  // change the segment struct when it reaches the end.
  while (cwk_path_get_next_segment(segment)) {
    // We just loop until there is no other segment left.
  }

  return true;
}

bool cwk_path_get_next_segment(struct cwk_segment *segment)
{
  const char *c;

  // First we jump to the end of the previous segment. The first character must
  // be either a '\0' or a separator.
  c = segment->begin + segment->size;
  if (*c == '\0') {
    return false;
  }

  // Now we skip all separator until we reach something else. We are not yet
  // guaranteed to have a segment, since the string could just end afterwards.
  assert(cwk_path_is_separator(c));
  do {
    ++c;
  } while (cwk_path_is_separator(c));

  // If the string ends here, we can safely assume that there is no other
  // segment after this one.
  if (*c == '\0') {
    return false;
  }

  // Now we are safe to assume there is a segment. We store the beginning of
  // this segment in the segment struct of the caller.
  segment->begin = c;

  // And now determine the size of this segment, and store it in the struct of
  // the caller as well.
  c = cwk_path_find_next_stop(c);
  segment->end = c;
  segment->size = c - segment->begin;

  // Tell the caller that we found a segment.
  return true;
}
bool cwk_path_get_previous_segment(struct cwk_segment *segment)
{
  const char *c;

  // The current position might point to the first character of the path, which
  // means there are no previous segments available.
  c = segment->begin;
  if (c <= segment->path) {
    return false;
  }

  // We move towards the beginning of the path until we either reached the
  // beginning or the character is no separator anymore.
  do {
    --c;
    if (c <= segment->path) {
      // So we reached the beginning here and there is no segment. So we return
      // false and don't change the segment structure submitted by the caller.
      return false;
    }
  } while (cwk_path_is_separator(c));

  // We are guaranteed now that there is another segment, since we moved before
  // the previous separator and did not reach the segment path beginning.
  segment->end = c + 1;
  segment->begin = cwk_path_find_previous_stop(segment->path, c) + 1;
  segment->size = segment->end - segment->begin;

  return true;
}

enum cwk_segment_type cwk_path_get_segment_type(
  const struct cwk_segment *segment)
{
  // We just make a string comparison with the segment contents and return the
  // appropriate type.
  if (strncmp(segment->begin, ".", segment->size) == 0) {
    return CWK_CURRENT;
  } else if (strncmp(segment->begin, "..", segment->size) == 0) {
    return CWK_BACK;
  }

  return CWK_NORMAL;
}

bool cwk_path_is_separator(const char *str)
{
  const char *c;

  // We loop over all characetrs in the read symbols.
  c = separators[path_style];
  while (*c) {
    if (*c == *str) {
      return true;
    }

    ++c;
  }

  return false;
}

void cwk_path_set_style(enum cwk_path_style style)
{
  // We can just set the global path style variable and then the behaviour for
  // all functions will change accordingly.
  assert(style == CWK_STYLE_UNIX || style == CWK_STYLE_WINDOWS);
  path_style = style;
}

enum cwk_path_style cwk_path_get_style(void)
{
  // Simply return the path style which we store in a global variable.
  return path_style;
}
