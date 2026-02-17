/*
██████╗ ██╗   ██╗███████╗███████╗███████╗██████╗ ██╗     ██╗███████╗████████╗██╗  ██╗
██╔══██╗██║   ██║██╔════╝██╔════╝██╔════╝██╔══██╗██║     ██║██╔════╝╚══██╔══╝██║  ██║
██████╔╝██║   ██║█████╗  █████╗  █████╗  ██████╔╝██║     ██║███████╗   ██║   ███████║
██╔══██╗██║   ██║██╔══╝  ██╔══╝  ██╔══╝  ██╔══██╗██║     ██║╚════██║   ██║   ██╔══██║
██████╔╝╚██████╔╝██║     ██║     ███████╗██║  ██║███████╗██║███████║   ██║██╗██║  ██║
╚═════╝  ╚═════╝ ╚═╝     ╚═╝     ╚══════╝╚═╝  ╚═╝╚══════╝╚═╝╚══════╝   ╚═╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#ifndef FLOAT_LIST_H
#define FLOAT_LIST_H

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
    int32_t length;
    int32_t realLength;
    float *data;
} bufferList_t;

/* create a bufferList */
bufferList_t* bufferList_init();

/* append to list */
void bufferList_append(bufferList_t *list, float data);

/* delete all contents of a list */
void bufferList_clear(bufferList_t *list);

/* pops the last item of the list off and returns it */
float bufferList_pop(bufferList_t *list);

/* deletes the item at list[index] of the list and returns it */
float bufferList_delete(bufferList_t *list, int32_t index);

/* deletes many items from the list spanning from [indexMin] to [indexMax - 1] */
void bufferList_delete_range(bufferList_t* list, int32_t indexMin, int32_t indexMax);

/* returns the index of the first instance of the item in the list, returns -1 if not found (python) */
int32_t bufferList_find(bufferList_t *list, float item);

/* duplicate of list_find */
int32_t bufferList_index(bufferList_t *list, float item);

/* counts how many instances of an item is found in the list */
int32_t bufferList_count(bufferList_t *list, float item);

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found (python but without ValueError) */
int32_t bufferList_remove(bufferList_t *list, float item);

/* copies one list to another */
void bufferList_copy(bufferList_t *dest, bufferList_t *src);

/* prints the list */
void bufferList_print(bufferList_t *list);

/* prints the list but without closing \n */
void bufferList_print_emb(bufferList_t *list);

/* frees the list's data but not the list itself */
void bufferList_free_lite(bufferList_t *list);

/* frees the data used by the list */
void bufferList_free(bufferList_t *list);

#endif /* FLOAT_LIST_H */