#include "list.h"

void list_printInt(FILE* stream, List* list) {
      fprintf(stream, "[");
      for (int i= 0; i < list->numThings; i++) {
            int* value= (int*) &list->array[i * list->typeSize];
            fprintf(stream, "%d", *value);
            if (i != list->numThings - 1) {
                  fprintf(stream, ", ");
            }
      }
      fprintf(stream, "]");
}

void list_printString(FILE* stream, List* list) {
      fprintf(stream, "[");
      for (int i= 0; i < list->numThings; i++) {
            char** value= (char**) &list->array[i * list->typeSize];
            fprintf(stream, "%s", *value);
            if (i != list->numThings - 1) {
                  fprintf(stream, ", ");
            }
      }
      fprintf(stream, "]");
}

// --> Initializes and deletes the array and array information
List* newList(int typeSize, int maxThings) {
      List* list= (List*) malloc(sizeof(List));
      list->typeSize= typeSize;
      list->maxThings= maxThings;
      list->numThings= 0;
      list->array= (char*) malloc(list->typeSize * list->maxThings);
      return list;
}
void initList(List* list, int typeSize, int maxThings) {
      list->typeSize= typeSize;
      list->maxThings= maxThings;
      list->numThings= 0;
      list->array= (char*) malloc(list->typeSize * list->maxThings);
      return;
}
void deleteList(List* list) {
      free(list->array);
      free(list);
      return;
}
void closeList(List* list) {
      free(list->array);
      return;
}
// --> Appends and removes elements from the end of the array
void* list_push(List* list, const void* input) {
      if (list->numThings == list->maxThings) {
            list->array= (char*) realloc(list->array, (list->maxThings + 1) * list->typeSize);
            list->maxThings++;
      }
      void* valuePtr= (void*) &list->array[list->numThings * list->typeSize];
      memcpy(valuePtr, input, list->typeSize);
      list->numThings++;
      return valuePtr;
}
void list_pop(List* list, void* output) {
      void* valuePtr= (void*) &list->array[(list->numThings - 1) * list->typeSize];
      if (output != NULL) {
            memcpy(output, valuePtr, list->typeSize);
      }
      list->numThings--;
      if (list->numThings <= list->maxThings / 2) {
            list->array= (char*) realloc(list->array, list->numThings * list->typeSize);
            list->maxThings= list->numThings;
      }
      return;
}
void* list_addElements(List* list, int numElements) {
      if (list->numThings + numElements >= list->maxThings) {
            list->array= (char*) realloc(list->array, (list->maxThings + numElements) * list->typeSize);
            list->maxThings += numElements;
      }
      void* valuePtr= (void*) &list->array[list->numThings * list->typeSize];
      list->numThings += numElements;
      return valuePtr;
}
// --> Inserts and removes elements from the middle of the array
void* list_insert(List* list, int index, const void* input) {
      if (list->numThings == list->maxThings) {
            list->array= (char*) realloc(list->array, (list->maxThings + 1) * list->typeSize);
            list->maxThings++;
      }
      for (int i= list->numThings * list->typeSize - 1; i >= index * list->typeSize; i--) {
            list->array[i + list->typeSize]= list->array[i];
      }
      void* valuePtr= (void*) &list->array[index * list->typeSize];
      memcpy(valuePtr, input, list->typeSize);
      list->numThings++;
      return valuePtr;
}
void list_remove(List* list, int index, void* output) {
      void* valuePtr= (void*) &list->array[index * list->typeSize];
      if (output != NULL) {
            memcpy(output, valuePtr, list->typeSize);
      }
      for (int i= (index + 1) * list->typeSize; i < list->numThings * list->typeSize; i++) {
            list->array[i - list->typeSize]= list->array[i];
      }
      list->numThings--;
      if (list->numThings <= list->maxThings / 2) {
            list->array= (char*) realloc(list->array, list->numThings * list->typeSize);
            list->maxThings= list->numThings;
      }
      return;
}
void* list_insertElements(List* list, int index, int numElements) {
      if (list->numThings + numElements >= list->maxThings) {
            list->array= (char*) realloc(list->array, (list->maxThings + numElements) * list->typeSize);
            list->maxThings += numElements;
      }
      for (int i= list->numThings * list->typeSize - 1; i >= index * list->typeSize; i--) {
            list->array[list->typeSize * numElements + i]= list->array[i];
      }
      void* valuePtr= (void*) &list->array[index * list->typeSize];
      list->numThings += numElements;
      return valuePtr;
}
// --> Reads values without removing them
void list_get(List* list, int index, void* output) {
      void* valuePtr= (void*) &list->array[index * list->typeSize];
      memcpy(output, valuePtr, list->typeSize);
      return;
}
void* list_getPtr(List* list, int index) {
      void* valuePtr= (void*) &list->array[index * list->typeSize];
      return valuePtr;
}
// --> Returns attributes of the list
int list_elementSize(List* list) {
      return list->typeSize;
}
int list_maxLen(List* list) {
      return list->maxThings;
}
int list_len(List* list) {
      return list->numThings;
}
