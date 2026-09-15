#ifndef BST_IDE_LOAD
#define BST_IDE_LOAD

typedef struct Element {
  char* name;
  struct Element** deps;
} Element;

typedef struct {
  Element* next;
  Element* prev;
} Elements;

extern char* PROJECT_PATH;

Element* load_element_alloc(const char* path);
char** get_element_paths_dynalloc(const char* dir_path);
#endif
