#include "map.h"

void map_fprintf_IntString(FILE* stream, Map* map) {
      for (int i= 0; i < list_len(&map->keys); i++) {
            char** key= (char**) list_getPtr(&map->keys, i);
            int* value= (int*) list_getPtr(&map->values, i);
            fprintf(stream, "%s: %d\n", *key, *value);
      }
}

// --> Initializes and deletes the array and array information
Map* newMap(int keySize, int valueSize, int maxThings) {
      Map* map= (Map*) malloc(sizeof(Map));
      initList(&map->keys, keySize, maxThings);
      initList(&map->values, valueSize, maxThings);
      return map;
}
void initMap(Map* map, int keySize, int valueSize, int maxThings) {
      initList(&map->keys, keySize, maxThings);
      initList(&map->values, valueSize, maxThings);
      return;
}
void deleteMap(Map* map) {
      closeList(&map->keys);
      closeList(&map->values);
      free(map);
      return;
}
void closeMap(Map* map) {
      closeList(&map->keys);
      closeList(&map->values);
      return;
}

// --> Adds and removes elements from the map
void map_add(Map* map, const void* key, void* value) {
      list_push(&map->keys, key);
      list_push(&map->values, value);
      return;
}
void map_remove(Map* map, const void* key, void* output) {
      for (int i= 0; i < list_len(&map->keys); i++) {
            if (memcmp(key, list_getPtr(&map->keys, i), list_elementSize(&map->keys)) == 0) {
                  list_remove(&map->keys, i, NULL);
                  list_remove(&map->values, i, output);
                  return;
            }
      }
      return;
}

// --> Reads values without removing them
void map_get(Map* map, const void* key, void* output) {
      for (int i= 0; i < list_len(&map->keys); i++) {
            if (memcmp(key, list_getPtr(&map->keys, i), list_elementSize(&map->keys)) == 0) {
                  list_get(&map->values, i, output);
                  return;
            }
      }
      return;
}
void* map_getPtr(Map* map, const void* key) {
      for (int i= 0; i < list_len(&map->keys); i++) {
            if (memcmp(key, list_getPtr(&map->keys, i), list_elementSize(&map->keys)) == 0) {
                  return list_getPtr(&map->values, i);
            }
      }
      return NULL;
}

// --> Returns attributes of the map
int map_keySize(Map* map) {
      return list_len(&map->keys);
}
int map_valueSize(Map* map) {
      return list_len(&map->values);
}
int map_maxLen(Map* map) {
      return list_maxLen(&map->keys);
}
int map_len(Map* map) {
      return list_len(&map->keys);
}
