#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2];
    pid_t child1, child2;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    if ((child1 = fork()) == -1) {
        perror("First fork failed");
        exit(EXIT_FAILURE);
    }

    if (child1 == 0) { 
        close(pipe1[0]); 
        close(pipe2[0]); 

        while (1) {
            char input;
            printf("Enter a character (Ctrl+C to exit): ");
            scanf(" %c", &input);
            write(pipe1[1], &input, sizeof(char)); 
        }
    }

    if ((child2 = fork()) == -1) {
        perror("Second fork failed");
        exit(EXIT_FAILURE);
    }

    if (child2 == 0) { 
        close(pipe1[1]); 
        close(pipe2[0]); 

        while (1) {
            char input;
            read(pipe1[0], &input, sizeof(char)); 
            input++;
            write(pipe2[1], &input, sizeof(char)); 
        }
    }

    close(pipe1[0]); 
    close(pipe1[1]); 
    close(pipe2[1]); 

    while (1) {
        char output;
        if (read(pipe2[0], &output, sizeof(char)) <= 0) { 
            printf("\nChild processes terminated\n");
            break;
        }

        output--;
        printf("Output: %c\n", output);
    }

    wait(NULL);
    wait(NULL);

    return 0;
}
