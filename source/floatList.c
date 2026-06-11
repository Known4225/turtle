/*
███████╗██╗      ██████╗  █████╗ ████████╗██╗     ██╗███████╗████████╗ ██████╗
██╔════╝██║     ██╔═══██╗██╔══██╗╚══██╔══╝██║     ██║██╔════╝╚══██╔══╝██╔════╝
█████╗  ██║     ██║   ██║███████║   ██║   ██║     ██║███████╗   ██║   ██║     
██╔══╝  ██║     ██║   ██║██╔══██║   ██║   ██║     ██║╚════██║   ██║   ██║     
██║     ███████╗╚██████╔╝██║  ██║   ██║   ███████╗██║███████║   ██║██╗╚██████╗
╚═╝     ╚══════╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝╚══════╝   ╚═╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#include "../include/floatList.h"

floatList_t* floatList_init() {
    floatList_t *list = malloc(sizeof(floatList_t));
    list -> length = 0;
    list -> realLength = 1;
    list -> data = calloc(1, sizeof(float));
    return list;
}

/* append to list */
void floatList_append(floatList_t *list, float data) {
    if (list -> realLength  <= list -> length) {
        list -> realLength *= 2;
        list -> data = realloc(list -> data, list -> realLength * sizeof(float));
    }
    list -> data[list -> length] = data;
    list -> length += 1;
}

void floatList_clear(floatList_t *list) {
    floatList_free_lite(list);
    list -> length = 0;
    list -> realLength = 1;
    list -> data = calloc(1, sizeof(float));
}

/* pops the last item of the list off and returns it */
float floatList_pop(floatList_t *list) {
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
float floatList_delete(floatList_t *list, int32_t index) {
    while (index < 0) {index += list -> length;}
    index %= list -> length;
    float ret = list -> data[index];
    for (int32_t i = index; i < list -> length - 1 ; i++) {
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
void floatList_delete_range(floatList_t* list, int32_t indexMin, int32_t indexMax) {
    if (indexMin > indexMax) {
        int32_t swap = indexMin;
        indexMin = indexMax;
        indexMax = swap;
    }
    char zerod = 0; // edge case: "should've used list_clear"
    int32_t difference = (indexMax - indexMin);
    list -> realLength = list -> length - difference;
    if (list -> realLength <= 1) {
        zerod = 1;
        list -> realLength = 1;
    }
    
    float *newData = malloc(list -> realLength * sizeof(float)); // no need to calloc we're gonna fill it all up anyway
    for (int32_t i = 0; i < indexMin; i++) {
        newData[i] = list -> data[i];
    }
    for (int32_t i = indexMax; i < list -> length; i++) {
        newData[i - difference] = list -> data[i];
    }
    list -> length = list -> realLength;
    if (zerod)
        list -> length = 0;
    free(list -> data);
    list -> data = newData;
}

/* returns the index of the first instance of the item in the list, returns -1 if not found (python) */
int32_t floatList_find(floatList_t *list, float item) {
    for (int32_t i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            return i;
        }
    }
    return -1;
}

/* duplicate of list_find */
int32_t floatList_index(floatList_t *list, float item) {
    for (int32_t i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            return i;
        }
    }
    return -1;
}

/* counts how many instances of an item is found in the list */
int32_t floatList_count(floatList_t *list, float item) {
    int32_t count = 0;
    for (int32_t i = 0; i < list -> length; i++) {
        count += (list -> data[i] == item);
    }
    return count;
}

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found (python but without ValueError) */
int32_t floatList_remove(floatList_t *list, float item) {
    for (int32_t i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            floatList_delete(list, i);
            return i;
        }
    }
    return -1;
}

/* copies one list to another */
void floatList_copy(floatList_t *dest, floatList_t *src) {
    floatList_free_lite(dest);
    dest -> data = calloc(src -> realLength, sizeof(float));
    int32_t len = src -> length;
    dest -> length = len;
    dest -> realLength = src -> realLength;
    for (int32_t i = 0; i < len; i++) {
        dest -> data[i] = src -> data[i];
    }
}

/* prints the list (like python would) */
void floatList_print(floatList_t *list) {
    printf("[");
    if (list -> length == 0) {
        printf("]\n");
        return;
    }
    for (int32_t i = 0; i < list -> length; i++) {
        printf("%f", list -> data[i]);
        if (i == list -> length - 1) {
            printf("]\n");
        } else {
            printf(", ");
        }
    }
}

/* prints the list but without closing \n */
void floatList_print_emb(floatList_t *list) {
    printf("[");
    if (list -> length == 0) {
        printf("]");
        return;
    }
    for (int32_t i = 0; i < list -> length; i++) {
        printf("%f", list -> data[i]);
        if (i == list -> length - 1) {
            printf("]");
        } else {
            printf(", ");
        }
    }
}

/* frees the list's data but not the list itself */
void floatList_free_lite(floatList_t *list) {
    free(list -> data);
}

/* frees the data used by the list */
void floatList_free(floatList_t *list) {
    floatList_free_lite(list);
    free(list);
}