#ifndef BST_IDE_UTILS
#define BST_IDE_UTILS

#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

Color u32_to_color(uint32_t i);

#define dyn_append(arr, v)                                                  \
  do {                                                                      \
    if (arr.count >= arr.capacity) {                                        \
      arr.capacity *= 2;                                                    \
      arr.values = realloc(arr.values, sizeof(*arr.values) * arr.capacity); \
    }                                                                       \
    arr.values[arr.count++] = v;                                            \
  } while (0)

#define dyn_init_default(arr)                                \
  do {                                                       \
    arr.count = 0;                                           \
    arr.capacity = 512 / sizeof(*arr.values);                \
    arr.values = malloc(arr.capacity * sizeof(*arr.values)); \
  } while (0)

#define dyn_init_N(arr, N)                                   \
  do {                                                       \
    arr.count = 0;                                           \
    arr.capacity = N;                                        \
    arr.values = malloc(arr.capacity * sizeof(*arr.values)); \
  } while (0)

#define dyn_rm(arr, index)                        \
  do {                                            \
    for (int i = index; i < arr.count - 1; i++) { \
      arr.values[i] = arr.values[i + 1];          \
    }                                             \
    arr.count--;                                  \
  } while (0)

#define _dyn_init_get(_1, _2, NAME, ...) NAME
#define dyn_init(...) \
  _dyn_init_get(__VA_ARGS__, dyn_init_N, dyn_init_default)(__VA_ARGS__)

#define dyn_clear(arr) \
  do {                 \
    free(arr.values);  \
    dyn_init(arr);     \
  } while (0)

#define dyn_copy_from(arr, N, buff)                     \
  do {                                                  \
    arr.count = N;                                      \
    memcopy(arr.values, buff, N * sizeof(*arr.values)); \
  } while (0)

typedef struct {
  size_t count;
  size_t capacity;
  char* values;
} DynStr;

#endif
