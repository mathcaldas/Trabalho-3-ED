/*Eventos -> Chegada de paciente, inicio/fim dos exames, inicio e fim dos laudos 
*/
#include "log.h"
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
    LogEvent **events; // Capacidade para 1000 eventos (ajuste conforme necessário)
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

/*Eventos -> Chegada de paciente, inicio/fim dos exames, inicio e fim dos laudos 
*/
void log_event(Log *log, const char *message) {
    if (log->count == log->n_max)
        reallocate(log);

    LogEvent *event = (LogEvent *)malloc(sizeof(LogEvent));
    strcpy(event->message, message);
    event->timestamp = time(NULL);
    log->events[log->count] = event;
    log->count++;
}

void save_log_to_file(const Log *log, const char *filename){
    FILE *patients_inf;
    patients_inf = fopen(filename, "w");

    for (int i=0; i < log->count; i++) {
        char *formatted_time = ctime(&log->events[i]->timestamp);
        formatted_time[strcspn(formatted_time, "\n")] = '\0';
        fprintf(patients_inf,"%s | %s\n", formatted_time, log->events[i]->message);
    }
    fclose(patients_inf);
}

void log_free(Log *log) {
    for (int i=0; i < log->count; i++)
        free(log->events[i]);
    free(log->events);
    free(log);
} 