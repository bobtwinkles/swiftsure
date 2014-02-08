#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>

#define DEBUG 9
#define INFO  5
#define WARN  1
#define CRIT  0

extern void log_set_log_level(int level);

extern int swiftsure_log_init(void);

extern void swiftsure_log(int, const char *, ...);

#endif
