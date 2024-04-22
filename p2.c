#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s n command\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    char **command = &argv[2];

    int pipes[n][2];
    struct timeval start, end;

    for (int i = 0; i < n; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            exit(1);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            close(pipes[i][0]);
            gettimeofday(&start, NULL);
            execvp(command[0], command);
            gettimeofday(&end, NULL);

            double time_taken = (end.tv_sec - start.tv_sec) * 1e6;
            time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-3;

            write(pipes[i][1], &time_taken, sizeof(time_taken));
            close(pipes[i][1]);
            exit(0);
        } else {
            close(pipes[i][1]); 
        }
    }

    double times[n];
    double max_time = 0, min_time = 1e6, sum_time = 0;
    for (int i = 0; i < n; i++) {
        read(pipes[i][0], &times[i], sizeof(times[i]));
        close(pipes[i][0]);

        if (times[i] > max_time) max_time = times[i];
        if (times[i] < min_time) min_time = times[i];
        sum_time += times[i];
    }

    double avg_time = sum_time / n;
    printf("Max: %.2f millis\n", max_time);
    printf("Min: %.2f millis\n", min_time);
    printf("Average: %.2f millis\n", avg_time);

    while (wait(NULL) > 0); 

    return 0;
}
