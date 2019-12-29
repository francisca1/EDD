#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "animation.h"
#include "../drawing/drawing.h"
#include <math.h>
#include <locale.h>

/** El thread encargado de actualizar el contenido de la ventana */
static pthread_t* update_thread;

/** Lleva a cabo la actualización del tablero */
static void* update(void* ptr)
{
	setlocale(LC_ALL, "C");

	/* Desencapsula los parámetros */
	void** param = ptr;
	GtkWidget* canvas = param[0];
	Content* cont = param[1];
	free(param);

	char command[64];

  while(update_thread)
  {
    /* Si alguna de las lecturas falla, dejamos de llamar */
    if(fscanf(stdin, "%s", command))
    {
			/* Cerrar ventana */
      if(!strcmp(command, "X"))
      {
				gtk_main_quit();
				break;
      }
			/* Tipo de la celda */
			else if(!strcmp(command, "T"))
			{
				int row, col;
				CellType type;
				fscanf(stdin, "%d %d %u", &row, &col, &type) ? : abort();
				pthread_mutex_lock(&drawing_mutex);
				drawing_cell_type(cont, row, col, type);
				pthread_mutex_unlock(&drawing_mutex);
				gtk_widget_queue_draw(canvas);
			}
			/* Distrito de la celda */
			else if(!strcmp(command, "D"))
			{
				int row, col;
				int district_id;
				fscanf(stdin, "%d %d %d", &row, &col, &district_id) ? : abort();
				pthread_mutex_lock(&drawing_mutex);
				drawing_cell_district(cont, row, col, district_id);
				pthread_mutex_unlock(&drawing_mutex);
				gtk_widget_queue_draw(canvas);
			}
			/* El ganador de un distrito */
			else if(!strcmp(command, "W"))
			{
				int district_id;
				CellType winner;
				fscanf(stdin, "%d %u", &district_id, &winner) ? : abort();
				pthread_mutex_lock(&drawing_mutex);
				drawing_district_color(cont, district_id, winner);
				pthread_mutex_unlock(&drawing_mutex);
				gtk_widget_queue_draw(canvas);
			}
			/* Guardar imagen */
			else if(!strcmp(command, "S"))
			{
				char filename[64];
				fscanf(stdin, "%s", filename) ? : abort();
				drawing_snapshot(cont, filename);
			}
			/* Comando desconocido */
			else
			{
				break;
			}
    }
    else
    {
      break;
    }

  }
  pthread_exit(NULL);
}

/** Inicializa el thread que animará el programa */
void animation_init(GtkWidget* canvas, gpointer user_data)
{
	/* Encapsula ambos parámetros dentro de un mismo puntero */
	void** param = calloc(2, sizeof(void*));
	param[0] = canvas;
	param[1] = user_data;

  /* Inicializamos el thread */
  update_thread = malloc(sizeof(pthread_t));
  /* Y lo lanzamos */
  pthread_create(update_thread, NULL, update, param);
}

/** Aborta la rutina que actualiza el tablero */
void animation_abort()
{
	pthread_cancel(*update_thread);
	free(update_thread);
	update_thread = NULL;
}
