#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(void)
{
    pid_t pid1, pid2;
    int status;

    /* Create first child */
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork child 1 failed");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        /* Child 1 */
        srandom((unsigned int)(time(NULL) ^ getpid()));

        int iterations = random() % 30 + 1;  /* 1 to 30 */

        for (int i = 0; i < iterations; i++) {
            printf("Child Pid: %d is going to sleep!\n", getpid());
            fflush(stdout);

            sleep(random() % 10 + 1);        /* 1 to 10 seconds */

            printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n",
                   getpid(), getppid());
            fflush(stdout);
        }

        exit(0);
    }

    /* Create second child */
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork child 2 failed");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        /* Child 2 */
        srandom((unsigned int)(time(NULL) ^ getpid()));

        int iterations = random() % 30 + 1;  /* 1 to 30 */

        for (int i = 0; i < iterations; i++) {
            printf("Child Pid: %d is going to sleep!\n", getpid());
            fflush(stdout);

            sleep(random() % 10 + 1);        /* 1 to 10 seconds */

            printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n",
                   getpid(), getppid());
            fflush(stdout);
        }

        exit(0);
    }

    /* Parent waits for both children */
    for (int i = 0; i < 2; i++) {
        pid_t completed_pid = wait(&status);

        if (completed_pid < 0) {
            perror("wait failed");
            exit(EXIT_FAILURE);
        }

        printf("Child Pid: %d has completed\n", completed_pid);
        fflush(stdout);
    }

    return 0;
}