#include <string.h>
#include <raylib.h>
#include <stdint.h>

Color u32_to_color(uint32_t i) {
  Color c;
  i = __builtin_bswap32(i);
  memcpy(&c, &i, sizeof(Color));
  return c;
}
