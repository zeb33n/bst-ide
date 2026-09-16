#ifndef BST_IDE_LOAD
#define BST_IDE_LOAD

#include <stddef.h>

typedef struct Element {
  const char* path;
  size_t* dep_handles;
  size_t* run_dep_handles;
  size_t* build_dep_handles;
} Element;

extern char* PROJECT_PATH;

void load_elements();
#endif
