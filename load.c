#include <stdint.h>
#include <stdio.h>
#include "load.h"
#include "utils.h"
#include <glob.h>


Element* load_element_alloc(const char* path) {
  char* out_buff = NULL;
  
  char cmd_buff[256];
  sprintf(cmd_buff, "bst show --deps none --format \%{deps} %s", path);
  FILE* stream = popen(cmd_buff, "r");
  if (stream == NULL) {
    perror("");
  }

  while (1) {
    int32_t c = fgetc(stream);
    if (c == EOF) {
      break;
    }
    dyn_append(out_buff, (char)c);
  }
  dyn_append(out_buff, '\0');

  printf("%s\n", out_buff);
}
