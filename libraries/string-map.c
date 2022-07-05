#include "string-hasher.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKET_NULL (void *)0

/* purposely collide all hashes for testing purposes
size_t hash(const char *s) {
    return 1;
}
//*/

struct bucket {
  const char *key;
  void *value;
  struct bucket *next_node;
};

typedef struct {
  size_t size;
  struct bucket **buckets;
} StringMap;

StringMap *create_map(size_t size) {
  StringMap *map = malloc(sizeof(StringMap));

  map->size = size;
  map->buckets = calloc(size, sizeof(size_t));

  return map;
};

void *get_map_value(StringMap *map, const char *key) {
  size_t kh = hash(key);
  struct bucket *b = map->buckets[kh % map->size];

  while ((void *)b != BUCKET_NULL) {
    if (strcmp(key, b->key) == 0) {
      return b->value;
    } else {
      b = b->next_node;
    }
  }

  return BUCKET_NULL;
}

void set_map_value(StringMap *map, const char *key, void *value) {
  size_t kh = hash(key);
  struct bucket *b = map->buckets[kh % map->size];

  if ((void *)b == BUCKET_NULL) {
    struct bucket *nb = malloc(sizeof(struct bucket));
    nb->key = key;
    nb->value = value;
    nb->next_node = BUCKET_NULL;
    map->buckets[kh % map->size] = nb;
    return;
  }

  while ((void *)b->next_node != BUCKET_NULL) {
    if (strcmp(key, b->key) == 0) {
      b->value = value;
      return;
    } else {
      b = b->next_node;
    }
  }

  if (strcmp(key, b->key) == 0) {
    b->value = value;
  } else {
    struct bucket *nb = malloc(sizeof(struct bucket));
    nb->key = key;
    nb->value = value;
    nb->next_node = BUCKET_NULL;
    b->next_node = nb;
  }
}