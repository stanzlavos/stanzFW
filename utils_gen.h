#ifndef __UTILS_GEN__
#define __UTILS_GEN__

#include <Arduino.h>
#include "utils_defs.h"

static inline void blink_led(uint8_t cnt) {
  for (int i = 0; i < cnt; i++) {
    digitalWrite(2, LOW);
    delay(500);
    digitalWrite(2, HIGH);
    delay(500);
  }
}

uint8_t get_op_mode(void);
void set_op_mode(uint8_t);

#endif // __UTILS_GEN__
