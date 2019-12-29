#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "drawing.h"
#include <stdbool.h>

double TILESIZE = 48.0;


static Color color_background = (Color){.R = 0.059, .G = 0.082, .B = 0.161};

/** El mutex de dibujo */
static pthread_mutex_t drawing_mutex;

/** Multiplica un color por un escalar, para aclararlo o oscurecerlo */
// static Color color_scale(Color color, double k)
// {
//   return (Color){.R = color.R * k,.G = color.G * k, .B = color.B * k};
// }

/** Setea el color RGB de cairo */
static void cairo_dip(cairo_t* cr, Color color)
{
  cairo_set_source_rgb(cr,color.R,color.G, color.B);
}

// static void cairo_dip_alpha(cairo_t* cr, Color color, double alpha)
// {
//   cairo_set_source_rgba(cr,color.R,color.G, color.B, alpha);
// }
//
// static void cairo_circle(cairo_t* cr, double xc, double yc, double rad)
// {
//   cairo_arc(cr, xc, yc, rad, 0, 2*M_PI);
// }
//
// static void cairo_line(cairo_t* cr, double x1, double y1, double x2, double y2)
// {
//   cairo_move_to(cr, x1, y1);
//   cairo_line_to(cr, x2, y2);
// }

static void reset_layer(cairo_surface_t* layer)
{
  cairo_t* cr = cairo_create(layer);

  cairo_dip(cr, color_background);

  cairo_paint(cr);

  cairo_destroy(cr);
}

void drawing_clear(Content* cont)
{
  reset_layer(cont -> buffer);
}

void drawing_draw_element (Content* cont, int row, int col, Type type)
{
  cairo_t* cr = cairo_create(cont -> buffer);

  cairo_set_source_surface(cr, cont -> sprites[type], col * TILESIZE, row * TILESIZE);

  cairo_paint(cr);

  cairo_destroy(cr);
}

bool drawing_draw(cairo_t* restrict cr, Content* restrict cont)
{
  pthread_mutex_lock(&drawing_mutex);

  cairo_set_source_surface(cr, cont -> buffer, 0, 0);

  cairo_paint(cr);

  pthread_mutex_unlock(&drawing_mutex);

	return true;
}

static void drawing_init_sprites(Content* cont)
{
  cairo_surface_t* allsprites = cairo_image_surface_create_from_png("assets/bigsprites.png");

  cont -> sprites[0] = NULL;

  cairo_format_t format = CAIRO_FORMAT_ARGB32;

  for(int i = 1; i < 19; i++)
  {
    cont -> sprites[i] = cairo_image_surface_create(format, TILESIZE, TILESIZE);

    cairo_t* cr = cairo_create(cont -> sprites[i]);

    cairo_set_source_surface(cr, allsprites, -(i-1) * TILESIZE, 0);

    cairo_paint(cr);

    cairo_destroy(cr);
  }

  cairo_surface_destroy(allsprites);
}

/** Genera el contenedor de la ventana */
Content* drawing_init(int height, int width)
{
  Content* cont = malloc(sizeof(Content));

  // if(height > width)
  // {
  //   TILESIZE = fminf(TILESIZE, MAXSIZE / ((double)height + 2));
  // }
  // else
  // {
  //   TILESIZE = fminf(TILESIZE, MAXSIZE / ((double)width + 2));
  // }
  //
  // TILESIZE = round(TILESIZE);
  //
  // cont -> height = (height + 2) * TILESIZE;
  // cont -> width = (width + 2) * TILESIZE;

  cont -> height = height * TILESIZE;
  cont -> width = width * TILESIZE;

  /* Inicializa las imagenes vectoriales */

  // El formato de imagen: R G B de 8 bits cada uno + Alpha
  cairo_format_t format = CAIRO_FORMAT_ARGB32;
  cont -> buffer = cairo_image_surface_create(format, cont -> width, cont -> height);

  reset_layer(cont -> buffer);

  drawing_init_sprites(cont);


	pthread_mutex_init(&drawing_mutex, NULL);

  return cont;
}



/** Geenera una imagen en pdf para un estado en particular */
void drawing_snapshot(Content* cont, char* filename)
{
	double width = cont -> width;
	double height = cont -> height;

	/* Imprimimos las imagenes del tablero */
	cairo_surface_t* surface;
	cairo_t *cr;

  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB30, width, height);

	// surface = cairo_pdf_surface_create (filename, width, height);
	cr = cairo_create(surface);

	/* Reseteamos los parámetros para generar correctamente la imagen */
	Content aux = *cont;

	/* Dibuja el estado actual */
	drawing_draw(cr, &aux);

  cairo_surface_write_to_png(surface, filename);

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}

/** Libera los recursos asociados a las herramientas de dibujo */
void drawing_free(Content* cont)
{
  cairo_surface_destroy(cont -> buffer);
  for(int i = 1; i <= 18; i++)
  {
    cairo_surface_destroy(cont -> sprites[i]);
  }
  free(cont);
  pthread_mutex_destroy(&drawing_mutex);
}
