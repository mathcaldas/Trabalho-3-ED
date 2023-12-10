#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

//Generate a random integer between "a" and "b", both included
int gen_randint(int a, int b) {
    return a + rand()%(b - a + 1);
}
// Calculate the time elapsed since exam initialization
int get_qtime_to_exam(int iteration, int initialization){
    return iteration - initialization;
}
// Sleep for a specified number of milliseconds
void ms_sleep(int miliseconds) {
    assert(miliseconds >= 0);
    struct timespec request;

    request.tv_sec = miliseconds/1000.0;
    request.tv_nsec = (miliseconds%1000)*1000000;

    nanosleep(&request, NULL);
}