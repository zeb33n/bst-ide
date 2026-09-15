#ifndef BST_IDE_UTILS
#define BST_IDE_UTILS

#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

Color u32_to_color(uint32_t i);

typedef struct {
  size_t count;
  size_t capacity;
} ArrayData;

#define dyn_init_if_null(arr)                                                \
  do {                                                                       \
    if (arr == NULL) {                                                       \
      size_t capacity = 512 / sizeof(*arr);                                  \
      ArrayData* data = malloc(capacity * sizeof(*arr) + sizeof(ArrayData)); \
      data->count = 0;                                                       \
      data->capacity = capacity;                                             \
      arr = (void*)(data + 1);                                               \
    }                                                                        \
  } while (0)

#define dyn_append(arr, v)                                                     \
  do {                                                                         \
    dyn_init_if_null(arr);                                                     \
    ArrayData* data = (ArrayData*)arr - 1;                                     \
    if (data->count >= data->capacity) {                                       \
      data->capacity *= 2;                                                     \
      data = realloc(data, sizeof(*arr) * data->capacity + sizeof(ArrayData)); \
      arr = (void*)(data + 1);                                                 \
    }                                                                          \
    arr[data->count++] = v;                                                    \
  } while (0)

#define dyn_rm(arr, index)                          \
  do {                                              \
    ArrayData* data = (ArrayData*)arr - 1;          \
    for (int i = index; i < data->count - 1; i++) { \
      arr[i] = arr[i + 1];                          \
    }                                               \
    data->count--;                                  \
  } while (0)

#define dyn_free(arr)          \
  do {                         \
    free((ArrayData*)arr - 1); \
  } while (0)

#endif
