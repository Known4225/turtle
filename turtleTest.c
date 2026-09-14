/*
Created by Ryan Srichai, 13.09.2026

Tests for turtle libraries
*/

#include "turtle.h"
#include <time.h>

int randomInt(int lowerBound, int upperBound) { // random integer between lower and upper bound (inclusive)
    return (rand() % (upperBound - lowerBound + 1) + lowerBound);
}

double randomDouble(double lowerBound, double upperBound) { // random double between lower and upper bound
    return (rand() * (upperBound - lowerBound) / RAND_MAX + lowerBound); // probably works idk
}

/* list upcast */
int32_t list_upcast_test() {
    list_t *test = list_init();
    /* append test */
    char testString[] = "Hello World";
    int32_t length = strlen(testString);
    for (int32_t i = 0; i < length; i++) {
        list_append(test, (unitype) testString[i], 'c');
    }
    for (int32_t i = 0; i < length; i++) {
        if (test -> data[i].lu != testString[i]) {
            return -1;
        }
    }
    /* replacement test */
    for (int32_t i = 0; i < length; i++) {
        test -> data[i].lu = -1;
    }
    for (int32_t i = 0; i < length; i++) {
        test -> data[i] = (unitype) testString[i]; // it seems like upcasting to unitype does pad the rest with 0s
        // Note: doing test -> data[i].c = testString[i]; does NOT WORK
    }
    for (int32_t i = 0; i < length; i++) {
        if (test -> data[i].lu != testString[i]) {
            return -2;
        }
    }
    /* negative numbers test */
    list_clear(test);
    char negative = -1;
    list_append(test, (unitype) negative, 'c');
    if (test -> data[0].i >= 0) {
        // return -3; // this FAILS
    }
    return 0;
}

int32_t list_sort_test() {
    int32_t index;
    list_t *test = list_init();
    list_t *testIndex;
    /* integer sort test */
    list_clear(test);
    for (int32_t i = 0; i < 100; i++) {
        list_append(test, (unitype) i, 'i');
        list_append(test, (unitype) i, 'i');
    }
    list_sort(test);
    index = 0;
    for (int32_t i = 99; i >= 0; i--) {
        if (test -> data[index].i != i) {
            return -1;
        }
        if (test -> data[index + 1].i != i) {
            return -1;
        }
        index += 2;
    }
    /* double sort test */
    list_clear(test);
    for (int32_t i = 0; i < 100; i++) {
        list_append(test, (unitype) (double) i, 'd');
        list_append(test, (unitype) (double) i, 'd');
    }
    list_sort(test);
    index = 0;
    for (int32_t i = 99; i >= 0; i--) {
        if (test -> data[index].d != i) {
            return -2;
        }
        if (test -> data[index + 1].d != i) {
            return -2;
        }
        index += 2;
    }
    /* hybrid sort test */
    list_clear(test);
    for (int32_t i = 0; i < 100; i++) {
        list_append(test, (unitype) (i - 1000), 'i');
        list_append(test, (unitype) (double) (i - 1000), 'd');
    }
    list_sort(test);
    index = 0;
    for (int32_t i = 99; i >= 0; i--) {
        if (test -> type[index] == 'i') {
            if (test -> data[index].i != i - 1000) {
                return -3;
            }
        } else if (test -> type[index] == 'd') {
            if (test -> data[index].d != i - 1000) {
                return -3;
            }
        }
        if (test -> type[index + 1] == 'i') {
            if (test -> data[index + 1].i != i - 1000) {
                return -3;
            }
        } else if (test -> type[index + 1] == 'd') {
            if (test -> data[index + 1].d != i - 1000) {
                return -3;
            }
        }
        index += 2;
    }
    /* stride sort test */
    list_clear(test);
    for (int32_t i = 0; i < 26; i++) {
        list_append(test, (unitype) "Hello", 's');
        list_append(test, (unitype) (i + 65), 'c');
        list_append(test, (unitype) i, 'i');
    }
    list_sort_stride(test, 3, 2);
    index = 0;
    for (int32_t i = 25; i >= 0; i--) {
        if (strcmp(test -> data[index].s, "Hello")) {
            return -4;
        }
        if (test -> data[index + 1].c != i + 65) {
            return -4;
        }
        if (test -> data[index + 2].i != i) {
            return -4;
        }
        index += 3;
    }
    /* index sort test */
    list_clear(test);
    for (int32_t i = 0; i < 100; i++) {
        list_append(test, (unitype) (100 - i), 'i');
        list_append(test, (unitype) i, 'i');
    }
    testIndex = list_sort_index(test);
    index = 0;
    for (int32_t i = 0; i < 100; i++) {
        if (test -> data[testIndex -> data[index].i].i != 100 - i) {
            return -5;
        }
        if (test -> data[testIndex -> data[index + 1].i].i != 99 - i) {
            return -5;
        }
        index += 2;
    }
    list_free(testIndex);
    /* index stride sort test */
    testIndex = list_sort_stride_index(test, 2, 1);
    index = 0;
    for (int32_t i = 0; i < 100; i++) {
        if (testIndex -> data[index].i != 200 - (i + 1) * 2) {
            return -6;
        }
        if (testIndex -> data[index + 1].i != 201 - (i + 1) * 2) {
            return -6;
        }
        index += 2;
    }
    list_free(testIndex);
    return 0;
}

int32_t list_select_test() {
    list_t *test = list_init();
    /* integer select test */
    for (int32_t j = 0; j < 100; j++) {
        list_clear(test);
        for (int32_t i = 0; i < 64; i++) {
            list_append(test, (unitype) randomInt(-100, 100), 'i');
        }
        int32_t medianIndex = list_median(test);
        int32_t median = test -> data[medianIndex].i;
        list_sort(test);
        if (test -> data[test -> length / 2].i != median) {
            return -1;
        }
    }
    /* double select test */
    for (int32_t j = 0; j < 100; j++) {
        list_clear(test);
        for (int32_t i = 0; i < 64; i++) {
            list_append(test, (unitype) randomDouble(-100, 100), 'd');
        }
        int32_t medianIndex = list_median(test);
        double median = test -> data[medianIndex].d;
        list_sort(test);
        if (test -> data[test -> length / 2].d != median) {
            return -2;
        }
    }
    /* hybrid select test */
    for (int32_t j = 0; j < 100; j++) {
        list_clear(test);
        for (int32_t i = 0; i < 64; i++) {
            list_append(test, (unitype) randomInt(-100, 100), 'i');
            list_append(test, (unitype) randomDouble(-100, 100), 'd');
        }
        int32_t medianIndex = list_median(test);
        unitype median = test -> data[medianIndex];
        list_sort(test);
        if (test -> data[test -> length / 2].lu != median.lu) {
            return -3;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    printf("\n\n\n\n");
    printf("list_upcast_test: %d\n", list_upcast_test());
    printf("list_sort_test: %d\n", list_sort_test());
    printf("list_select_test: %d\n", list_select_test());
    printf("==========================================\n");
    printf("Tests completed\n");
    printf("==========================================\n");
}