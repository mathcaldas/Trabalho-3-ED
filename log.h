#ifndef LOG_H
#define LOG_H

typedef struct LogEvent;
typedef struct Log;

Log* create_log();

void log_event(Log *log, const char *message);

void save_log_to_file(const Log *log, const char *filename);

#endif