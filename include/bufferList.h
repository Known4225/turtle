#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/*
28.03.24:
bufferList - like a normal list but only supports floats
*/

typedef struct {
    unsigned int length;
    unsigned int realLength;
    float *data;
} bufferList_t;

bufferList_t* bufferList_init() { // initialise a list
    bufferList_t *list = malloc(sizeof(bufferList_t));
    list -> length = 0;
    list -> realLength = 1;
    list -> data = calloc(1, sizeof(float));
    return list;
}

void bufferList_free_lite(bufferList_t *);
void bufferList_free(bufferList_t *);
void bufferList_print_emb(bufferList_t *);

void bufferList_append(bufferList_t *list, float data) { // append to list
    // printf("append start\n");
    if (list -> realLength  <= list -> length) {
        list -> realLength *= 2;
        list -> data = realloc(list -> data, list -> realLength * sizeof(float));
    }
    list -> data[list -> length] = data;
    list -> length += 1;
    // printf("append end\n");
}

void bufferList_clear(bufferList_t *list) {
    bufferList_free_lite(list);
    list -> length = 0;
    list -> realLength = 1;
    list -> data = calloc(1, sizeof(float));
}

float bufferList_pop(bufferList_t *list) { // pops the last item of the list off and returns it
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

float bufferList_delete(bufferList_t *list, int index) { // deletes the item at list[index] of the list and returns it
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

void bufferList_delete_range(bufferList_t* list, int indexMin, int indexMax) { // deletes many items from the list spanning from [indexMin] to [indexMax - 1]
    if (indexMin > indexMax) {
        int swap = indexMin;
        indexMin = indexMax;
        indexMax = swap;
    }
    char zerod = 0; // edge case: "should've used list_clear"
    int difference = (indexMax - indexMin);
    list -> realLength = list -> length - difference;
    if (list -> realLength <= 1) {
        zerod = 1;
        list -> realLength = 1;
    }
    
    float *newData = malloc(list -> realLength * sizeof(float)); // no need to calloc we're gonna fill it all up anyway
    for (int i = 0; i < indexMin; i++) {
        newData[i] = list -> data[i];
    }
    for (int i = indexMax; i < list -> length; i++) {
        newData[i - difference] = list -> data[i];
    }
    list -> length = list -> realLength;
    if (zerod)
        list -> length = 0;
    free(list -> data);
    list -> data = newData;
}

int bufferList_find(bufferList_t *list, float item) { // returns the index of the first instance of the item in the list, returns -1 if not found (python)
    for (int i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            return i;
        }
    }
    return -1;
}

int bufferList_index(bufferList_t *list, float item) { // duplicate of list_find
    for (int i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            return i;
        }
    }
    return -1;
}

int bufferList_count(bufferList_t *list, float item) { // counts how many instances of an item is found in the list
    int count = 0;
    for (int i = 0; i < list -> length; i++) {
        count += (list -> data[i] == item);
    }
    return count;
}

int bufferList_remove(bufferList_t *list, float item) { // deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found (python but without ValueError)
    for (int i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            bufferList_delete(list, i);
            return i;
        }
    }
    return -1;
}

void bufferList_copy(bufferList_t *src, bufferList_t *dest) { // copies one list to another
    bufferList_free_lite(dest);
    dest -> data = calloc(src -> realLength, sizeof(float));
    unsigned int len = src -> length;
    dest -> length = len;
    dest -> realLength = src -> realLength;
    for (int i = 0; i < len; i++) {
        dest -> data[i] = src -> data[i];
    }
}

void bufferList_print(bufferList_t *list) { // prints the list (like python would)
    printf("[");
    if (list -> length == 0) {
        printf("]\n");
        return;
    }
    for (int i = 0; i < list -> length; i++) {
        printf("%f", list -> data[i]);
        if (i == list -> length - 1) {
            printf("]\n");
        } else {
            printf(", ");
        }
    }
}

void bufferList_print_emb(bufferList_t *list) { // prints the list but without closing \n
    printf("[");
    if (list -> length == 0) {
        printf("]");
        return;
    }
    for (int i = 0; i < list -> length; i++) {
        printf("%f", list -> data[i]);
        if (i == list -> length - 1) {
            printf("]");
        } else {
            printf(", ");
        }
    }
}

void bufferList_free_lite(bufferList_t *list) { // frees the list's data but not the list itself
    free(list -> data);
}

void bufferList_free(bufferList_t *list) { // frees the data used by the list
    bufferList_free_lite(list);
    free(list);
}