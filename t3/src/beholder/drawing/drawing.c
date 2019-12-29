#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cairo-pdf.h>
#include "drawing.h"

#define WINDOW_MAX_SIZE 640.0

/** Los colores para cada tipo de celda */
Color colors[4];
Color dark_colors[4];
Color light_colors[4];
Color unsat_colors[4];

void cairo_dip(cairo_t* cr, Color color)
{
  cairo_set_source_rgb(cr, color.R, color.G, color.B);
}

/** Indica que tipo de votante tiene esta celda */
void drawing_cell_type(Content* cont, int row, int col, CellType type)
{
  cont -> voter_matrix[row][col] = type;
}
/** Indica el número del distrito al que pertenece esta celda */
void drawing_cell_district(Content* cont, int row, int col, int id)
{
  cont -> district_matrix[row][col] = id;
}
/** Indica que partido está ganando en el distrito dado */
void drawing_district_color(Content* cont, int district_id, CellType winner)
{
  cont -> district_color[district_id] = winner;
}

void cairo_circle(cairo_t* cr, int cx, int cy, double radius)
{
  cairo_arc (cr, cx, cy, radius, 0, 2*M_PI);
}

/** Dibuja el problema en la ventana */
bool drawing_draw(cairo_t* restrict cr, Content* restrict cont)
{
  /* Para prevenir cambios mientras dibujamos */
  pthread_mutex_lock(&drawing_mutex);

  cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND);

  for(int row = 0; row < cont -> matrix_height; row++)
  {
    for(int col = 0; col < cont -> matrix_width; col++)
    {
      cairo_dip(cr, unsat_colors[cont -> district_color[cont -> district_matrix[row][col]]]);
      cairo_rectangle(cr, col * cont -> cell_size, row * cont -> cell_size, cont -> cell_size, cont -> cell_size);
      cairo_fill(cr);
    }
  }

  /* Dibujamos lineas claras y delgadas */
  cairo_set_line_width(cr, cont -> cell_size / 32);
  cairo_set_source_rgb(cr, 0.9, 0.9, 1);

  /* Lineas verticales */
  for(int row = 0; row < cont -> matrix_height; row++)
  {
    for(int col = 1; col < cont -> matrix_width; col++)
    {
      /* Solo se dibujan si las celdas son de distinto distrito o ambas no pertenecen a un distrito */
      if(cont -> district_matrix[row][col - 1] != cont -> district_matrix[row][col] || !cont -> district_matrix[row][col])
      {
        cairo_move_to(cr, col * cont -> cell_size, row * cont -> cell_size);
        cairo_rel_line_to(cr, 0, cont -> cell_size);
        cairo_stroke(cr);
      }
    }
  }

  /* Lineas horizontales */
  for(int col = 0; col < cont -> matrix_width; col++)
  {
    for(int row = 1; row < cont -> matrix_height; row++)
    {
      /* Solo se dibujan si las celdas son de distinto distrito o ambas no pertenecen a un distrito */
      if(cont -> district_matrix[row - 1][col] != cont -> district_matrix[row][col] || !cont -> district_matrix[row][col])
      {
        cairo_move_to(cr, col * cont -> cell_size, row * cont -> cell_size);
        cairo_rel_line_to(cr, cont -> cell_size, 0);
        cairo_stroke(cr);
      }
    }
  }

  /* Numbers */

  // cairo_text_extents_t te;
  // cairo_select_font_face (cr, "monospace",
  //     CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  // cairo_set_font_size (cr, cont -> cell_size / 2);
  // char text[2];

  for(int row = 0; row < cont -> matrix_height; row++)
  {
    for(int col = 0; col < cont -> matrix_width; col++)
    {
      if(cont -> voter_matrix[row][col] == EMPTY) continue;

      double cx = (col + 0.5) * cont -> cell_size;
      double cy = (row + 0.5) * cont -> cell_size;

      double radius = 0.25 * cont -> cell_size;

      cairo_dip(cr, light_colors[cont -> voter_matrix[row][col]]);
      cairo_circle(cr, cx, cy, radius);
      cairo_fill_preserve(cr);
      cairo_set_line_width(cr, 2);
      cairo_dip(cr, dark_colors[cont -> voter_matrix[row][col]]);
      cairo_stroke(cr);

      // sprintf(text,"%u",cont -> voter_matrix[row][col]);
      //
      // cairo_text_extents (cr, text, &te);
      //
      // double x = cont -> cell_size / 2 + cont -> cell_size * col - te.width / 2 - te.x_bearing;
      // double y = cont -> cell_size / 2 + cont -> cell_size * row - te.height / 2 - te.y_bearing;
      // cairo_move_to (cr, x, y);
      // cairo_text_path(cr, text);
      // cairo_set_source_rgb(cr, 0.9, 0.9, 1);
      // cairo_fill_preserve(cr);
      // cairo_set_line_width(cr, cont -> cell_size / 256);
      // cairo_set_source_rgb(cr, 0, 0, 0);
      // cairo_stroke(cr);
    }
  }

  pthread_mutex_unlock(&drawing_mutex);

	return true;
}

/** Genera los contenedores para las dos imagenes superpuestas */
Content* drawing_init(int height, int width, int max_districts)
{
  Content* cont = malloc(sizeof(Content));

  pthread_mutex_init(&drawing_mutex, NULL);

  /* Dimensiones de la matriz */
  cont -> matrix_height = height;
  cont -> matrix_width = width;

  /* Los colores usados para representar los distintos tipos de celda */
  colors[NONE] = color_init(0.9*255,0.9*255,0.9*255);
  dark_colors[NONE] = color_scale_value(colors[NONE], 0.6);
  light_colors[NONE] = color_scale_saturation(colors[NONE], 0.9);
  unsat_colors[NONE] = color_scale_saturation(colors[NONE], 0.6);

  colors[EMPTY] = color_init(27,27,67);
  dark_colors[EMPTY] = colors[EMPTY];
  light_colors[EMPTY] = colors[EMPTY];
  unsat_colors[EMPTY] = colors[EMPTY];

  colors[PARTY_A] = color_init(0,150,150);
  // colors[PARTY_A] = color_init(249,202,30);
  dark_colors[PARTY_A] = color_scale_value(colors[PARTY_A], 0.6);
  light_colors[PARTY_A] = color_scale_saturation(colors[PARTY_A], 0.9);
  unsat_colors[PARTY_A] = color_scale_saturation(colors[PARTY_A], 0.4);

  colors[PARTY_B] = color_init(153,50,204);
  // colors[PARTY_B] = color_init(70,79,171);
  dark_colors[PARTY_B] = color_scale_value(colors[PARTY_B], 0.6);
  light_colors[PARTY_B] = color_scale_saturation(colors[PARTY_B], 0.9);
  unsat_colors[PARTY_B] = color_scale_saturation(colors[PARTY_B], 0.4);

  /* Las dimensiones de la ventana deben ajustarse a la matriz */
	cont -> cell_size = WINDOW_MAX_SIZE / fmax(height, width);

  /* Dimensiones de la ventana */
	cont -> image_height = cont -> cell_size * height;
	cont -> image_width  = cont -> cell_size * width;

  /* Inicializamos las matrices */
  cont -> voter_matrix = calloc(height, sizeof(CellType*));
  cont -> district_matrix = calloc(height, sizeof(uint8_t*));
  for(int row = 0; row < height; row++)
  {
    cont -> voter_matrix[row] = calloc(width, sizeof(CellType));
    cont -> district_matrix[row] = calloc(width, sizeof(uint8_t));
  }

  // Distrito 0 = no asignado
  cont -> district_color = calloc(max_districts + 1, sizeof(CellType));

  return cont;
}

/** Geenera una imagen en pdf para un estado en particular */
void drawing_snapshot(Content* cont, char* filename)
{
	double width = cont -> matrix_width * 64;
	double height = cont -> matrix_height * 64;

	/* Imprimimos las imagenes del tablero */
	cairo_surface_t* surface;
	cairo_t *cr;

  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);

	/* Reseteamos los parámetros para generar correctamente la imagen */
	Content aux = *cont;

  aux.cell_size = 64;
  aux.image_height = height;
  aux.image_width = width;

  cr = cairo_create(surface);

	/* Dibuja el estado actual */
	drawing_draw(cr, &aux);

  cairo_surface_write_to_png(surface, filename);

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}

/** Libera los recursos asociados a las herramientas de dibujo */
void drawing_free(Content* cont)
{
  for(int row = 0; row < cont -> matrix_height; row++)
  {
    free(cont -> voter_matrix[row]);
    free(cont -> district_matrix[row]);
  }
  free(cont -> voter_matrix);
  free(cont -> district_matrix);

  free(cont -> district_color);

  free(cont);
}
