#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  

    if (pid == -1) {
        perror("fork"); 
        return 1;
    }

    if (pid == 0) {
        
        printf("Child process ID: %d\n", getpid());
        exit(0);  
    } else {
        
        printf("Parent process ID: %d, Child ID: %d\n", getpid(), pid);
        sleep(5);  
        printf("Parent process terminates after 5 seconds\n");
    }

    return 0;
}
