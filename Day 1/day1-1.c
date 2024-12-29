#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int split(char *str, size_t len, char delim) {
    for (int i = 0; i < len; i++) {
        if (str[i] == delim) {
            return i;
        }
    }

    return -1;
}

void swap(int *array, int p1, int p2) {
    int tmp = array[p1];
    array[p1] = array[p2];
    array[p2] = tmp;
    return;
}

int partition(int *array, int lo, int hi) {
    int pivot = array[hi];

    int i = lo;
    for (int j = lo; j < hi; j++) {
        if (array[j] < pivot) {
            swap(array, i, j);
            i++;
        }
    }

    swap(array, i, hi);
    return i;
}

void quick_sort(int *array, int lo, int hi) {
    if (lo >= hi || lo < 0) {
        return;
    }

    int p = partition(array, lo, hi);
    quick_sort(array, lo, p - 1);
    quick_sort(array, p + 1, hi);
}

int main(void) {
    int list1[1000];
    int list2[1000];

    FILE *f = fopen("./input", "r");

    if (f == NULL) {
        fprintf(stderr, "ERROR: Couldn't open input file.\n");
        exit(1);
    }

    char buff[51];
    char n1_str[25];
    char n2_str[25];

    for (int line = 0; line < 1000; line++) {
        if (!fgets(buff, 50, f)) {
            fprintf(stderr, "ERROR: Error reading from file.\n");
            exit(1);
        }

        int sp_idx = split(buff, strlen(buff), ' ');
        if (sp_idx < 0) {
            fprintf(stderr, "ERROR: you goofed up");
            exit(1);
        }

        strncpy(n1_str, buff, sp_idx);
        strncpy(n2_str, buff + sp_idx + 1, strlen(buff + sp_idx + 1));

        list1[line] = atoi(n1_str);
        list2[line] = atoi(n2_str);
    }

    quick_sort(list1, 0, 999);
    quick_sort(list2, 0, 999);

    int res = 0;
    for (int i = 0; i < 1000; i++) {
        res += abs(list1[i] - list2[i]);
    }

    printf("Answer is %d\n", res);
    return 0;
}