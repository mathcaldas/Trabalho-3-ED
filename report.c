#include "report.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Struct representing the report queue
struct report_queue{
    int n;
    ReportQueueNode *front;
    ReportQueueNode *rear;
};

//Struct representing a node in the report queue
struct report_queue_node {
    Exam *exam;
    ReportQueueNode *next;
};

// Struct representing an examination
struct exam {
    int patient_id, initialization;
    Condition condition;
};

// Create and Initialize a new report queue
ReportQueue *req_create(){
    ReportQueue *q = (ReportQueue *)malloc(sizeof(ReportQueue));
    q->n = 0;
    q->front = q->rear = NULL;
    return q;
}

//Get the length of the report queue
int req_length(ReportQueue *q) {
    return q->n;
}

// Check if the report queue is empty
int req_is_empty(ReportQueue *q){
    return req_length(q) == 0; 
}

// Generate a random condition for an exam
static Condition gen_condition () {
    int prob = gen_randint(0, 100);
    if (prob <= 30)
        return HEALTHY;
    if (prob <= 50)
        return BRONCHITIS;
    if (prob <= 70)
        return PNEUMONIA;
    if (prob <= 85)
        return FEMUR_FRACTURE;
    else
        return APPENDICITIS;
}

// Enqueue a new exam into the report queue
void req_enqueue(ReportQueue *q, int patient_id, int initialization){
    ReportQueueNode *node = (ReportQueueNode *)malloc(sizeof(ReportQueueNode));
    
    node->exam = (Exam*)malloc(sizeof(Exam));
    
    node->exam->patient_id = patient_id;
    node->exam->initialization = initialization;
    node->exam->condition = gen_condition();
    
    node->next = NULL;

    if (req_is_empty(q))
        q->front = node; 
    else 
        q->rear->next = node;
    q->rear = node;

    q->n++;
}

// Dequeue an exam from the report queue
Exam *req_dequeue(ReportQueue *q) {
    assert(!req_is_empty(q));

    Exam *e = q->front->exam;
    ReportQueueNode *p = q->front;

    if (q->front != q->rear)
        q->front = q->front->next;
    else
        q->front = q->rear = NULL;
    free(p);
    
    q->n--;
    return e;
}

// Get attributes of an exam
void req_get_exam_attributes(Exam *e, int *patient_id, int *initialization, Condition *condition) {
    *patient_id = e->patient_id;
    *initialization = e->initialization;
    *condition = e->condition;
}

// Clear old exams from the report queue
void req_clear(ReportQueue *q, int iteration, int limit, int avg_pathology_time[5], int cont_pathology_exams[5]) {
    while (q->front != NULL && iteration - q->front->exam->initialization >= limit) {
        Exam *e = req_dequeue(q);
        avg_pathology_time[e->condition] += limit;
        cont_pathology_exams[e->condition]++;
        free(e);   
    }
}

// Free memory allocated for the report queue
void req_free(ReportQueue *q) {
    ReportQueueNode *p = q->front; 
    while (p != NULL){
        ReportQueueNode *t = p->next;
        free(p->exam);
        free(p);
        p = t;
    }
    free(q);
}

// Print the patient IDs in the report queue
void req_print(ReportQueue *q) {
    for (ReportQueueNode *p = q->front; p != NULL; p = p->next)
       printf("%d ", p->exam->patient_id);
    printf("\n");
}

// Get the name of a condition as a string
char *get_condition_name(Condition condition) {
    switch (condition) {
        case HEALTHY: return "HEALTHY";
        case BRONCHITIS: return "BRONCHITIS";
        case PNEUMONIA: return "PNEUMONIA";
        case FEMUR_FRACTURE: return "FEMUR_FRACTURE";
        case APPENDICITIS: return "APPENDICITIS";
        default: return "INVALID";
    }
} 