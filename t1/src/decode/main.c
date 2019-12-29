#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sopa.h"
#include "arbol.h"

/** Funcion que busca la palabra en la sopa de forma naive */
int naive_search(Sopa* sopa, uint8_t* word, int size)
{
	// Contador de repeticiones
	int count = 0;

	// Itero por las posiciones iniciales
	for (int row = 0; row < sopa -> height; row++)
	{
		for (int col = 0; col < sopa -> width; col++)
		{
			// Direcciones
			uint8_t dxs[3] = {0, 1, 1};
			uint8_t dys[3] = {1, 0, 1};

			// Itero por las direcciones
			for (int d = 0; d < 3; d++)
			{
				// Itero por las letras de la palabra viendo si coincide
				for (int l = 0; l < size; l++)
				{
					int r = row + dys[d] * l;
					int c = col + dxs[d] * l;

					// Veo si se sale de la sopa
					if (r >= sopa -> height) break;
					else if (c >= sopa -> width) break;

					// Veo si coincide la letra
					if (sopa -> matrix[r][c] != word[l]) break;

					// Si llego a la letra final hasta aca, sumo 1
					if (l == size - 1) count++;
				}
			}
		}
	}

	// Retorno el contador
	return count;
}

void arbol_from_sopa(Sopa *sopa, Nodo *arbol){
	uint8_t palabra[516];
	

	int count = 0;
	// filas
	for (int i = 0; i < sopa->height; i++)
	{
		count = sopa->width-1;
		for (int j = sopa->width - 1; j >= 0; j--)
		{
			// agrego todos los sufijos de las filas.
			palabra[count] = sopa->matrix[i][j];
			agregar_palabra(arbol, &palabra[count], sopa->width-1 -count + 1);
			count--;
		}
	}

	// columnas
	for (int i = 0; i < sopa->width; i++)
	{
		count = sopa->height-1;
		for (int j = sopa->height - 1; j >= 0; j--)
		{
			// agrego todos los sufijos de las columnas.
			palabra[count] = sopa->matrix[j][i];
			agregar_palabra(arbol, &palabra[count], sopa->height-1 -count + 1);
			count--;
		}
	}
	
	// diagonal superior
	for (int i = sopa->width-1; i >=0; i--)
	{
		count = sopa->height-1;
		for (int j = sopa->width-1; j >i-1 ; j--)
		{
			if (((j - i) < sopa->height) && (j < sopa->width))
			{
	
				palabra[count] = sopa->matrix[j - i][j];
				agregar_palabra(arbol, &palabra[count], sopa->height-1-count + 1);
				count--;
			}
		}
		
	}

	//diagonal inferior
	for (int i = sopa->height-1; i >=0; i--)
	{
	 	count = sopa->height-1;
	 	for (int j = 0; j < i; j++)
	 	{
			palabra[count] = sopa->matrix[sopa->height - j - 1][i - j - 1];
			agregar_palabra(arbol, &palabra[count], sopa->height-1-count + 1);
			count--;
		}
	}
	
	

}
/** Metodo main del programa */
int main(int argc, char *argv[])
{
	/** El codigo base contiene la solucion al problema con el metodo naive
	de busqueda en la sopa. Tu objetivo es optimizarlo para que sea mas eficiente
	usando un Trie */

	// Revisar los parametros
	if (argc != 4)
	{
		printf("Modo de uso: ./decode <input_image.png> <queries.txt> <output.txt>\n");
		return 0;
	}

	// Cargo la sopa desde la imagen
	Sopa* sopa = sopa_init(argv[1]);
	Nodo *suffix_tree = arbol_init(20);

	// Abro el archivo de palabras y el de output
	FILE* words_file = fopen(argv[2], "r");
	FILE* output_file = fopen(argv[3], "w");
	arbol_from_sopa(sopa, suffix_tree);
	// Leo el numero de palabras desde el archivo
	int word_count;
	fscanf(words_file, "%d", &word_count);

	// Creo un arreglo de tamanio 512 (tamanio maximo de las palabras)
	uint8_t word[512];

	// Leo palabra a palabra
	for (int w = 0; w < word_count; w++)
	{
	// Leo el tamanio de la palabra
	 	int size;
	 	fscanf(words_file, "%d", &size);

	// Leo las letras de la palabra de la una
	 	for (int l = 0; l < size; l++)
	 	{
	 		fscanf(words_file, "%hhu", &word[l]);
	 	}

	// Cuento las ocurrencias de la palabra usando el metodo naive
		int count = buscar_palabra(word, size, suffix_tree, 0);
		// int count = naive_search(sopa, word, size);

		// Escribo el contador en el archivo de output
	 	fprintf(output_file, "%d\n", count);
	 }

	// Cierro los archivos
	fclose(words_file);
	fclose(output_file);

	// // Libero la sopa
	sopa_destroy(sopa);
	destruir_arbol(suffix_tree);
	// Retornar 0 significa que el programa termina sin errores
	return 0;
}