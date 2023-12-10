#include "awaitable.h"
#include "utils.h"
#include <stdlib.h>

// Define the struct to represent an awaitable task
struct awaitable {
    int patient_id, duration;
};

// Initialize an awaitable task
void initialize_awaitable(Awaitable *a) {
    a->duration = 0;
}

// Set parameters for an awaitable task
Awaitable **create_awaitables(int qtd) {
    Awaitable **a = (Awaitable **)malloc(qtd*sizeof(Awaitable *));
    for (int i=0; i < qtd; i++) {
        a[i] = (Awaitable *)malloc(sizeof(Awaitable));
        initialize_awaitable(a[i]);
    }
    return a;
}

// Set parameters for an awaitable task
void use_awaitable(Awaitable *a, int patient_id, int mininum, int maximum) {
    a->patient_id = patient_id;
    a->duration = gen_randint(mininum, maximum);
}

// Check if an awaitable task is available
int is_awaitable_available(Awaitable *a){
    return a->duration == 0;
}

// Check if any awaitable task is busy
int is_any_awaitable_busy(Awaitable **a, int qtd) {
    for (int i=0; i < qtd; i++)
        if (!is_awaitable_available(a[i]))
            return 1;
    return 0;
}

// Decrease the duration of an awaitable task by one
int awaitable_duration_decrease(Awaitable *a) {
    return --a->duration;
}

// Get the patient ID associated with an awaitable task
int get_awaitable_id(Awaitable *a){
    return a->patient_id;
}

// Free the memory allocated for awaitable tasks
void awaitable_free(Awaitable **a, int qtd) {
    for (int i=0; i < qtd; i++)
        free(a[i]);
    free(a);
}