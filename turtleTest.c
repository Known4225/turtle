/*
Created by Ryan Srichai, 13.09.2026

Tests for turtle libraries
*/

#include "turtle.h"

/* list upcast */
int32_t list_upcast_test() {
    list_t *test = list_init();
    char testString[] = "Hello";
    for (int32_t i = 0; i < strlen(testString); i++) {
        list_append(test, (unitype) testString[i], 'c');
    }
    for (int32_t i = 0; i < test -> length; i++) {
        if (test -> data[i].l != testString[i]) {
            return -1;
        }
    }
    return 0;
}

int32_t list_sort_test() {
    list_t *test = list_init();
    for (int32_t i = 0; i < 100; i++) {
        list_append(test, (unitype) i, 'i');
    }
    list_print(test);
    list_sort(test);
    list_print(test);
    return 0;
}

int main(int argc, char *argv[]) {
    int32_t status = 0;
    printf("\n\n\n\n");
    printf("list_upcast_test: %d\n", status |= list_upcast_test());
    printf("list_sort_test: %d\n", status |= list_sort_test());
    printf("==========================================\n");
    if (status) {
        printf("Test status: FAILED\n");
    } else {
        printf("Test status: PASSED\n");
    }
    printf("==========================================\n");
}