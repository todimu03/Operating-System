#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile sig_atomic_t alarm_count = 0;
time_t start_time;

void alarm_handler(int sig) {
    alarm_count++;
    alarm(1);               /* request another SIGALRM in 1 second */
}

void int_handler(int sig) {
    time_t end_time = time(NULL);
    printf("\nTotal execution time: %ld seconds\n", end_time - start_time);
    printf("Number of alarms: %d\n", alarm_count);
    exit(0);
}

int main(void) {
    start_time = time(NULL);

    signal(SIGALRM, alarm_handler);
    signal(SIGINT, int_handler);

    alarm(1);

    while (1) {
        pause();            /* wait for any signal */
    }

    return 0;
}