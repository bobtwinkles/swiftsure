#include "log.h"

#include <stdio.h>

#define MAX_LOG_SIZE 32

static int log_level = 9;
static FILE * log_file;
static log_data_t the_log[MAX_LOG_SIZE];
static int log_start;
static int log_end;
static int log_length;
static int log_abs_position;

int swiftsure_log_init(void) {
  log_file = fopen("log.log.loggity", "w");
  if (!log_file) {return -1;}
  log_start = 0;
  log_end = 0;
  log_length = 0;
  log_abs_position = 0;
  return 0;
}

void swiftsure_log(int level, const char * fmt, ...) {
  va_list args;

  if (level > log_level) return;

  va_start(args, fmt);

  vfprintf(log_file, fmt, args);

  //We need to restart the args because vfprintf stomps on them
  va_end(args);
  va_start(args, fmt);

  //Print to internal log buffer
  vsnprintf((char*)&the_log[log_end].msg, MAX_LOG_LINE_LENGTH, fmt, args);
  the_log[log_end].level = level;
  the_log[log_end].abs_index = log_abs_position++;

  va_end(args);

  log_end = (log_end + 1) % MAX_LOG_SIZE;
  if (log_end == log_start) {
    log_start = (log_start + 1) % MAX_LOG_SIZE;
  }
  if (log_length < MAX_LOG_SIZE) {
    log_length += 1;
  }

}

log_data_t * swiftsure_log_get_message(int msg) {
  int index = (msg + log_start - 1) % MAX_LOG_SIZE;
  return the_log + index;
}

int swiftsure_log_get_length(void) { return log_length; }
