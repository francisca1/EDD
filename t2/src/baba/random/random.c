#include "random.h"

/** Retorna un numero aleatorio de 64 bits */
uint64_t get_random()
{
  return pcg32_random() + (((uint64_t) pcg32_random()) << 32);
}
