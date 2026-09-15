#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

bool string_ends_with(const char* s1, const char* s2) {
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);
  if (len1 < len2) {
    return false;
  }
  size_t offset = len1 - len2;
  s1 += offset;
  return strcmp(s1, s2) == 0;
}

void dynstring_concat(char* dyn, const char* s) {
  size_t dyn_len = strlen(dyn);
  dyn_rm(dyn, dyn_len);
  for (size_t i = 0; i <= strlen(s); i++) {
    dyn_append(dyn, s[i]);
  }
}

char* string_format_dynalloc(const char* templ, const char* s) {
  char* out = NULL;
  // not precise but close enough for our purposes
  size_t n = strlen(templ) + strlen(s) + 1;
  dyn_set_capacity(out, n);
  sprintf(out, templ, s);
  ((ArrayData*)out - 1)->count = strlen(out) + 1;
  return out;
}

Color u32_to_color(uint32_t i) {
  Color c;
  i = __builtin_bswap32(i);
  memcpy(&c, &i, sizeof(Color));
  return c;
}
