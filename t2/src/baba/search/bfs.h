#pragma once
#include "../board/board.h"
#include "table.h"
/** Nodo de cola para bfs */
typedef struct node
{
  /** Nodo seguiente en la cola */
  struct node* next;
  /** Tablero guardado en el nodo */
  Board* board;
} Node;

/** Cola para BFS */
typedef struct queue
{
  /** Elemento que fue insertado hace mas tiempo */
  Node* first;
  /** Elemento que fue insertado al ultimo */
  Node* last;
} Queue;

/** Hace BFS y retorna el tablero final */
Board* BFS(Board* board, Table* table);
