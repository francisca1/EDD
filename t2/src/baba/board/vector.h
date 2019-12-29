#pragma once
#include <stdint.h>

/** Posicion row, col */
typedef struct
{
  uint8_t row;
  uint8_t col;
} Vector;

/** Niega el vector */
Vector vector_negate(Vector vect);

/** Suma 2 vectores */
Vector vector_sum(Vector v1, Vector v2);
