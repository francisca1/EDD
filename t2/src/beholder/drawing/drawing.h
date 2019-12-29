#pragma once

#include <cairo.h>
#include <stdbool.h>
#include "color.h"
#include <pthread.h>

/** Todos los posibles tipos de objetos */
typedef enum
{
  EMPTY = 0,
  BABA = 1,
  KEKE = 2,
  MEME = 3,
  WALL = 4,
  KEY = 5,
  DOOR = 6,
  ICE = 7,
  LAVA = 8,
  FLAG = 9,
  STAR = 10,
  JELLY = 11,
  CRAB = 12,
  ROCK = 13,
  SKULL = 14,
  ALGAE = 15,
  BRICK = 16,
  PILLAR = 17,
  HEDGE = 18
} Type;

/** Contiene la información de lo que ha de ser dibujado en el canvas */
struct content
{
	/** Ancho de la ventana */
	int width;
	/** Alto de la ventana */
	int height;
	/** Imagen vectorial donde vamos dibujando las cosas */
	cairo_surface_t* buffer;
	/** Arreglo con cada uno de los sprites */
	cairo_surface_t* sprites[19];
};
/** Contiene la información de lo que ha de ser dibujado en el canvas */
typedef struct content Content;

/** Inicializa las herramientas de dibujo */
Content* drawing_init         (int height, int width);
/** Dibuja el contenido sobre el canvas dado */
bool     drawing_draw         (cairo_t* cr, Content* cont);
/** Dibuja un elemento en la ventana */
void     drawing_draw_element (Content* cont, int row, int col, Type type);
/** Limpia la ventana */
void     drawing_clear        (Content* cont);
/** Genera una imagen en PNG para un estado en particular */
void     drawing_snapshot     (Content* cont, char* filename);
/** Libera los recursos asociados a las herramientas de dibujo */
void     drawing_free         (Content* cont);
