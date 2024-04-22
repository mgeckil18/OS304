#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_NUMBERS 1000

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s number n\n", argv[0]);
        return 1;
    }

    int target = atoi(argv[1]);
    int n = atoi(argv[2]);
    int segment_size = MAX_NUMBERS / n;

   
    int shmid = shmget(IPC_PRIVATE, MAX_NUMBERS * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }
    int *data = (int *)shmat(shmid, NULL, 0);
    if (data == (int *)(-1)) {
        perror("shmat");
        return 1;
    }

     
    for (int i = 0; i < MAX_NUMBERS && scanf("%d", &data[i]) != EOF; i++);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
           
            int start_index = i * segment_size;
            int end_index = (i == n-1) ? MAX_NUMBERS : start_index + segment_size;
            for (int j = start_index; j < end_index; j++) {
                if (data[j] == target) {
                    printf("Number %d found at index %d by process %d\n", target, j, getpid());
                    exit(0);
                }
            }
            exit(1); 
        }
    }

    int status = 0;
    pid_t child_pid;
    while ((child_pid = wait(&status)) > 0) {
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Child %d found the number. Terminating other processes.\n", child_pid);
            
        }
    }

   
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
