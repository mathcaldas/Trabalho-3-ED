#ifndef LOG_H
#define LOG_H

typedef struct logevent LogEvent;
typedef struct log Log;

//Function to create a new log structure.
Log* create_log();

//Function to log an event in the specified log.
void log_event(Log *log, const char *message);

// Function to save log to a file, and write metrics.
void save_log_to_file(const Log *log, const char *filename, float exam_time_result, float avg_pathology_time[5], int passed_limit_time, int patient_amount);

//Function to free the memory allocated for a log, including its events.
void log_free(Log *log);

#endif