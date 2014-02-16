#include "log.h"

#include <stdio.h>

#include "gamedefs.h"

#define MAX_LOG_SIZE 32

static int log_level = 9;
static FILE * log_file;
static log_data_t the_log[MAX_LOG_SIZE];
static int log_start;
static int log_length;
static int log_abs_position;

int swiftsure_log_init(void) {
  log_file = fopen("log.log.loggity", "w");
  if (!log_file) {return -1;}
  log_start = 0;
  log_length = 0;
  log_abs_position = 0;
  swiftsure_log(INFO, "Max log line length is %d characters\n", MAX_LOG_LINE_LENGTH);
  return 0;
}

void swiftsure_log(int level, const char * fmt, ...) {
  va_list args;

  if (level > log_level) return;

  va_start(args, fmt);

  //Print to internal log buffer
  vsnprintf((char*)&the_log[log_start].msg, MAX_LOG_LINE_LENGTH, fmt, args);
  the_log[log_start].level = level;
  the_log[log_start].abs_index = log_abs_position++;
  //And to the log file
  fprintf(log_file, "%03d:%05d:%s", frame, the_log[log_start].abs_index, the_log[log_start].msg);
  //And to standard out because why not
  printf("%03d:%05d:%s", frame, the_log[log_start].abs_index, the_log[log_start].msg);

  va_end(args);

  log_start = (log_start + 1) % MAX_LOG_SIZE;
  if (log_length < MAX_LOG_SIZE) {
    log_length += 1;
  }

}

log_data_t * swiftsure_log_get_message(int msg) {
  int index = msg + log_start - log_length;
  if (index < 0) {index += MAX_LOG_SIZE;}
  index %= MAX_LOG_SIZE;
  return the_log + index;
}

int swiftsure_log_get_length(void) { return log_length; }
