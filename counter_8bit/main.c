#include <stdio.h>
#include <unistd.h>
#include "gpio.h"

#define SECOND 1000000

#define WRITE_DELAY 70

// Pin names
#define CLOCK 11
#define A 7
#define B 8
#define CLEAR 9


void init_gpio (unsigned pin) {
  // Set GPIO pins 7-11 to output
  INP_GPIO(pin); // must use INP_GPIO before we can use OUT_GPIO
  OUT_GPIO(pin);
}

void set_gpio (unsigned pin, unsigned high) {
  if (high == 1) {
     GPIO_SET = 1<<pin;
  } else {
     GPIO_CLR = 1<<pin;
  }
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

int main(int argc, char **argv)
{
  int i;
  // Set up gpi pointer for direct register access
  setup_io();

  // Switch GPIO 7..11 to output mode
  init_gpio(CLOCK);
  init_gpio(CLEAR);
  init_gpio(A);
  init_gpio(B);

  for (;;) {
    for (i=0; i!=256; i++) {
      //printf("%d -", i);
      write_number(i);
      //printf("\n");
      usleep(250000);
    }
  }

  return 0;

}
