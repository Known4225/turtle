/*
██╗     ██╗███████╗████████╗██╗  ██╗
██║     ██║██╔════╝╚══██╔══╝██║  ██║
██║     ██║███████╗   ██║   ███████║
██║     ██║╚════██║   ██║   ██╔══██║
███████╗██║███████║   ██║██╗██║  ██║
╚══════╝╚═╝╚══════╝   ╚═╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#ifndef UNITYPE_LIST_H
#define UNITYPE_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdatomic.h>

/*
Created by Ryan Srichai 21.04.23:
unitype list, supports a variety of types

example usage:
list_t *newList = list_init();
list_append(newList, (unitype) "hello", 's'); // add to list
list_append(newList, (unitype) "test", LIST_TYPE_STRING); // add to list using enum
list_print(newList);
> [hello, test]
list_insert(newList, (unitype) 128, 'i', 1);
list_insert(newList, (unitype) 12.0, 'd', 1);
list_print(newList);
> [hello, 12.0, 128, test]
printf("%s\n", newList -> data[3].s);
> test
printf("%d\n", newList -> length);
> 4 
list_delete(newList, 0);
list_print(newList);
> [12.0, 128, test]
list_clear(newList);
list_print(newList);
> []
list_free(newList);

Notes:
Strings added to the list will be "strdup"d - meaning that you can pass in stack allocated buffers and string literals. This does not apply to pointers added to the list which must be heap allocated
When calling list_clear() or list_free(), the list will free all strings, pointers, and lists within itself. If you don't want this to happen append the item to the list as a uint64
list_copy will make a copy of all strings, pointers, and lists - it will not use the same pointers (a list can be safely freed after being copied without causing effects to the copied list). The data in pointers is not copied as the list does not know how big the data is
You must call list_init() when intending to copy a list - all lists must be initialised before any functions can be called on them (if your program is crashing - check to make sure you initialised all your lists)
*/

typedef enum {
    LIST_TYPE_BOOL = 'b',
    LIST_TYPE_CHAR = 'c',
    LIST_TYPE_INT8 = 'e',
    LIST_TYPE_UINT8 = 'b',
    LIST_TYPE_INT16 = 'j',
    LIST_TYPE_UINT16 = 'h',
    LIST_TYPE_INT32 = 'i',
    LIST_TYPE_UINT32 = 'u',
    LIST_TYPE_INT64 = 'm',
    LIST_TYPE_UINT64 = 'l',
    LIST_TYPE_FLOAT = 'f',
    LIST_TYPE_DOUBLE = 'd',
    LIST_TYPE_STRING = 's',
    LIST_TYPE_POINTER = 'p',
    LIST_TYPE_LIST = 'r',
} list_type_t;

struct list_f; // so basically im a good programmer
typedef struct list_f list_t;

/* unitype - a union of many supported types to the list */
typedef union {
    signed char ch;
    bool bo;
    int8_t c;
    uint8_t b;
    int16_t h;
    uint16_t hu;
    int32_t i;
    uint32_t u;
    int64_t li;
    uint64_t l;
    float f;
    double d; // i will stand by it. d should be double
    double lf;
    char *s;
    void *p;
    list_t *r;
    list_t *list;
} unitype;

struct list_f {
    _Atomic volatile int8_t lock;
    int32_t ID;
    int32_t length;
    int32_t realLength;
    int8_t *type;
    unitype *data;
};

typedef struct {
    _Atomic volatile int8_t lock;
    int32_t ID;
    int32_t length;
    int32_t dummy;
    int8_t *type;
    unitype *data;
} sublist_t;

/* create a list */
list_t *list_init();

/* acquire lock on list (only do this in multithreaded applications) */
void list_acquire(list_t *list);

/* release lock on list (only do this in multithreaded applications) */
void list_release(list_t *list);

/* append to list, must specify type */
void list_append(list_t *list, unitype data, char type);

/* inserts the item value at list[index] of the list */
void list_insert(list_t *list, int32_t index, unitype value, char type);

/* clears the list */
void list_clear(list_t *list);

/* pops the last item of the list off and returns it */
unitype list_pop(list_t *list);

/* deletes the item at list[index] of the list and returns it */
unitype list_delete(list_t *list, int32_t index);

/* deletes many items from the list spanning from [indexMin] to [indexMax - 1] */
void list_delete_range(list_t *list, int32_t indexMin, int32_t indexMax);

/* checks if two unitype items are equal */
int32_t unitype_check_equal(unitype item1, unitype item2, int8_t typeItem1, int8_t typeItem2);

/* returns the index of the first instance of the item in the list, returns -1 if not found */
int32_t list_find(list_t *list, unitype item, char type);

/* duplicate of list_find */
int32_t list_index(list_t *list, unitype item, char type);

/* counts how many instances of an item is found in the list */
int32_t list_count(list_t *list, unitype item, char type);

/* sort list (biggest to smallest) */
void list_sort(list_t *list);

/* return a list of indices that would sort the list (biggest to smallest) */
list_t *list_sort_index(list_t *list);

/* return a list of indices that would sort the list (for a list of doubles) */
list_t *list_sort_index_double(list_t *list);

/* sort list (stride) (biggest to smallest) */
void list_sort_stride(list_t *list, int32_t stride, int32_t offset);

/* return a list of indices that would sort the list (stride) (biggest to smallest) */
list_t *list_sort_stride_index(list_t *list, int32_t stride, int32_t offset);

/* return a list of indices that would sort the list (stride) (for a list of doubles) */
list_t *list_sort_stride_index_double(list_t *list, int32_t stride, int32_t offset);

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found */
int32_t list_remove(list_t *list, unitype item, char type);

/* prints a unitype item */
void unitype_fprint(FILE *fp, unitype item, char type);

/* copies one list to another (duplicates strings or pointers) */
void list_copy(list_t *dest, list_t *src);

/* prints the list without brackets */
void list_fprint_emb(FILE *fp, list_t *list);

/* prints the list */
void list_print(list_t *list);

/* prints the types of the list */
void list_print_type(list_t *list);

/* frees the list's data but not the list itself */
void list_free_lite(list_t *list);

/* frees the data used by the list */
void list_free(list_t *list);

/* creates a sublist (from bottom to top - 1) out of an existing list, do not modify a parent list while a sublist exist */
sublist_t *sublist_init(list_t *list, int32_t bottom, int32_t top);

/* delete a sublist */
void sublist_free(sublist_t *sublist);

#endif /* UNITYPE_LIST_H */
