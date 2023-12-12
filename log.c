/* Events -> Patient arrival, start/end of exams, start and end of reports */
#include "log.h"
#include "report.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

struct logevent {
    char message[256];
    time_t timestamp;
};

struct log {
    LogEvent **events; // Capacity for 1000 events (adjust as necessary)
    int n_max;
    int count;
};

Log* create_log() {
    Log *l = (Log *)malloc(sizeof(Log));
    l->count = 0;
    l->n_max = 1000;
    l->events = (LogEvent**)malloc(l->n_max*sizeof(LogEvent *));
    return l;
} 

static void reallocate(Log *log) {
    log->n_max *= 2;
    log->events = realloc(log->events, log->n_max*sizeof(LogEvent *));
}

/* Events -> Patient arrival, start/end of exams, start and end of reports */
void log_event(Log *log, const char *message) {
    if (log->count == log->n_max)
        reallocate(log);

    LogEvent *event = (LogEvent *)malloc(sizeof(LogEvent));
    strcpy(event->message, message);

    event->timestamp = time(NULL);
    log->events[log->count] = event;
    log->count++;
}

void save_log_to_file(const Log *log, const char *filename, float exam_time_result, float avg_pathology_time[5], int passed_limit_time, int patient_amount){
    FILE *log_file;
    log_file = fopen(filename, "w");

    for (int i=0; i < log->count; i++) {
        char *formatted_time = ctime(&log->events[i]->timestamp);
        formatted_time[strcspn(formatted_time, "\n")] = '\0';
        fprintf(log_file,"%s | %s\n", formatted_time, log->events[i]->message);
    }

    // Save metrics
    fprintf(log_file, "\nAverage waiting time for report: %.2f\n\n", exam_time_result);
    
    fprintf(log_file, "Average time per condition:\n");
    for (int i=HEALTHY; i <= APPENDICITIS; i++)
        fprintf(log_file, "%s: %.2f\n", get_condition_name(i), avg_pathology_time[i]);
    fprintf(log_file, "\n");
    fprintf(log_file, "Patients exceeding the time limit: %d\n", passed_limit_time);
    fprintf(log_file, "\nNUMBER OF PATIENTS IN THE LIST: %d\n", patient_amount);

    fclose(log_file);
}

void log_free(Log *log) {
    for (int i=0; i < log->count; i++)
        free(log->events[i]);
    free(log->events);
    free(log);
} 