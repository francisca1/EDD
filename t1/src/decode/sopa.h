#pragma once
#include <stdint.h>

/** Estructura de una SOPA */
typedef struct sopa
{
  /** Alto de la sopa */
  int height;
  /** Ancho de la sopa */
  int width;
  /** Matriz de 1s y 0s */
  uint8_t** matrix;
} Sopa;

/** Inicializala la sopa a partir del archivo */
Sopa* sopa_init(char* filepath);

/** Libera la sopa */
void sopa_destroy(Sopa* sopa);
