#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100

int array[MAX_SIZE];

typedef struct {
    int start;
    int end;
} Thread_Arguments;

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void Merge_Sorting(int initial, int middle, int ending) {
    int left_half_length = middle - initial + 1;
    int right_half_length = ending - middle;

    int left_half_portion[left_half_length];
    int right_half_portion[right_half_length];

    for (int i = 0; i < left_half_length; i++)
        left_half_portion[i] = array[initial + i];

    for (int j = 0; j < right_half_length; j++)
        right_half_portion[j] = array[middle + 1 + j];

    int i = 0, j = 0, k = initial;

    while (i < left_half_length && j < right_half_length) {
        if (left_half_portion[i] <= right_half_portion[j]) {
            array[k] = left_half_portion[i];
            i++;
        } else {
            array[k] = right_half_portion[j];
            j++;
        }
        k++;
    }

    while (i < left_half_length) {
        array[k] = left_half_portion[i];
        i++;
        k++;
    }

    while (j < right_half_length) {
        array[k] = right_half_portion[j];
        j++;
        k++;
    }
}

void* sorting_half_portion(void* args) {
    Thread_Arguments* arg = (Thread_Arguments*) args;
    int start = arg->start;
    int end = arg->end;

    qsort(&array[start], end - start, sizeof(int), compare);

    return NULL;
}

void* merge_sorted_arrays(void* args) {
    int mid = *((int*) args);

    Merge_Sorting(0, mid - 1, MAX_SIZE - 1);

    return NULL;
}

int main() {
    int n;
    printf("Enter the number of Elements: ");
    scanf("%d", &n);

    printf("Enter the Elements: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &array[i]);

    pthread_t thread1, thread2;
    Thread_Arguments args1 = {0, n / 2};
    Thread_Arguments args2 = {n / 2, n};

    pthread_create(&thread1, NULL, sorting_half_portion, &args1);
    pthread_create(&thread2, NULL, sorting_half_portion, &args2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_t merge_thread;
    int mid = n / 2;
    pthread_create(&merge_thread, NULL, merge_sorted_arrays, &mid);
    pthread_join(merge_thread, NULL);

    printf("Sorted elements: ");
    for (int i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("\n");

    return 0;
}
