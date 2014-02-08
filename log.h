#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>

#define DEBUG 3
#define INFO  2
#define WARN  1
#define CRIT  0

#define MAX_LOG_LINE_LENGTH 40
typedef struct log_data {
  int level;
  int abs_index;
  char msg[MAX_LOG_LINE_LENGTH];
} log_data_t;

extern void log_set_log_level(int level);

extern int swiftsure_log_init(void);

extern void swiftsure_log(int, const char *, ...);

extern int swiftsure_log_get_length(void);

extern log_data_t * swiftsure_log_get_message(int);

#endif
