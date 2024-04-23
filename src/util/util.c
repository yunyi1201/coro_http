#include "util.h"

#include <time.h>

struct tm *get_tm() {
  time_t t;

  time(&t);
  return localtime(&t);
}

long long get_curr_mseconds() {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}