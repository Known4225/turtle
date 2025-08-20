#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../include/bufferList.h"

bufferList_t* bufferList_init() {
    bufferList_t *list = malloc(sizeof(bufferList_t));
    list -> length = 0;
    list -> realLength = 1;
    list -> data = calloc(1, sizeof(float));
    return list;
}

/* append to list */
void bufferList_append(bufferList_t *list, float data) {
    if (list -> realLength  <= list -> length) {
        list -> realLength *= 2;
        list -> data = realloc(list -> data, list -> realLength * sizeof(float));
    }
    list -> data[list -> length] = data;
    list -> length += 1;
}

void bufferList_clear(bufferList_t *list) {
    bufferList_free_lite(list);
    list -> length = 0;
    list -> realLength = 1;
    list -> data = calloc(1, sizeof(float));
}

/* pops the last item of the list off and returns it */
float bufferList_pop(bufferList_t *list) {
    if (list -> length > 0) {
        list -> length -= 1;
        float ret = list -> data[list -> length];
        list -> data[list -> length] = (float) 0;
        if (list -> length <= list -> realLength / 2 && list -> realLength > 1) {
            list -> realLength /= 2;
            list -> data = realloc(list -> data, list -> realLength * sizeof(float));
        }
        return ret;
    } else {
        return (float) 0;
    }
}

/* deletes the item at list[index] of the list and returns it */
float bufferList_delete(bufferList_t *list, int32_t index) {
    while (index < 0) {index += list -> length;}
    index %= list -> length;
    float ret = list -> data[index];
    for (uint32_t i = index; i < list -> length - 1 ; i++) {
        list -> data[i] = list -> data[i + 1];
    }
    list -> length -= 1;
    list -> data[list -> length] = (float) 0;
    if (list -> length <= list -> realLength / 2 && list -> realLength > 1) {
        list -> realLength /= 2;
        list -> data = realloc(list -> data, list -> realLength * sizeof(float));
    }
    return ret;
}

/* deletes many items from the list spanning from [indexMin] to [indexMax - 1] */
void bufferList_delete_range(bufferList_t* list, uint32_t indexMin, uint32_t indexMax) {
    if (indexMin > indexMax) {
        uint32_t swap = indexMin;
        indexMin = indexMax;
        indexMax = swap;
    }
    char zerod = 0; // edge case: "should've used list_clear"
    uint32_t difference = (indexMax - indexMin);
    list -> realLength = list -> length - difference;
    if (list -> realLength <= 1) {
        zerod = 1;
        list -> realLength = 1;
    }
    
    float *newData = malloc(list -> realLength * sizeof(float)); // no need to calloc we're gonna fill it all up anyway
    for (uint32_t i = 0; i < indexMin; i++) {
        newData[i] = list -> data[i];
    }
    for (uint32_t i = indexMax; i < list -> length; i++) {
        newData[i - difference] = list -> data[i];
    }
    list -> length = list -> realLength;
    if (zerod)
        list -> length = 0;
    free(list -> data);
    list -> data = newData;
}

/* returns the index of the first instance of the item in the list, returns -1 if not found (python) */
int32_t bufferList_find(bufferList_t *list, float item) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            return i;
        }
    }
    return -1;
}

/* duplicate of list_find */
int32_t bufferList_index(bufferList_t *list, float item) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            return i;
        }
    }
    return -1;
}

/* counts how many instances of an item is found in the list */
uint32_t bufferList_count(bufferList_t *list, float item) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < list -> length; i++) {
        count += (list -> data[i] == item);
    }
    return count;
}

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found (python but without ValueError) */
int32_t bufferList_remove(bufferList_t *list, float item) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            bufferList_delete(list, i);
            return i;
        }
    }
    return -1;
}

/* copies one list to another */
void bufferList_copy(bufferList_t *dest, bufferList_t *src) {
    bufferList_free_lite(dest);
    dest -> data = calloc(src -> realLength, sizeof(float));
    uint32_t len = src -> length;
    dest -> length = len;
    dest -> realLength = src -> realLength;
    for (uint32_t i = 0; i < len; i++) {
        dest -> data[i] = src -> data[i];
    }
}

/* prints the list (like python would) */
void bufferList_print(bufferList_t *list) {
    printf("[");
    if (list -> length == 0) {
        printf("]\n");
        return;
    }
    for (uint32_t i = 0; i < list -> length; i++) {
        printf("%f", list -> data[i]);
        if (i == list -> length - 1) {
            printf("]\n");
        } else {
            printf(", ");
        }
    }
}

/* prints the list but without closing \n */
void bufferList_print_emb(bufferList_t *list) {
    printf("[");
    if (list -> length == 0) {
        printf("]");
        return;
    }
    for (uint32_t i = 0; i < list -> length; i++) {
        printf("%f", list -> data[i]);
        if (i == list -> length - 1) {
            printf("]");
        } else {
            printf(", ");
        }
    }
}

/* frees the list's data but not the list itself */
void bufferList_free_lite(bufferList_t *list) {
    free(list -> data);
}

/* frees the data used by the list */
void bufferList_free(bufferList_t *list) {
    bufferList_free_lite(list);
    free(list);
}