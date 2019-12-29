#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include <math.h>
#include "../random/random.h"
#define SONDEO  2
#define METODO 1
#define AUREO (1+sqrt(5))/2

// div-cuadratico = 3, mult lineal = 1 , mult cuad = 2
/** Calcula una mala funcion de hash del tablero */
static uint64_t bad_hash(Board* board)
{
  /*
  Esta funcion numera las posiciones del tablero desde la 1 hasta la
  weight x height y multiplica el valor de los elementos que hay en la posicion
  por el indice de la posicion y suma todo.
  Es una mala funcion de hash porque los valores van a tender a caer en un
  rango pequenio
  */

  // Parto con el hash en 0
  uint64_t hash = 0;

  // Indice de la posicion
  int i = 1;
  for (int row = 0; row < board -> height; row++)
  {
    for (int col = 0; col < board -> width; col++)
    {
      // Lista de la celda
      List* list = board -> matrix[row][col];

      // Itero por los objetos de la posicion multiplicando i por el valor
      for (Cell* current = list -> first; current; current = current -> next)
      {
        // Multiplico el valor del objeto con el indice y lo sumo al hash
        hash += current -> type * i;
      }

      // Aumento el indice
      i++;
    }
  }

  // Retorno el hash
  return hash;
}

Table* table_init(uint8_t height, uint8_t width, int size, float f_carga){
    //inicializo tabla
    Table* table = malloc(sizeof(Table));

    // Inicializo el arreglo
    table->size = size;
    table->count = 0;
    table->array = calloc(table->size, sizeof(Data *));
    table->f_carga = f_carga;
    table->contador = calloc(table->size, sizeof(int));

    for (int i = 0; i < table->size; i++)
    {
      table->contador[i] = 1;
    }

    //inicializo tablero de hash
    table->width = width;
    table->height = height;
    table->board = calloc(table->height, sizeof(uint64_t  **));

    for (int row = 0; row < table->height; row++)
    {
        // pido para las filas
        table->board[row] = calloc(table->width, sizeof(uint64_t  *));

        // pido para los diferentes numeros random en cada casilla
        for (int col = 0; col < table->width; col++)
        {
            table->board[row][col] = calloc(19, sizeof(uint64_t ));
            // inicializo con un numero random
            for (int i = 0; i < 19; i++)
            {
                table->board[row][col][i] = get_random();
            }
        }
    }

    return table;
}



void table_destroy(Table *table){
  for (int row = 0; row < table->height; row++)
  {
    for (int col = 0; col < table->width; col++)
    {
      /* libero la lista de numeros aleatorios */
      free(table->board[row][col]);
    }
    // libero la fila
    free(table->board[row]);
  }

  for (int i = 0; i < table->size; i++)
  {
    if (table->array[i])
    {
      free(table->array[i]);
    }
  }
  free(table->array);
  // libero la matriz
  free(table->board);
  free(table->contador);
  //libero el la tabla
  free(table);
}

uint64_t zobrish_hash(Table *table, Board *board){
    uint64_t hash = 0;

    for (int row = 0; row < board->height; row++)
    {
        for (int col = 0; col < board->width; col++)
        {
            // si hay elementos
            if (board->matrix[row][col]->first)
            {
                for  (Cell *current = board->matrix[row][col]->first; current; current = current->next)
                {
                    hash = hash ^ table->board[row][col][current->type];
                }
            }
            
        }
        
    }
    return hash;
}

bool table_insert(Table* table, Board* board)
{
  // Calculo el hash del tablero
  uint64_t hash =bad_hash(board);
  // Busco el tablero en la tabla

  uint64_t buscar;

  if (METODO ==1)
  {
    /* metodo de la division */
    buscar = hash % table->size;
  }
  else
  {
   
    buscar = floor(table->size * fmod((double)AUREO *(uint32_t) hash, 1));

    }

  uint64_t insercion = buscar;
  int iteracion = 1;
  while (insercion < table->size)
  {
    // si no tiene nada, salgo
    table->contador[insercion]++;

    if (table->array[insercion] == NULL)
    {
      break;
    }

    // Veo el tablero y hash que hay en la posicion
    uint64_t h2 = table->array[insercion]->hash;
    Board *b2 = table->array[insercion]->board;
    // Si son el mismo tablero
    if (board_compare(board, hash, b2, h2))
    {
      // Retorno false porque ya existia
      return false;
    }

    //sondeo lineal
    if (SONDEO == 1)
    {
      insercion++;
    } else{ //sondeo cuadrático
      insercion = hash + (uint64_t)pow(iteracion, 2) * 3 + (uint64_t)pow(iteracion - 1, 2) * 7;
    }
    if (insercion>= table->size)
    {
      insercion = insercion % table->size;
    }
    iteracion++;
  }

  // Si llego hasta aca es porque no existia el tablero por lo que lo inserto

  table->array[insercion] = malloc(sizeof(Data));
  table -> array[insercion] -> board = board;
  table -> array[insercion] -> hash = hash;

  table -> count++;
  
  if ((float)(table->count /(float) table->size) >= table->f_carga)
  {
    
    printf("rehasheando....size %d\n", table->size);
    Data ** pointer = rehash(table);
    free(table->array);
    table->array = pointer;
    table->size = table->size * 2;
  }

  return true;
}

Data** rehash(Table* table)
{
  
  Data** new_array = calloc(table->size*2, sizeof(Data *));
  
  int *new_contador = calloc(2*table->size, sizeof(int));

  for (int i = 0; i < (2*table->size); i++)
    {
      new_contador[i] = 1;
    }
  //recorro la tabla antigua
  for (int i = 0; i < table->size; i++)
  {
    if (table->array[i]) 
    {
      //si hay un elemento calculo su posicion y la inserto en la nueva tabla
      uint64_t buscar;

      if (METODO == 1)
      {
        /* metodo de la division */
        buscar = table->array[i]->hash % (2 * table->size);
        } else
        {
          buscar = floor(2*table->size * fmod(AUREO*(uint32_t)table->array[i]->hash,1));
        }

      uint64_t insercion = buscar;

      int iteracion = 1;
      while (insercion < (2*table->size))
      {
        
        new_contador[insercion]++;
        if (new_array[insercion] == NULL)
        {
          break;
        }

        //sondeo lineal
        if (SONDEO == 1)
        {
          insercion++;
        } else{ //sondeo cuadrático
          insercion = table->array[i]->hash + (uint64_t)pow(iteracion, 2)*3 + (uint64_t)pow(iteracion - 1, 2)*7;
        }
        if (insercion>= (2*table->size))
        {
          insercion = insercion % (2*table->size);
        }
        iteracion++;
      }
      if (insercion>= (2*table->size))
      {
        insercion = insercion % (2*table->size);
      }

      new_array[insercion] = malloc(sizeof(Data));
      new_array[insercion] -> board = table->array[i]->board;
      new_array[insercion] -> hash = table->array[i]->hash;
      free(table->array[i]);
    }
    }
    free(table->contador);
    table->contador = new_contador;
    return new_array;
}