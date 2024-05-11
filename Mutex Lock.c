#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 100

char Input[MAX_LENGTH];
char Output[MAX_LENGTH];

pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;

void* convert_Case(void* arg) {
    int* index = (int*)arg;
    while (1) {
        pthread_mutex_lock(&Mutex);
        int i = (*index)++;
        pthread_mutex_unlock(&Mutex);

        if (i >= strlen(Input)) {
            break;
        }
        
        if (islower(Input[i]))
            Output[i] = toupper(Input[i]);
        else if (isupper(Input[i]))
            Output[i] = tolower(Input[i]);
        else
            Output[i] = Input[i];
    }
    return NULL;
}

int main() {
    int n;
    printf("Enter a sentence: ");
    fgets(Input, MAX_LENGTH, stdin);

    printf("Enter the number of threads: ");
    scanf("%d", &n);

    pthread_t threads[n];
    int thread_args[n];

    int i;
    for (i = 0; i < n; i++) {
        thread_args[i] = 0;
        pthread_create(&threads[i], NULL, convert_Case, &thread_args[i]);
    }

    for (i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Converted sentence: %s\n", Output);

    return 0;
}
