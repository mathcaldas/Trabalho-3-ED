#ifndef EXAM_H
#define EXAM_H

typedef struct exam_queue ExamQueue;
typedef struct exam_queue_node ExamQueueNode;

ExamQueue *exq_create();

// Get the length of the exam queue
int exq_length(ExamQueue *q);

// Check if the exam queue is empty
int exq_is_empty(ExamQueue *q);

// Enqueue a new exam into the exam queue
void exq_enqueue(ExamQueue *q, int patient_id);

// Dequeue an exam from the exam queue
int exq_dequeue(ExamQueue *q);

// Free memory allocated for the exam queue
void exq_free(ExamQueue *q);

// Print the patient IDs in the exam queue
void exq_print(ExamQueue *q);

#endif