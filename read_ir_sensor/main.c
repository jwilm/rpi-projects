#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "gpio.h"
#include "util.h" // diff_timespec

#define SECOND 1000000

#define WRITE_DELAY 10

// Pin names
#define CLOCK 11
#define A 7
#define B 8
#define CLEAR 9


void init_gpio_input (unsigned pin) {
  INP_GPIO(pin);
}

void init_gpio_output(unsigned pin) {
  INP_GPIO(pin);
  OUT_GPIO(pin);
}



void set_gpio (unsigned pin, unsigned high) {
  if (high == 1) {
     GPIO_SET = 1<<pin;
  } else {
     GPIO_CLR = 1<<pin;
  }
}

unsigned read_gpio(unsigned pin) {
  return (GET_GPIO(pin) >> pin) & 1;
}

void tick () {
  set_gpio(CLOCK, 1);
  usleep(WRITE_DELAY);
  set_gpio(CLOCK, 0);
}

void clear () {
  set_gpio(CLEAR, 0);
  usleep(WRITE_DELAY);
  set_gpio(CLEAR, 1);
}

void write_number (int n) {
  // reinit
  clear();
  set_gpio(A, 1);
  set_gpio(B, 1);

  // Need to work down from largest power of two
  set_gpio(B, (n >> 7) & 1);
  tick();
  set_gpio(B, (n >> 6) & 1);
  tick();
  set_gpio(B, (n >> 5) & 1);
  tick();
  set_gpio(B, (n >> 4) & 1);
  tick();
  set_gpio(B, (n >> 3) & 1);
  tick();
  set_gpio(B, (n >> 2) & 1);
  tick();
  set_gpio(B, (n >> 1) & 1);
  tick();
  set_gpio(B, n & 1);
  tick();
}

int main(int argc, char **argv) {
  // Set up gpi pointer for direct register access
  setup_io();

  // Switch GPIO 7..11 to output mode
  init_gpio_input(CLOCK);

  struct timespec t_prev;
  struct timespec t_now;
  struct timespec t_sum;
  struct timespec t_diff;

  int prev = 1;
  int now = 1;

  // init time structs
  clock_gettime(CLOCK_REALTIME, &t_prev);
  copy_timespec(&t_prev, &t_now);
  clear_timespec(&t_sum);

  for (;;) {
    // Get current state
    now = read_gpio(CLOCK);
    clock_gettime(CLOCK_REALTIME, &t_now);

    if (now != prev) {
      // switched state. Print state and duration active. We log the current
      // state since the IR sensor is _off_ when the led is _on_.
      printf("%d, %3d, %9d\n", now, t_sum.tv_sec, t_sum.tv_nsec);

      // Reset timer
      clear_timespec(&t_sum);

      // Mark that state changed
      prev = now;
    } else {
      // Update sum
      diff_timespec(&t_prev, &t_now, &t_diff);
      add_timespec(&t_sum, &t_diff);
    }

    // Need to update previous time every iteration
    copy_timespec(&t_now, &t_prev);
  }

  return 0;
}
