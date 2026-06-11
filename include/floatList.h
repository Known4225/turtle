/*
███████╗██╗      ██████╗  █████╗ ████████╗██╗     ██╗███████╗████████╗██╗  ██╗
██╔════╝██║     ██╔═══██╗██╔══██╗╚══██╔══╝██║     ██║██╔════╝╚══██╔══╝██║  ██║
█████╗  ██║     ██║   ██║███████║   ██║   ██║     ██║███████╗   ██║   ███████║
██╔══╝  ██║     ██║   ██║██╔══██║   ██║   ██║     ██║╚════██║   ██║   ██╔══██║
██║     ███████╗╚██████╔╝██║  ██║   ██║   ███████╗██║███████║   ██║██╗██║  ██║
╚═╝     ╚══════╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝╚══════╝   ╚═╝╚═╝╚═╝  ╚═╝
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
floatList - like a normal list but only supports floats
*/

typedef struct {
    int32_t length;
    int32_t realLength;
    float *data;
} floatList_t;

/* create a floatList */
floatList_t* floatList_init();

/* append to list */
void floatList_append(floatList_t *list, float data);

/* delete all contents of a list */
void floatList_clear(floatList_t *list);

/* pops the last item of the list off and returns it */
float floatList_pop(floatList_t *list);

/* deletes the item at list[index] of the list and returns it */
float floatList_delete(floatList_t *list, int32_t index);

/* deletes many items from the list spanning from [indexMin] to [indexMax - 1] */
void floatList_delete_range(floatList_t* list, int32_t indexMin, int32_t indexMax);

/* returns the index of the first instance of the item in the list, returns -1 if not found (python) */
int32_t floatList_find(floatList_t *list, float item);

/* duplicate of list_find */
int32_t floatList_index(floatList_t *list, float item);

/* counts how many instances of an item is found in the list */
int32_t floatList_count(floatList_t *list, float item);

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found (python but without ValueError) */
int32_t floatList_remove(floatList_t *list, float item);

/* copies one list to another */
void floatList_copy(floatList_t *dest, floatList_t *src);

/* prints the list */
void floatList_print(floatList_t *list);

/* prints the list but without closing \n */
void floatList_print_emb(floatList_t *list);

/* frees the list's data but not the list itself */
void floatList_free_lite(floatList_t *list);

/* frees the data used by the list */
void floatList_free(floatList_t *list);

#endif /* FLOAT_LIST_H */