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

void load_elements();
#endif
