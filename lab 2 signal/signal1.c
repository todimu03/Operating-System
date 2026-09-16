#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t flag = 0;

void handler(int sig) {
    printf("Hello World!\n");
    flag = 1;
}

int main(void) {
    signal(SIGALRM, handler);
    alarm(5);

    while (!flag) {
        /* busy wait for SIGALRM */
    }

    printf("Turing was right!\n");
    return 0;
}