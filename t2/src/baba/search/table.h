#pragma once
#include <stdint.h>
#include "../board/board.h"

typedef struct data
{
  /** Valor del hash del board almacenado */
  uint64_t hash;
  /** Tablero mismo */
  Board* board;
} Data;


typedef struct table
{
  /** Arreglo de datas */
  Data** array;
  /** Tamanio del arreglo */
  int size;
  /** Numero de elementos en el arreglo */
  int count;

  float f_carga;

  int height;

  int width;
  /** tablero para hacer el hash*/
  uint64_t*** board;

  int *contador;

} Table;


Table* table_init(uint8_t height, uint8_t width, int size, float f_carga);

bool table_insert(Table* table, Board* board);

void table_destroy(Table* table);

uint64_t zobrish_hash(Table* table, Board *board);

Data** rehash(Table *table);

uint64_t num_insercion(Table *table, Board *board, int inicio);