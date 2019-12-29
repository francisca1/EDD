#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "window.h"
#include <math.h>

/** Revisa que la cantidad de parámetros entregada sea correcta */
static bool check_parameters(int argc, char** argv)
{
	return argc == 4;
}

int main(int argc, char** argv)
{
	/* Se revisa que los parametros entregados sean correctos */
  if(!check_parameters(argc, argv)) return 1;

	/* Las dimensiones de la matriz */
	int height = atoi(argv[1]);
	int width = atoi(argv[2]);

	/* Cantidad máxima de distritos */
	int max_districts = atoi(argv[3]);

	/* Inicializa los contenedores */
	Content* cont = drawing_init(height, width, max_districts);

	/* Crea la ventana */
	window_create(cont);

	/* Libera los contenedores */
	drawing_free(cont);

	return 0;
}
