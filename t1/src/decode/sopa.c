#include <stdlib.h>
#include "sopa.h"
#include "../imagelib/imagelib.h"

/** Inicializala la sopa a partir del archivo */
Sopa* sopa_init(char* filepath)
{
  // Creo la sopa
  Sopa* sopa = malloc(sizeof(Sopa));

  // Abro la imagen
  Image* image = img_png_read_from_file(filepath);

  // Le doy las dimensiones a la matriz
  sopa -> height = image -> height;
  sopa -> width = image -> width;

  // Creo la matriz
  sopa -> matrix = malloc(sizeof(uint8_t*) * sopa -> height);
  for (int row = 0; row < sopa -> height; row++)
  {
    sopa -> matrix[row] = malloc(sizeof(uint8_t) * sopa -> width);
  }

  // Asigno los valores de la matriz a partir de la imagen
  for (int row = 0; row < sopa -> height; row++)
  {
    for (int col = 0; col < sopa -> width; col++)
    {
      if (image -> pixels[row][col].R != 255)
      {
        sopa -> matrix[row][col] = 0;
      }
      else if (image -> pixels[row][col].G != 255)
      {
        sopa -> matrix[row][col] = 0;
      }
      else if (image -> pixels[row][col].B != 255)
      {
        sopa -> matrix[row][col] = 0;
      }
      else
      {
        sopa -> matrix[row][col] = 1;
      }
    }
  }

  // Libero la imagen
  img_png_destroy(image);

  // Retorno la sopa
  return sopa;
}

/** Libera la sopa */
void sopa_destroy(Sopa* sopa)
{
  // Libero la matriz de 1s y 0s
  for (int row = 0; row < sopa -> height; row++)
  {
    free(sopa -> matrix[row]);
  }
  free(sopa -> matrix);

  // Libero la sopa
  free(sopa);
}
