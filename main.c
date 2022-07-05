#include "libraries/string-map.h"
#include <stdio.h>

int main(int argc, const char *argv[]) {
  StringMap *map = create_map(4096);
  for (size_t i = 1; i < argc; i++) {
    set_map_value(map, argv[i], (void *)i);
  }
  for (size_t i = 1; i < argc; i++) {
    size_t val = (size_t)get_map_value(map, argv[i]);
    printf("%s,\t%zu,\t%zu\n", argv[i], i, (size_t)val);
  }
  set_map_value(map, "absent", (void *)"absent");
  size_t val = (size_t)get_map_value(map, "absent");
  printf("%s,\t%s,\t%s\n", "absent", "absent", (char*)val);
}