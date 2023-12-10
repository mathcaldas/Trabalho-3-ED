#include "log.h"
#include <time.h>

typedef struct {
    char message[256];
    time_t timestamp;
} LogEvent;

typedef struct {
    LogEvent events[1000]; // Capacidade para 1000 eventos (ajuste conforme necessário)
    int count;
} Log;

Log* create_log();


void log_event(Log *log, const char *message);


void save_log_to_file(const Log *log, const char *filename);



