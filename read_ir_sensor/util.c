#include "util.h"

#define ONE_BILLION 1000000000

void diff_timespec (struct timespec * a,
                    struct timespec * b,
                    struct timespec * res)
{
  res->tv_sec = b->tv_sec - a->tv_sec;
  res->tv_nsec = b->tv_nsec - a->tv_nsec;
  if (res->tv_nsec < 0) {
    res->tv_nsec = ONE_BILLION + res->tv_nsec;
  }
}

void copy_timespec(struct timespec * source, struct timespec * dest) {
  dest->tv_sec = source->tv_sec;
  dest->tv_nsec = source->tv_nsec;
}

void add_timespec(struct timespec * sum, struct timespec * val) {
  sum->tv_sec = sum->tv_sec + val->tv_sec;
  sum->tv_nsec = sum->tv_nsec + val->tv_nsec;

  if (sum->tv_nsec > ONE_BILLION) {
    sum->tv_nsec = sum->tv_nsec - ONE_BILLION;
    sum->tv_sec = sum->tv_sec + 1;
  }
}

void clear_timespec(struct timespec * t) {
  t->tv_sec = 0;
  t->tv_nsec = 0;
}
