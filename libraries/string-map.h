#include <stddef.h>

typedef struct {
  size_t size;
  struct bucket **buckets;
} StringMap;

StringMap *create_map(size_t size);

void *get_map_value(StringMap *map, const char *key);

void set_map_value(StringMap *map, const char *key, void *value);