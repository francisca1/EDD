#pragma once

/****************************************************************************/
/*                                Watcher                                   */
/*                                                                          */
/* Modulo a cargo de la visualizacion del problema.                         */
/****************************************************************************/

#include <stdbool.h>
#include "../puzzle/cell_type.h"

/** Abre una ventana para mostrar una matriz de las dimensiones dadas */
void watcher_open(int height, int width, int max_districts);
/** Indica que tipo de votante tiene esta celda */
void watcher_set_cell_type(int row, int col, CellType cellType);
/** Indica el número del distrito al que pertenece esta celda
 *  distrito 0 indica que no tiene un valor asignado
 *  distritos 1..n indica que tiene un valor asignado
 */
void watcher_set_cell_district(int row, int col, int district_id);
/** Indica que partido está ganando en el distrito dado, o ninguno */
void watcher_set_district_color(int district_id, CellType winner);
/** Imprime el contenido de la ventana a una imagen .PNG */
void watcher_snapshot(char *filename);
/** Cierra y libera los recursos de la ventana */
void watcher_close();
