#include <stdlib.h>
#include <stdio.h>
#include "bfs.h"

/** Crea una cola vacia */
static Queue* queue_init()
{
  // Creo la cola
  Queue* queue = malloc(sizeof(Queue));

  // Dejo los valores en NULL
  queue -> first = NULL;
  queue -> last = NULL;

  // Retorno la cola
  return queue;
}

/** Inserta un elemento al principio de la cola */
static void queue_insert(Queue* queue, Board* board)
{
  // Creo el nodo de la cola a insertar
  Node* node = malloc(sizeof(Node));
  node -> board = board;
  node -> next = NULL;

  // Si la cola esta vacia, dejo el nodo como primero y ultimo
  if (!queue -> first)
  {
    queue -> first = node;
    queue -> last = node;
  }
  // Sino, lo dejo como last y conecto el anterior last con el nodo
  else
  {
    queue -> last -> next = node;
    queue -> last = node;
  }
}

/** Elimina el primer nodo de la cola y lo retorna */
static Board* queue_pop(Queue* queue)
{
  // Nodo a eliminar
  Node* poped = queue -> first;

  // Si no quedan elementos retorno NULL
  if (!poped) return NULL;

  // Sino, dejo el siguiente como first
  queue -> first = poped -> next;

  // Si no quedan nodos entonces last es NULL
  if (!queue -> first) queue -> last = NULL;

  // Obtengo el board y libero el nodo
  Board* board = poped -> board;
  free(poped);

  // Retorno el board
  return board;
}

/** Libera la cola con todos sus nodos */
static void queue_destroy(Queue* queue)
{
  // Libera iteretivamente los nodos
  Node* current = queue -> first;
  while (current)
  {
    Node* next = current -> next;
    free(current);
    current = next;
  }

  // Libera la cola
  free(queue);
}

/** Hace BFS y retorna el tablero final */
Board* BFS(Board* board, Table* table)
{
  // Profundidad actual
  uint16_t depth = 0;

  // Creo la cola con el board original
  Queue* queue = queue_init();
  queue_insert(queue, board);

  // Inserto el board original en la tabla
  table_insert(table, board);

  // Mientras queden elementos en la cola
  while (queue -> first)
  {
    // Saco el primer elemento de la cola
    Board* current = queue_pop(queue);

    // Si esta a una nueva profundiad, imprimo
    if (current -> depth == depth)
    {
      printf("Profundidad %hu\n", depth);
      depth++;
    }

    // Itero por los movimientos
    for (Direction dir = UP; dir <= LEFT; dir++)
    {
      // Obtengo el tablero dado el movimiento
      Board* new = board_move(current, dir);

      // Si el tablero es WIN, termino
      if (new -> is_win)
      {
        // Libero la cola
        queue_destroy(queue);

        // Retorno el tablero
        return new;
      }

      // Inserto en la tabla
      if (table_insert(table, new))
      {
        // Inserto en la cola
        queue_insert(queue, new);
      }
      // Si ya estaba en la tabla, lo libero
      else
      {
        board_destroy(new);
      }
    }
  }

  // Si llegue hasta aca es porque no hay solucion
  queue_destroy(queue);
  return NULL;
}
