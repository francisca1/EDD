#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "../watcher/watcher.h"
#include "../puzzle/cell_type.h"
#include "declaraciones.h"

int backtracking(int *celdas, int* kodos, int*jack, int contador, int **values, CellType **cells, int height, int width, int n_distritos, int n_pers, int *p_actual_dist, int pers_por_dist);

int main(int argc, char **argv)
{
	/* Revisamos que los parámetros sean correctos */
	if(argc != 2)
	{
		printf("Uso: %s <test>\nDonde\n", argv[0]);
		printf("\t<test> es la ruta al archivo a resolver\n");
	}

	// El siguiente codigo es un ejemplo de como usar la interfaz y como abrir
	// los archivos de test. Este codigo tiene sleeps para poder ver lo que pasa
	// pero al enviar tu codigo no puede tener sleeps ya que puede hacer que
	// termine fuera de los 10 segundos

	/* Abrimos el test */
	FILE* test = fopen(argv[1], "r");

	// Leemos las dimensiones del tablero
	int height, width;
	fscanf(test, "%d %d", &height, &width);

	// Leemos el numero de distritos
	int district_count;
	fscanf(test, "%d", &district_count);

	int personas = 0; // número de personas totales
	int contador = 0;
	// Creamos una matriz de votantes y una de asignaciones de distritos
	CellType** cells = malloc(sizeof(CellType*) * height);
	int** values = malloc(sizeof(int*) * height);
	register int row = 0;
	register int col = 0;

	for (row = 0; row < height; row++)
	{
		cells[row] = malloc(sizeof(CellType) * width);
		values[row] = malloc(sizeof(int) * width);
		
		for (col = 0; col < width; col++)
		{
			// Leo el valor de la celda en el archivo y la asigno a la matriz
			fscanf(test, "%u", &cells[row][col]);
			if (cells[row][col] != 0){
				personas += 1;
			}

			// Dejo todas las celdas sin asignar
			values[row][col] = 0;
		}
	}

	/* Cerramos el archivo de test*/
	fclose(test);

	int pers_por_dist = personas / district_count;
	printf("personas por distrito = %d\n", pers_por_dist);
	int *p_actual_dist = calloc(district_count, sizeof(int));

	// lista con los botos por kodos en cada distrito
	int *kodos = calloc(district_count, sizeof(int));
	int *jack = calloc(district_count, sizeof(int));
	int *celdas = calloc(district_count, sizeof(int));

	// Abrimos la interfaz
	watcher_open(height, width, district_count);

	// Iteramos por cells agregando los elementos de la matriz a la interfaz
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			watcher_set_cell_type(row, col, cells[row][col]);
		}
	}


	if (backtracking(celdas, kodos, jack, contador, values, cells, height, width, district_count, personas, p_actual_dist, pers_por_dist))
	{
		printf("hay solución\n");
		for (row = 0; row < height; row++)
		{
			for (col = 0; col < width; col++)
			{
				// le asigno el valor de distrito
				//printf("%d  ", values[row][col]);
				watcher_set_cell_district(row, col, values[row][col]);
			}
		}
	}else{
		printf("no hay solucion uwu\n");
	}

	// Cerramos la interfaz
	sleep(1); // si saco este sleep no se cierra la ventana ni termina el programa.
	watcher_close();

	// Liberamos las matrices que creamos
	for (row = 0; row < height; row++)
	{
		free(cells[row]);
		free(values[row]);
	}
	free(cells);
	free(values);
	free(p_actual_dist);
	free(kodos);
	free(jack);
	free(celdas);
	/* Retornamos 0 indicando que todo salió bien */
	return 0;
}


int backtracking(int *celdas, int *kodos, int* jack, int contador, int ** values, CellType** cells, int height, int width, int n_distritos, int n_pers, int* p_actual_dist, int pers_por_dist){

	if(es_solucion(kodos, jack, contador, values, cells, height, width, pers_por_dist, n_distritos)){
		return 1;
	}

	celda *cel = malloc(sizeof(celda));

	elegir_celda(values, height, width, cel);
	int r = cel->row;
	int c = cel->col;
	free(cel);

	int *revisados = calloc(n_distritos, sizeof(int));
	int dist;
	while (!rev(revisados, n_distritos))
	{
		dist = elegir_num(revisados, n_distritos);
		// elegir distrito de forma random

		if (is_valido(celdas, kodos, jack, values, cells, n_distritos, dist, r, c, p_actual_dist, pers_por_dist, height, width))
		{
			values[r][c] = dist;
			contador += 1;
			sumar_voto(dist, cells, kodos, jack, r, c);
			//watcher_set_cell_district(r, c, dist);
			celdas[dist - 1] += 1;
		
			if (cells[r][c] != 0)
			{
				p_actual_dist[dist - 1] += 1;
			}

			if (backtracking(celdas, kodos, jack, contador, values, cells, height, width, n_distritos, n_pers, p_actual_dist, pers_por_dist))
			{
				free(revisados);
				return 1;
			}

			values[r][c] = 0;
			contador -= 1;
			restar_voto(dist, cells, kodos, jack, r, c);
			// watcher_set_cell_district(r, c, 0);
			celdas[dist - 1] -= 1;

			if (cells[r][c] != 0)
			{
				p_actual_dist[dist - 1] -= 1;
			}
		}
		revisados[dist - 1] = 1;
	}
	free(revisados);
	return 0;
}