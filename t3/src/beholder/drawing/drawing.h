#pragma once

#include <cairo.h>
#include <stdbool.h>
#include "color.h"
#include <pthread.h>
#include "../../watcher/watcher.h"


pthread_mutex_t drawing_mutex;

/** Contiene la información de lo que ha de ser dibujado en el canvas */
struct content
{
	/** Ancho de la ventana */
	int image_width;
	/** Alto de la ventana */
	int image_height;
	/** Matriz con el tipo de votante de cada celda */
	CellType** voter_matrix;
	/** Ancho de la matriz */
	int matrix_width;
	/** Alto de la matriz */
	int matrix_height;
	/** Indica el tamaño de una celda */
	int cell_size;
	/** Indica el distrito al que pertenece cada celda */
	uint8_t** district_matrix;
	/** Indica el tipo predominante de cada distrito */
	CellType* district_color;
};
/** Contiene la información de lo que ha de ser dibujado en el canvas */
typedef struct content Content;

/** Inicializa las herramientas de dibujo */
Content* drawing_init           (int height, int width, int max_districts);
/** Dibuja sobre el canvas dado */
bool     drawing_draw           (cairo_t* cr, Content* cont);
/** Indica que tipo de votante tiene esta celda */
void     drawing_cell_type      (Content* cont, int row, int col, CellType type);
/** Indica el número del distrito al que pertenece esta celda */
void     drawing_cell_district  (Content* cont, int row, int col, int id);
/** Indica que partido está ganando en el distrito dado */
void     drawing_district_color (Content* cont, int district_id, CellType winner);
/** Genera una imagen en PNG para un estado en particular */
void     drawing_snapshot       (Content* cont, char* filename);
/** Libera los recursos asociados a las herramientas de dibujo */
void     drawing_free           (Content* cont);
