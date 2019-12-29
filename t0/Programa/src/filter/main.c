#include <stdio.h>
#include <stdlib.h>
#include "../imagelib/imagelib.h"
#include "declaraciones.h"

int main(int argc, char *argv[])
{
	/* El programa recibe 2 parametros */
	if(argc != 3)
	{
		printf("Modo de uso: %s <input.png> <output.png>\n", argv[0]);
		printf("\t<input.png> es la imagen a filtrar\n");
		printf("\t<output.png> es donde se guardará la imagen resultante\n");
		return 1;
	}

	/* Leemos la imagen a partir del archivo */
	char* input_file = argv[1];
	Image* img = img_png_read_from_file(input_file);

	// INICIALIZA LA NUEVA IMGEN
	Image *new_img = malloc(sizeof(Image));
	new_img->pixels = calloc(img->height, sizeof(Color *));
	new_img->height = img->height;
	new_img->width = img->width;
	for (int row = 0; row < img->height; row++)
	{
		new_img->pixels[row] = calloc(img->width, sizeof(Color));
		for (int col = 0; col < img->width; col++)
		{
		new_img->pixels[row][col].R = 0;
		new_img->pixels[row][col].G = 0;
		new_img->pixels[row][col].B = 0;
		}
  	}


	/* TODO: Filtro de mediana */
	Color *new = malloc(sizeof(Color));
	new->R = 0;
	new->G = 0;
	new->B = 0;
	int blancos = 0;
	/* A modo de ejemplo, damos invertimos los colores de la imagen */
	for (int row = 0; row < img -> height; row++)
	{
		for (int col = 0; col < img -> width; col++)
		{
			if (is_white(img->pixels[row][col]))
			{
				blancos++;
				// printf("es blanco \n");
				if (is_corner(row, col, img->width, img->height))
				{
					new_color(new, img, row, col, 5);

				} else if (is_up(row, col, img->width, img->height))
				{
					// printf("arriba\n");
					new_color(new, img, row, col, 1);

					// free(new);
					// printf("el color es R: %d, G: %d, B: %d\n", new->R, new->G, new->B);
		
				} else if (is_down(row, col, img->width, img->height))
				{
					// printf("abajo\n");
					new_color(new, img, row, col, 2);

					// free(new);
				
					// printf("abajo\n");
				} else if (is_right(row, col, img->width, img->height))
				{
					// printf("derecha\n");
					new_color(new, img, row, col, 4);

					// free(new);
					
					// printf("derecha\n");
				} else if (is_left(row, col, img->width, img->height))
				{
					// printf("izquierda\n");
					new_color(new, img, row, col, 3);

					// free(new);
					
					// printf("izquierda\n");
				} else{
					// printf("normal\n");
					new_color(new, img, row, col, 0);

					// printf("el color es R: %d, G: %d, B: %d\n", new->R, new->G, new->B);
					// printf("normal\n");
				}
				
			} else{
				new ->R = img -> pixels[row][col].R;
				new -> G = img -> pixels[row][col].G;
				new ->B = img -> pixels[row][col].B;
			}
			new_img -> pixels[row][col].R = new->R;
			new_img -> pixels[row][col].G = new->G;
			new_img -> pixels[row][col].B = new->B;
		}
	}
	printf("La imagen es de width: %d height: %d\n", img->width, img->height);
	printf("la cantidad de blancos es : %d\n", blancos);
	/* Guardamos la imagen como un archivo png */
	char *output_file = argv[2];
	img_png_write_to_file (new_img, output_file);




	/* Liberamos los recursos asociados de la imagen */
	img_png_destroy(img);
	img_png_destroy(new_img);
	free(new);

	return 0;
}
