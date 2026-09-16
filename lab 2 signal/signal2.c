#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t flag = 0;

void handler(int sig) {
    printf("Hello World!\n");
    flag = 1;
    alarm(5);               /* schedule the next SIGALRM */
}

int main(void) {
    signal(SIGALRM, handler);
    alarm(5);

    while (1) {
        if (flag) {
            printf("Turing was right!\n");
            flag = 0;
        }
    }

    return 0;
}