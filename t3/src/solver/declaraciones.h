#include "../puzzle/cell_type.h"

void sumar_voto(int distrito, CellType **cells, int *kodos, int *jack, int r, int  c);
void restar_voto(int distrito, CellType **cells, int *kodos, int *jack, int r, int c);
void elegir_celda(int** values, int height, int width, celda* cel);
int es_solucion(int* kodos, int* jack, int contador, int** values, CellType** cells, int rows, int cols, int per_disct, int n_dist);
int gana_jack(int**values, int* kodos, int* jack, CellType** cells, int *per_act_dist, int n_dist, int pers_por, int r, int c, int distrito);
int is_valido(int* celdas, int* kodos, int* jack, int** values, CellType **cells, int n_dist, int distrito, int row, int col, int* per_act_dist, int pers_por, int height, int width);
int cierra(int row, int col, int **values, int distrito, int *pers_act, int pers_por, int height);
int elegir_num(int *revisados, int n_distritos);
int rev(int *revisados, int n_distritos);