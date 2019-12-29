#include "vector.h"

/** Niega el vector */
Vector vector_negate(Vector vect)
{
  return (Vector)
  {
    .row = -vect.row,
    .col = -vect.col
  };
}

/** Suma 2 vectores */
Vector vector_sum(Vector v1, Vector v2)
{
  return (Vector)
  {
    .row = v1.row + v2.row,
    .col = v1.col + v2.col
  };
}
