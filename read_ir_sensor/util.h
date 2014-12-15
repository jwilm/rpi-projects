#ifndef UTIL_H
#define UTIL_H

#include <time.h>

/**
 * Compute timespec difference b - a and store it in res
 */

void diff_timespec(struct timespec * a,
                    struct timespec * b,
                    struct timespec * res);

/**
 * Copy source to dest
 */

void copy_timespec(struct timespec * source, struct timespec * dest);

/**
 * Add timespec val to sum
 */

void add_timespec(struct timespec * sum, struct timespec * val);

/**
 * reset a timespec
 */

void clear_timespec(struct timespec * t);

#endif
