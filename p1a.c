#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of forks>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 0) {
        fprintf(stderr, "Number of forks must be non-negative\n");
        return 1;
    }

    int level = 0; 

    printf("Main Process ID: %d, level: %d\n", getpid(), level);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            level++; 
            printf("Process ID: %d, Parent ID: %d, level: %d\n", getpid(), getppid(), level);
            
            // break;
        }
    }

    if (level == 0) {
        for (int i = 0; i < n; i++) {
            wait(NULL);
        }
    }

    return 0;
}
