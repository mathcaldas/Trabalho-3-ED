#include "exam.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Struct representing the exam queue
struct exam_queue {
    int n;
    ExamQueueNode *front;
    ExamQueueNode *rear;
};

// Struct representing a node in the exam queue
struct exam_queue_node {
    int patient_id;
    ExamQueueNode *next;
};

// Create and initialize a new exam queue
ExamQueue *exq_create() {
    ExamQueue *q = (ExamQueue *)malloc(sizeof(ExamQueue));
    q->front = q->rear = NULL;
    return q;
}

// Get the length of the exam queue
int exq_length(ExamQueue *q) {
    return q->n;
}

// Check if the exam queue is empty
int exq_is_empty(ExamQueue *q) {
    return exq_length(q) == 0;
}

// Enqueue a new exam into the exam queue
void exq_enqueue(ExamQueue *q, int patient_id) {
    ExamQueueNode *node = (ExamQueueNode *)malloc(sizeof(ExamQueueNode));

    node->patient_id = patient_id;
    node->next = NULL;

    if (exq_is_empty(q))
        q->front = node;
    else
        q->rear->next = node;
    q->rear = node;

    q->n++;
}

// Dequeue an exam from the exam queue
int exq_dequeue(ExamQueue *q) {
    assert(!exq_is_empty(q));

    int v = q->front->patient_id;
    ExamQueueNode *p = q->front;

    if(q->front != q->rear)
        q->front = q->front->next;
    else
        q->front = q->rear = NULL;
    
    free(p);

    q->n--;

    return v;
}

// Free memory allocated for the exam queue
void exq_free(ExamQueue *q) {
    ExamQueueNode *p = q->front;

    while (p != NULL) {
        ExamQueueNode *t = p->next;
        free(p);
        p = t;
    }
    free(q);
}

// Print the patient IDs in the exam queue
void exq_print(ExamQueue *q) {
    for (ExamQueueNode *p = q->front; p != NULL; p = p->next)
        printf("%d ", p->patient_id);
    printf("\n");
}