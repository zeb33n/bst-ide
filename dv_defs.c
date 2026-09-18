#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <raylib.h>

void js_fill_circ(float x, float y, float radius, uint32_t c) {
  DrawCircle((int32_t)x, (int32_t)y, radius, u32_to_color(c));
}

void js_follow_link() {}

void js_log(const uint8_t* ptr, size_t len) {
  printf("%s\n", ptr);
}

void js_fill_rect(float x, float y, float w, float h, uint32_t c) {
  DrawRectangle((int32_t)x, (int32_t)y, (int32_t)w, (int32_t)h,
                u32_to_color(c));
}

void js_fill_line(float x1,
                  float y1,
                  float x2,
                  float y2,
                  uint32_t c,
                  float width) {
  DrawLine((int32_t)x1, (int32_t)y1, (int32_t)x2, (int32_t)y2, u32_to_color(c));
}

void js_fill_string(float x,
                    float y,
                    char* s,
                    size_t len,
                    uint32_t c,
                    float size) {
  char* buff = calloc(len, 1);
  memcpy(buff, s, len);
  DrawText(buff, (int32_t)x, (int32_t)y, (int32_t)size, u32_to_color(c));
  free(buff);
}
