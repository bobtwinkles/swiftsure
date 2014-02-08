#include "log.h"

#include <stdio.h>

static int log_level = 9;
static FILE * log_file;

int swiftsure_log_init(void) {
  log_file = fopen("log.log.loggity", "w");
  if (!log_file) {return -1;}
  return 0;
}

void swiftsure_log(int level, const char * fmt, ...) {
  va_list args;

  if (level > log_level) return;

  va_start(args, fmt);

  vfprintf(log_file, fmt, args);

  va_end(args);
}
