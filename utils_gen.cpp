#include "utils_gen.h"

uint8_t op_mode = MODE_RUN;

uint8_t get_op_mode(void)
{
  return op_mode;
}

void set_op_mode(uint8_t mode)
{
  op_mode = mode;
}
