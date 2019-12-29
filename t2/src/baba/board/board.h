#pragma once
#include <stdint.h>
#include "object.h"

typedef enum
{
  UP,
  RIGHT,
  DOWN,
  LEFT
} Direction;

/** Nodo de ligada de objetos en una celda */
typedef struct cell
{
  /** Siguiente celda */
  struct cell* next;
  /** Objeto en la celda */
  Type type;
} Cell;

/** Lista simplemente ligada de cells */
typedef struct list
{
  /** Puntero  */
  Cell* first;
} List;

/** Tablero con celdas */
typedef struct board
{
  /** Altura del tablero */
  uint8_t height;
  /** Ancho de tablero */
  uint8_t width;
  /** Matriz de celdas */
  List*** matrix;
  /** Indica si el tablero esta resuelto o no */
  bool is_win;
  /** Tablero del cual viene el tablero */
  struct board* parent;
  /** Profundidad del tablero */
  uint16_t depth;
} Board;

/** Inicializa el tablero a partir del archivo de reglas */
Board* board_init(char* filename);

/** Copia el tablero */
Board* board_copy(Board* board);

/** Hace un movimiento y retorna el tablero nuevo (hace una copia) */
Board* board_move(Board* board, Direction dir);

/** Imprime el tablero */
void board_print(Board* board);

/** Compara 2 tableros primero comparando su funcion de hash */
bool board_compare(Board* b1, uint64_t h1, Board* b2, uint64_t h2);

/** Libera el tablero */
void board_destroy(Board* board);
