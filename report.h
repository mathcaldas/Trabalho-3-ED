#ifndef REPORT_H
#define REPORT_H

typedef struct report_queue ReportQueue;
typedef struct report_queue_node ReportQueueNode;
typedef struct exam Exam;

// Enumeration of possible medical conditions
typedef enum {
    HEALTHY,
    BRONCHITIS,
    PNEUMONIA,
    FEMUR_FRACTURE,
    APPENDICITIS,
} Condition;

// Create and Initialize a new report queue
ReportQueue *req_create();

//Get the length of the report queue
int req_length(ReportQueue *q);

// Check if the report queue is empty
int req_is_empty(ReportQueue *q);

// Enqueue a new exam into the report queue
void req_enqueue(ReportQueue *q, int patient_id, int initialization);

// Dequeue an exam from the report queue
Exam *req_dequeue(ReportQueue *q);

// Get attributes of an exam
void req_get_exam_attributes(Exam *e, int *patient_id, int *initialization, Condition *condition);

// Clear old exams from the report queue
void req_clear(ReportQueue *q, int iteration, int limit, int avg_pathology_time[5], int cont_pathology_exams[5]);

// Free memory allocated for the report queue
void req_free(ReportQueue *q);

// Print the patient IDs in the report queue
void req_print(ReportQueue *q);

// Get the name of a condition as a string
char *get_condition_name(Condition condition);

#endif




