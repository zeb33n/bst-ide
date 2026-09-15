#include <dirent.h>
#include <glob.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "load.h"
#include "utils.h"

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
      for dyn_iter(element_paths, i) {
        dyn_append(out, element_paths[i]);
      }
      dyn_free(element_paths);
    }
  }

  closedir(d);
  dyn_free(elements_path);
  return out;
}

// TODO this takes a while to run we should cache this somehow.
// or just parse the yaml directly? 
void load_element_alloc(const char* path) {
  char* cmd =
      string_format_dynalloc("bst show --deps none --format \%{deps} %s", path);
  FILE* stream = popen(cmd, "r");
  if (stream == NULL) {
    perror(ERROR_MSG);
    exit(1);
  }
  dyn_free(cmd);

  int32_t c;
  char* stdout = NULL;
  while ((c = fgetc(stream)) != EOF) {
    dyn_append(stdout, (char)c);
  }
  dyn_append(stdout, '\0');
  char** dependencies = string_split_dynalloc(stdout, '\n');
  if (dependencies == NULL) {
    return;
  }
  for dyn_iter(dependencies, i) {
    printf("%s\n", dependencies[i]);
  }
}

void load_elements() {
  char** element_paths = get_element_paths_dynalloc(".");
  for dyn_iter(element_paths, i) {
    load_element_alloc(element_paths[i]);
  }
  dyn_free(element_paths);
}
