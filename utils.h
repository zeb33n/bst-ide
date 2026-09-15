#ifndef BST_IDE_UTILS
#define BST_IDE_UTILS

#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_BUFF_LEN 1024
#define ERROR_MSG "Error:"

Color u32_to_color(uint32_t i);
char* string_format_dynalloc(const char* templ, const char* s);
char** string_split_dynalloc(const char* str, char c);
bool string_ends_with(const char* s1, const char* s2);
void dynstring_concat(char* dyn, const char* s);

typedef struct {
  size_t count;
  size_t capacity;
} ArrayData;

#define dyn_make(arr, n)                                                   \
  do {                                                                     \
    size_t capacity = n;                                                   \
    ArrayData* data = malloc(capacity * sizeof(*arr) + sizeof(ArrayData)); \
    data->count = 0;                                                       \
    data->capacity = capacity;                                             \
    memcpy(data + 1, arr, n * sizeof(*arr));                               \
    arr = (void*)(data + 1);                                               \
  } while (0)

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

#define dyn_set_capacity(arr, n)                                             \
  do {                                                                       \
    dyn_init_if_null(arr);                                                   \
    ArrayData* data = (ArrayData*)arr - 1;                                   \
    data->capacity = n;                                                      \
    if (data->count > data->capacity) {                                      \
      printf("%s resizing array would cause data loss", ERROR_MSG);          \
      exit(1);                                                               \
    }                                                                        \
    data = realloc(data, sizeof(*arr) * data->capacity + sizeof(ArrayData)); \
    arr = (void*)(data + 1);                                                 \
  } while (0)

#define dyn_rm(arr, index)                          \
  do {                                              \
    ArrayData* data = (ArrayData*)arr - 1;          \
    if (data->count == 0)                           \
      break;                                        \
    for (int i = index; i < data->count - 1; i++) { \
      arr[i] = arr[i + 1];                          \
    }                                               \
    data->count--;                                  \
  } while (0)

#define dyn_free(arr) free((ArrayData*)arr - 1)

#define dyn_iter(arr, i) (size_t i = 0; i < ((ArrayData*)arr - 1)->count; i++)

#endif
