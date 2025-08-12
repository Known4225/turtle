#include "include/list.h"

/*
21.04.23:
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
list_copy will make a copy of all strings, pointers, and lists - it will not use the same pointers (a list can be safely freed after being copied without causing effects to the copied list)
You must call list_init() when intending to copy a list - all lists must be initialised before any functions can be called on them (if your program is crashing - check to make sure you initialised all your lists)
*/

/* create a list */
list_t *list_init() {
    list_t *list = malloc(sizeof(list_t));
    list -> length = 0;
    list -> realLength = 1;
    list -> type = calloc(1, sizeof(char));
    list -> data = calloc(1, sizeof(unitype));
    return list;
}

/* append to list, must specify type */
void list_append(list_t *list, unitype data, char type) {
    if (list -> realLength  <= list -> length) {
        list -> realLength *= 2;
        list -> type = realloc(list -> type, list -> realLength);
        list -> data = realloc(list -> data, list -> realLength * sizeof(unitype));
    }
    if (type == 'z') {
        list -> type[list -> length] = 's';
    } else {
        list -> type[list -> length] = type;
    }
    if (type == 's') {
        list -> data[list -> length].s = strdup(data.s);
    } else {
        list -> data[list -> length] = data;
    }
    list -> length += 1;
}

/* inserts the item value at list[index] of the list */
void list_insert(list_t *list, int32_t index, unitype value, char type) {
    if (list -> length == index) {
        list_append(list, value, type);
        return;
    }
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
void list_delete_range(list_t *list, uint32_t indexMin, uint32_t indexMax) {
    if (indexMin > indexMax) {
        uint32_t swap = indexMin;
        indexMin = indexMax;
        indexMax = swap;
    }
    int8_t zerod = 0; // edge case: "should've used list_clear"
    uint32_t difference = (indexMax - indexMin);
    list -> realLength = list -> length - difference;
    if (list -> realLength <= 1) {
        zerod = 1;
        list -> realLength = 1;
    }
    
    int8_t *newType = malloc(list -> realLength * sizeof(int8_t)); // no need to calloc we're gonna fill it all up anyway
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
int32_t unitype_check_equal(unitype item1, unitype item2, int8_t typeItem1, int8_t typeItem2) {
    if ((typeItem1 == 's' || typeItem2 == 's') && typeItem1 != typeItem2) {
        return 0;
    }

    switch (typeItem1) {
        case LIST_TYPE_CHAR:
            if (item1.c == item2.c) {return 1;}
            return 0;
        case LIST_TYPE_INT8:
        case LIST_TYPE_UINT8: // UINT8 or BOOL
            if (item1.b == item2.b) {return 1;}
            return 0;
        case LIST_TYPE_INT16:
        case LIST_TYPE_UINT16:
            if (item1.hu == item2.hu) {return 1;}
            return 0;
        case LIST_TYPE_INT32:
        case LIST_TYPE_UINT32:
            if (item1.u == item2.u) {return 1;}
            return 0;
        case LIST_TYPE_INT64:
        case LIST_TYPE_UINT64:
            if (item1.l == item2.l) {return 1;}
            return 0;
        case LIST_TYPE_FLOAT:
            if (item1.f == item2.f) {return 1;}
            return 0;
        case LIST_TYPE_DOUBLE:
            if (item1.d == item2.d) {return 1;}
            return 0;
        case LIST_TYPE_STRING:
            if (strcmp(item1.s, item2.s) == 0) {return 1;}
            return 0;
        case LIST_TYPE_POINTER:
            if (item1.p == item2.p) {return 1;} // questionable
            return 0;
        case LIST_TYPE_LIST:
            if (item1.r == item2.r) {return 1;} // questionable^2 (doesn't check if lists are equivalent/congruent, just compares memory location)
            return 0;
        default:
            printf("unitype_check_equal - type %d not recognized\n", typeItem1);
            return 0;
    }
}

/* returns the index of the first instance of the item in the list, returns -1 if not found */
int32_t list_find(list_t *list, unitype item, char type) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (unitype_check_equal(list -> data[i], item, list -> type[i], type)) {
            return i;
        }
    }
    return -1;
}

/* duplicate of list_find */
int32_t list_index(list_t *list, unitype item, char type) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (unitype_check_equal(list -> data[i], item, list -> type[i], type)) {
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

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found */
int32_t list_remove(list_t *list, unitype item, char type) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (unitype_check_equal(list -> data[i], item, list -> type[i], type)) {
            list_delete(list, i);
            return i;
        }
    }
    return -1;
}

/* prints a unitype item */
void unitype_fprint(FILE *fp, unitype item, char type) {
    switch (type) {
        case LIST_TYPE_CHAR:
            fprintf(fp, "%c", item.c);
        break;
        case LIST_TYPE_INT8:
            fprintf(fp, "%hhi", item.b);
        break;
        case LIST_TYPE_UINT8: // UINT8 or BOOL
            fprintf(fp, "%hhu", item.b);
        break;
        case LIST_TYPE_INT16:
            fprintf(fp, "%hi", item.h);
        break;
        case LIST_TYPE_UINT16:
            fprintf(fp, "%hu", item.hu);
        break;
        case LIST_TYPE_INT32:
            fprintf(fp, "%d", item.i);
        break;
        case LIST_TYPE_UINT32:
            fprintf(fp, "%u", item.u);
        break;
        case LIST_TYPE_INT64:
            fprintf(fp, "%lli", item.li);
        break;
        case LIST_TYPE_UINT64:
            fprintf(fp, "%llu", item.l);
        break;
        case LIST_TYPE_FLOAT:
            fprintf(fp, "%f", item.f);
        break;
        case LIST_TYPE_DOUBLE:
            fprintf(fp, "%lf", item.d);
        break;
        case LIST_TYPE_STRING:
            fprintf(fp, "%s", item.s);
        break;
        case LIST_TYPE_POINTER:
            fprintf(fp, "%p", item.p);
        break;
        case LIST_TYPE_LIST:
            list_fprint_emb(fp, item.r);
        break;
        default:
            printf("unitype_fprint - type %d not recognized\n", type);
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
            list_copy(dest -> data[i].r, src -> data[i].r);
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

/* prints the list without brackets */
void list_fprint_emb(FILE *fp, list_t *list) {
    fprintf(fp, "[");
    if (list -> length == 0) {
        fprintf(fp, "]");
        return;
    }
    for (uint32_t i = 0; i < list -> length; i++) {
        unitype_fprint(fp, list -> data[i], list -> type[i]);
        if (i == list -> length - 1) {
            fprintf(fp, "]");
        } else {
            fprintf(fp, ", ");
        }
    }
}

/* prints the list */
void list_print(list_t *list) {
    list_fprint_emb(stdout, list);
    printf("\n");
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
