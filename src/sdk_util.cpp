#include "precompiled.h"

char *UTIL_VarArgs(const char *format, ...) {
  va_list argptr;
  static char string[1024];

  va_start(argptr, format);
  _vsnprintf(string, sizeof(string), format, argptr);
  va_end(argptr);

  return string;
}

void StringReplace(char *source_str, const char *dest, const char *result) {
  if (!result) return;

  auto match_ptr = source_str;
  const auto dest_len = std::strlen(dest), result_len = std::strlen(result);
  while ((match_ptr = std::strstr(match_ptr, dest))) {
    if (dest_len != result_len) {
      std::memmove(match_ptr + result_len, match_ptr + dest_len,
                   std::strlen(match_ptr) - dest_len + 1);
    }
    std::memcpy(match_ptr, result, result_len);
    match_ptr += result_len;
  }
}

void NormalizePath(char *path) {
  for (char *current_char = path; *current_char; current_char++) {
    if (std::isupper(*current_char))
      *current_char = std::tolower(*current_char);
    if (*current_char == '\\') *current_char = '/';
  }
}
