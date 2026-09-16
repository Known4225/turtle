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
float_list - like a normal list but only supports floats
*/

typedef struct {
    int32_t length;
    int32_t realLength;
    float *data;
} float_list_t;

/* create a float_list */
float_list_t* float_list_init();

/* append to list */
void float_list_append(float_list_t *list, float data);

/* delete all contents of a list */
void float_list_clear(float_list_t *list);

/* pops the last item of the list off and returns it */
float float_list_pop(float_list_t *list);

/* deletes the item at list[index] of the list and returns it */
float float_list_delete(float_list_t *list, int32_t index);

/* deletes many items from the list spanning from [indexMin] to [indexMax - 1] */
void float_list_delete_range(float_list_t *list, int32_t indexMin, int32_t indexMax);

/* returns the index of the first instance of the item in the list, returns -1 if not found (python) */
int32_t float_list_find(float_list_t *list, float item);

/* duplicate of list_find */
int32_t float_list_index(float_list_t *list, float item);

/* counts how many instances of an item is found in the list */
int32_t float_list_count(float_list_t *list, float item);

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found (python but without ValueError) */
int32_t float_list_remove(float_list_t *list, float item);

/* copies one list to another */
void float_list_copy(float_list_t *dest, float_list_t *src);

/* prints the list */
void float_list_print(float_list_t *list);

/* prints the list but without closing \n */
void float_list_print_emb(float_list_t *list);

/* frees the list's data but not the list itself */
void float_list_free_lite(float_list_t *list);

/* frees the data used by the list */
void float_list_free(float_list_t *list);

#endif /* FLOAT_LIST_H */