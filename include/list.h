#ifndef UNITYPELISTSET
#define UNITYPELISTSET // include guard

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/*
21.04.23:
unitype list, supports a variety of types

access items of a list:
list -> data[0]

access length of list:
list -> length

list functions:

create list:
list_t list = list_init();

access items of a list (as a void pointer):
void *item = list_item(list, [index]);

append to list:
list_append(list, (unitype) [data], 'i');

insert to list
list_insert(list, [index], (unitype) [data], 'i');

pop from list:
list_pop(list);

delete index from list:
list_delete(list, [index]);

delete range of indices from list (deletes from indexMin to indexMax - 1):
list_delete_range(list, [indexMin], [indexMax]);

find element in the list (must specify type):
list_find(list, [element], 'i');

count how many elements are in the list (must specify type):
list_count(list, [elements], 'i');

find and delete element in the list (must specify type):
list_remove(list, [element], 'i');

copy one list to another
list_copy(dest, source);

delete all the elements of a list
list_clear(list);

print the list:
list_print(list);

free the list (when done using):
list_free(list);
*/

struct list_f; // so basically im a good programmer
typedef struct list_f list_t;

typedef union { // supported types
    int32_t i;
    uint32_t u;
    float f;
    double d;
    char c;
    char* s;
    void* p;
    list_t* r;
    uint64_t l;
    uint16_t h;
    uint8_t b;
} unitype;

struct list_f {
    uint32_t length;
    uint32_t realLength;
    char *type;
    unitype *data;
};

/* create a list */
list_t *list_init() {
    list_t *list = malloc(sizeof(list_t));
    list -> length = 0;
    list -> realLength = 1;
    list -> type = calloc(1, sizeof(char));
    list -> data = calloc(1, sizeof(unitype));
    return list;
}

/* accesses an item of the list as a void pointer */
void *list_item(list_t *list, uint32_t index) {
    void *ret;
    switch (list -> type[index]) {
        case 'i':
            ret = &list -> data[index].i;
        break;
        case 'u':
            ret = &list -> data[index].u;
        break;
        case 'f':
            ret = &list -> data[index].f;
        break;
        case 'd':
            ret = &list -> data[index].d;
        break;
        case 'c':
            ret = &list -> data[index].c;
        break;
        case 's':
            ret = list -> data[index].s;
        break;
        case 'p':
            ret = &list -> data[index].p;
        break;
        case 'r':
            ret = &list -> data[index].r;
        break;
        case 'l':
            ret = &list -> data[index].l;
        break;
        case 'h':
            ret = &list -> data[index].h;
        break;
        case 'b':
            ret = &list -> data[index].b;
        break;
        default:
            ret = NULL;
            printf("item - type not recognized\n");
    }
    return ret;
}

void list_free_lite(list_t *);
void list_free(list_t *);
void list_print_emb(list_t *);

/* append to list, must specify type */
void list_append(list_t *list, unitype data, char type) {
    if (list -> realLength  <= list -> length) {
        list -> realLength *= 2;
        list -> type = realloc(list -> type, list -> realLength);
        list -> data = realloc(list -> data, list -> realLength * sizeof(unitype));
    }
    list -> type[list -> length] = type;
    if (type == 's') {
        list -> data[list -> length].s = strdup(data.s);
    } else {
        list -> data[list -> length] = data;
    }
    list -> length += 1;
}

/* inserts the item value at list[index] of the list */
void list_insert(list_t *list, int32_t index, unitype value, char type) {
    while (index < 0) {index += list -> length;}
    index %= list -> length;
    list_append(list, (unitype) 0, type);
    int32_t i;
    for (i = list -> length - 1; i > index; i--) {
        list -> data[i] = list -> data[i - 1];
        list -> type[i] = list -> type[i - 1];
    }
    list -> data[i] = value;
}

/* clears the list */
void list_clear(list_t *list) {
    list_free_lite(list);
    list -> length = 0;
    list -> realLength = 1;
    list -> type = calloc(1, sizeof(char));
    list -> data = calloc(1, sizeof(unitype));
}

/* pops the last item of the list off and returns it */
unitype list_pop(list_t *list) {
    if (list -> length > 0) {
        list -> length -= 1;
        unitype ret = list -> data[list -> length];
        if (list -> type[list -> length] == 'r') {
            list_free(list -> data[list -> length].r);
        }
        if (list -> type[list -> length] == 's' || list -> type[list -> length] == 'p') {
            free(list -> data[list -> length].p);
        }
        list -> type[list -> length] = (char) 0;
        list -> data[list -> length] = (unitype) 0;
        if (list -> length <= list -> realLength / 2 && list -> realLength > 1) {
            list -> realLength /= 2;
            list -> type = realloc(list -> type, list -> realLength);
            list -> data = realloc(list -> data, list -> realLength * sizeof(unitype));
        }
        return ret;
    } else {
        return (unitype) 0;
    }
}

/* deletes the item at list[index] of the list and returns it */
unitype list_delete(list_t *list, int32_t index) {
    while (index < 0) {index += list -> length;}
    index %= list -> length;
    unitype ret = list -> data[index];
    if (list -> type[index] == 'r') {
        list_free(list -> data[index].r);
    }
    if (list -> type[index] == 's' || list -> type[index] == 'p') {
        free(list -> data[index].p);
    }
    for (uint32_t i = index; i < list -> length - 1 ; i++) {
        list -> data[i] = list -> data[i + 1];
        list -> type[i] = list -> type[i + 1];
    }
    list -> length -= 1;
    list -> type[list -> length] = (char) 0;
    list -> data[list -> length] = (unitype) 0;
    if (list -> length <= list -> realLength / 2 && list -> realLength > 1) {
        list -> realLength /= 2;
        list -> type = realloc(list -> type, list -> realLength);
        list -> data = realloc(list -> data, list -> realLength * sizeof(unitype));
    }
    return ret;
}

/* deletes many items from the list spanning from [indexMin] to [indexMax - 1] */
void list_delete_range(list_t* list, uint32_t indexMin, uint32_t indexMax) {
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
    
    char *newType = malloc(list -> realLength * sizeof(char)); // no need to calloc we're gonna fill it all up anyway
    unitype *newData = malloc(list -> realLength * sizeof(unitype));
    for (uint32_t i = 0; i < indexMin; i++) {
        newType[i] = list -> type[i];
        newData[i] = list -> data[i];
    }
    for (uint32_t i = indexMax; i < list -> length; i++) {
        newType[i - difference] = list -> type[i];
        newData[i - difference] = list -> data[i];
    }
    list -> length = list -> realLength;
    if (zerod)
        list -> length = 0;
    free(list -> type);
    free(list -> data);
    list -> type = newType;
    list -> data = newData;
}

/* checks if two unitype items are equal */
int32_t unitype_check_equal(unitype item1, unitype item2, char typeItem1, char typeItem2) {
    if ((typeItem1 == 's' || typeItem2 == 's') && typeItem1 != typeItem2) { // logical xor but idk how to do it
        return 0;
    }
    switch (typeItem1) {
        case 'i':
            if (item1.i == item2.i) {return 1;}
        break;
        case 'u':
            if (item1.i == item2.i) {return 1;}
        break;
        case 'f':
            if (item1.f == item2.f) {return 1;}
        break;
        case 'd':
            if (item1.d == item2.d) {return 1;}
        break;
        case 'c':
            if (item1.c == item2.c) {return 1;}
        break;
        case 's':
            if (strcmp(item1.s, item2.s) == 0) {return 1;}
        break;
        case 'p':
            if (item1.p == item2.p) {return 1;} // questionable
        break;
        case 'r':
            if (item1.r == item2.r) {return 1;} // questionable^2 (doesn't check if lists are equivalent/congruent, just compares memory location)
        break;
        case 'l':
            if (item1.l == item2.l) {return 1;}
        break;
        case 'h':
            if (item1.h == item2.h) {return 1;}
        break;
        case 'b':
            if (item1.b == item2.b) {return 1;}
        break;
    }
    return 0;
}

/* returns the index of the first instance of the item in the list, returns -1 if not found (python) */
int32_t list_find(list_t *list, unitype item, char type) {
    int32_t trig = 0;
    for (uint32_t i = 0; i < list -> length; i++) {
        trig += unitype_check_equal(list -> data[i], item, list -> type[i], type);
        if (trig == 1) {
            return i;
        }
    }
    return -1;
}

/* duplicate of list_find */
int32_t list_index(list_t *list, unitype item, char type) {
    int32_t trig = 0;
    for (uint32_t i = 0; i < list -> length; i++) {
        trig += unitype_check_equal(list -> data[i], item, list -> type[i], type);
        if (trig == 1) {
            return i;
        }
    }
    return -1;
}

/* counts how many instances of an item is found in the list */
uint32_t list_count(list_t *list, unitype item, char type) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < list -> length; i++) {
        count += unitype_check_equal(list -> data[i], item, list -> type[i], type);
    }
    return count;
}

/* sort list */
void list_sort(list_t *list) {
    /* create min heap */
    int32_t temp;
    for (uint32_t i = 2; i < list -> length + 1; i++) {
        int32_t j = i;
        while (j > 1 && list -> data[j / 2 - 1].i > list -> data[j - 1].i) {
            temp = list -> data[j / 2 - 1].i;
            list -> data[j / 2 - 1].i = list -> data[j - 1].i;
            list -> data[j - 1].i = temp;
            j /= 2;
        }
    }
    /* heapsort */
    for (int32_t i = list -> length - 1; i > 0; i--) {
        temp = list -> data[0].i;
        list -> data[0].i = list -> data[i].i;
        list -> data[i].i = temp;
        int32_t j = 1;
        while ((j * 2 - 1 < i && list -> data[j - 1].i > list -> data[j * 2 - 1].i) || (j * 2 < i && list -> data[j - 1].i > list -> data[j * 2].i)) {
            if (list -> data[j * 2 - 1].i < list -> data[j * 2].i || j * 2 == i) {
                temp = list -> data[j - 1].i;
                list -> data[j - 1] = list -> data[j * 2 - 1];
                list -> data[j * 2 - 1].i = temp;
                j = j * 2;
            } else {
                temp = list -> data[j - 1].i;
                list -> data[j - 1].i = list -> data[j * 2].i;
                list -> data[j * 2].i = temp;
                j *= 2 + 1;
            }
        }
    }
}

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found (python but without ValueError) */
int32_t list_remove(list_t *list, unitype item, char type) {
    uint32_t trig = 0;
    for (uint32_t i = 0; i < list -> length; i++) {
        trig += unitype_check_equal(list -> data[i], item, list -> type[i], type);
        if (trig == 1) {
            list_delete(list, i);
            return i;
        }
    }
    return -1;
}

/* prints a unitype item */
void unitype_print(unitype item, char type) {
    switch (type) {
        case 'i':
            printf("%d", item.i);
        break;
        case 'u':
            printf("%u", item.u);
        break;
        case 'f':
            printf("%f", item.f);
        break;
        case 'd':
            printf("%lf", item.d);
        break;
        case 'c':
            printf("%c", item.c);
        break;
        case 's':
            printf("%s", item.s);
        break;
        case 'p':
            printf("%p", item.p);
        break;
        case 'r':
            list_print_emb(item.r);
        break;
        case 'l':
            printf("%llu", item.l);
        break;
        case 'h':
            printf("%hi", item.h);
        break;
        case 'b':
            printf("%hi", item.b);
        break;
        default:
            printf("print - type not recognized\n");
            return;
    }
}

/* copies one list to another (duplicates strings or pointers) */
void list_copy(list_t *dest, list_t *src) {
    list_free_lite(dest);
    dest -> type = calloc(src -> realLength, sizeof(int32_t));
    dest -> data = calloc(src -> realLength, sizeof(unitype));
    uint32_t len = src -> length;
    dest -> length = len;
    dest -> realLength = src -> realLength;
    for (uint32_t i = 0; i < len; i++) {
        dest -> type[i] = src -> type[i];
        if (src -> type[i] == 'r') {
            dest -> data[i] = (unitype) (void *) list_init();
            list_copy(src -> data[i].r, dest -> data[i].r);
        } else {
            if (src -> type[i] == 'p') {
                memcpy(dest -> data[i].p, src -> data[i].p, sizeof(unitype));
            } else {
                if (src -> type[i] == 's') {
                    dest -> data[i].s = strdup(src -> data[i].s);
                } else {
                    dest -> data[i] = src -> data[i];
                }
            }
        }
    }
}

/* prints the list (like python would) */
void list_print(list_t *list) {
    printf("[");
    if (list -> length == 0) {
        printf("]\n");
        return;
    }
    for (uint32_t i = 0; i < list -> length; i++) {
        unitype_print(list -> data[i], list -> type[i]);
        if (i == list -> length - 1) {
            printf("]\n");
        } else {
            printf(", ");
        }
    }
}

/* prints the list but without closing \n */
void list_print_emb(list_t *list) {
    printf("[");
    if (list -> length == 0) {
        printf("]");
        return;
    }
    for (uint32_t i = 0; i < list -> length; i++) {
        unitype_print(list -> data[i], list -> type[i]);
        if (i == list -> length - 1) {
            printf("]");
        } else {
            printf(", ");
        }
    }
}

/* prints the types of the list */
void list_print_type(list_t *list) {
    printf("[");
    if (list -> length == 0) {
        printf("]\n");
        return;
    }
    for (uint32_t i = 0; i < list -> length; i++) {
        printf("%c", list -> type[i]);
        if (i == list -> length - 1) {
            printf("]\n");
        } else {
            printf(", ");
        }
    }
}

/* frees the list's data but not the list itself */
void list_free_lite(list_t *list) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (list -> type[i] == 'r') {
            list_free(list -> data[i].r);
        }
        if ((list -> type[i] == 's' || list -> type[i] == 'p') && list -> data[i].p != NULL) {
            free(list -> data[i].s);
        }
    }
    free(list -> type);
    free(list -> data);
}

/* frees the data used by the list */
void list_free(list_t *list) {
    list_free_lite(list);
    free(list);
}

#endif