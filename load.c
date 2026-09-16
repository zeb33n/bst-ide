#include <dirent.h>
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>
#include "load.h"
#include "utils.h"

Element* ELEMENTS = NULL;

char** get_element_paths_dynalloc(const char* dir_path) {
  char** out = NULL;

  char* elements_path = string_format_dynalloc("%s/elements/", PROJECT_PATH);
  dynstring_concat(elements_path, dir_path);

  DIR* d = NULL;
  if ((d = opendir(elements_path)) == NULL) {
    printf("%s\n", elements_path);
    perror(ERROR_MSG);
    exit(1);
  }

  struct dirent* entry;
  while ((entry = readdir(d)) != NULL) {
    char* element_path = NULL;
    dyn_init_if_null(element_path);
    dynstring_concat(element_path, dir_path);
    dynstring_concat(element_path, "/");
    dynstring_concat(element_path, entry->d_name);

    if (entry->d_type == DT_REG && string_ends_with(entry->d_name, ".bst")) {
      dyn_append(out, element_path);
    }

    if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 &&
        strcmp(entry->d_name, "..") != 0) {
      char** element_paths = get_element_paths_dynalloc(element_path);
      dyn_free(element_path);
      if (element_paths == NULL) {
        continue;
      }
      for
        dyn_iter(element_paths, i) {
          dyn_append(out, element_paths[i]);
        }
      dyn_free(element_paths);
    }
  }

  closedir(d);
  dyn_free(elements_path);
  return out;
}

size_t elements_insert_and_get_index(const char* path) {
  for
    dyn_iter(ELEMENTS, i) {
      if (strcmp(ELEMENTS[i].path, path) == 0) {
        return i;
      }
    }
  Element e;
  e.path = path;
  e.build_dep_handles = NULL;
  e.run_dep_handles = NULL;
  e.dep_handles = NULL;
  dyn_append(ELEMENTS, e);
  return dyn_len(ELEMENTS) - 1;
}

typedef enum {
  BUILD_DEPENDS,
  RUN_DEPENDS,
  DEPENDS,
  NONE,
} BstKey;

void load_element(const char* bst_path) {
  char* path = string_format_dynalloc("%s/elements/", PROJECT_PATH);
  dynstring_concat(path, bst_path);

  FILE* f = fopen(path, "r");

  if (f == NULL) {
    perror(ERROR_MSG);
    exit(1);
  }

  yaml_parser_t parser;
  yaml_event_t event;

  if (!yaml_parser_initialize(&parser)) {
    fclose(f);
    exit(1);
  }

  yaml_parser_set_input_file(&parser, f);

  // remove the ./ at the start of the path string
  size_t handle = elements_insert_and_get_index(bst_path + 2);
  bool done = false;
  bool mapping = false;
  BstKey current_key = NONE;
  char* value;

  while (!done) {
    if (!yaml_parser_parse(&parser, &event)) {
      printf("%s YAML parse error", ERROR_MSG);
      exit(1);
    }

    switch (event.type) {
      case YAML_SCALAR_EVENT:
        if (mapping)
          break;
        value = (char*)event.data.scalar.value;

        // skip buildstream yaml directives
        if (strcmp(value, "(>)") == 0)
          break;

        if (current_key != NONE) {
          size_t dep_handle = elements_insert_and_get_index(value);
          // TODO append based on on current_key
          dyn_append(ELEMENTS[handle].build_dep_handles, dep_handle);
        }
        if (strcmp(value, "depends") == 0) {
          current_key = DEPENDS;
        }
        if (strcmp(value, "build-depends") == 0) {
          current_key = BUILD_DEPENDS;
        }
        if (strcmp(value, "runtime-depends") == 0) {
          current_key = RUN_DEPENDS;
        }
        break;
      // TODO actually parse filename: tags
      case YAML_MAPPING_START_EVENT:
        if (current_key != NONE) {
          mapping = true;
        }
        break;
      case YAML_MAPPING_END_EVENT:
        if (current_key != NONE) {
          mapping = false;
        }
        break;
      case YAML_SEQUENCE_END_EVENT:
        current_key = NONE;
        break;
      case YAML_STREAM_END_EVENT:
        done = true;
        break;
      default:
        break;
        yaml_event_delete(&event);
    }
  }
  yaml_parser_delete(&parser);
  fclose(f);
}

void load_elements() {
  dyn_init_if_null(ELEMENTS);
  char** element_paths = get_element_paths_dynalloc(".");
  for
    dyn_iter(element_paths, i) {
      load_element(element_paths[i]);
    }
  dyn_free(element_paths);
}

// TODO there is a bug in here somewhere
// looks like dependencies aren't what they should be
char* get_dot_alloc() {
  GVC_t* gvc = gvContext();
  Agraph_t* graph = agopen(0, Agdirected, 0);
  agsafeset(graph, "rankdir", "LR", "");
  agsafeset(graph, "splines", "polyline", "");
  Agnode_t** nodes = NULL;
  for
    dyn_iter(ELEMENTS, i) {
      dyn_append(nodes, agnode(graph, (char*)ELEMENTS[i].path, 1));
    }
  for
    dyn_iter(ELEMENTS, parent_handle) {
      size_t* child_handles = ELEMENTS[parent_handle].build_dep_handles;
      if (child_handles == NULL)
        continue;
    for
      dyn_iter(child_handles, j) {
        (void)agedge(graph, nodes[parent_handle], nodes[child_handles[j]], 0, 1);
      }
    }

  gvLayout(gvc, graph, "dot");
  size_t l;
  char* out;
  gvRenderData(gvc, graph, "dot", &out, &l);

  dyn_free(nodes);
  gvFreeLayout(gvc, graph);
  agclose(graph);
  
  return out;
}
