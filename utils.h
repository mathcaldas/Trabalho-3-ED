#ifndef UTILS_H
#define UTILS_H

//Generate a random integer between "a" and "b", both included
int gen_randint(int a, int b);

// Calculate the time elapsed since exam initialization
int get_qtime_to_exam(int iteration, int initialization);

// Sleep for a specified number of milliseconds
void ms_sleep(int miliseconds);

#endif