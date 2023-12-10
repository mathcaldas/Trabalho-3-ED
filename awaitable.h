#ifndef AWAITABLE_H
#define AWAITABLE_H

typedef struct awaitable Awaitable;

// Initialize an awaitable task
Awaitable **create_awaitables(int qtd);

// Create and initialize a new exam queue
void initialize_awaitable(Awaitable *a);

// Function to use an awaitable task with a given patient ID and time range
void use_awaitable(Awaitable *a, int patient_id, int mininum, int maximum);

// Check if an awaitable task is available
int is_awaitable_available(Awaitable *a);

// Check if any awaitable task is busy
int is_any_awaitable_busy(Awaitable **a, int qtd);

// Decrease the duration of an awaitable task by one
int awaitable_duration_decrease(Awaitable *a);

// Get the patient ID associated with an awaitable task
int get_awaitable_id(Awaitable *a);

// Free the memory allocated for awaitable tasks
void awaitable_free(Awaitable **a, int qtd);

#endif