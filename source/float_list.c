/*
███████╗██╗      ██████╗  █████╗ ████████╗██╗     ██╗███████╗████████╗ ██████╗
██╔════╝██║     ██╔═══██╗██╔══██╗╚══██╔══╝██║     ██║██╔════╝╚══██╔══╝██╔════╝
█████╗  ██║     ██║   ██║███████║   ██║   ██║     ██║███████╗   ██║   ██║     
██╔══╝  ██║     ██║   ██║██╔══██║   ██║   ██║     ██║╚════██║   ██║   ██║     
██║     ███████╗╚██████╔╝██║  ██║   ██║   ███████╗██║███████║   ██║██╗╚██████╗
╚═╝     ╚══════╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝╚══════╝   ╚═╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#include "../include/float_list.h"

float_list_t* float_list_init() {
    float_list_t *list = malloc(sizeof(float_list_t));
    list -> length = 0;
    list -> realLength = 1;
    list -> data = calloc(1, sizeof(float));
    return list;
}

/* append to list */
void float_list_append(float_list_t *list, float data) {
    if (list -> realLength  <= list -> length) {
        list -> realLength *= 2;
        list -> data = realloc(list -> data, list -> realLength * sizeof(float));
    }
    list -> data[list -> length] = data;
    list -> length += 1;
}

void float_list_clear(float_list_t *list) {
    float_list_free_lite(list);
    list -> length = 0;
    list -> realLength = 1;
    list -> data = calloc(1, sizeof(float));
}

/* pops the last item of the list off and returns it */
float float_list_pop(float_list_t *list) {
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
float float_list_delete(float_list_t *list, int32_t index) {
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
void float_list_delete_range(float_list_t* list, int32_t indexMin, int32_t indexMax) {
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
int32_t float_list_find(float_list_t *list, float item) {
    for (int32_t i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            return i;
        }
    }
    return -1;
}

/* duplicate of list_find */
int32_t float_list_index(float_list_t *list, float item) {
    for (int32_t i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            return i;
        }
    }
    return -1;
}

/* counts how many instances of an item is found in the list */
int32_t float_list_count(float_list_t *list, float item) {
    int32_t count = 0;
    for (int32_t i = 0; i < list -> length; i++) {
        count += (list -> data[i] == item);
    }
    return count;
}

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found (python but without ValueError) */
int32_t float_list_remove(float_list_t *list, float item) {
    for (int32_t i = 0; i < list -> length; i++) {
        if (list -> data[i] == item) {
            float_list_delete(list, i);
            return i;
        }
    }
    return -1;
}

/* copies one list to another */
void float_list_copy(float_list_t *dest, float_list_t *src) {
    float_list_free_lite(dest);
    dest -> data = calloc(src -> realLength, sizeof(float));
    int32_t len = src -> length;
    dest -> length = len;
    dest -> realLength = src -> realLength;
    for (int32_t i = 0; i < len; i++) {
        dest -> data[i] = src -> data[i];
    }
}

/* prints the list (like python would) */
void float_list_print(float_list_t *list) {
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
void float_list_print_emb(float_list_t *list) {
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
void float_list_free_lite(float_list_t *list) {
    free(list -> data);
}

/* frees the data used by the list */
void float_list_free(float_list_t *list) {
    float_list_free_lite(list);
    free(list);
}