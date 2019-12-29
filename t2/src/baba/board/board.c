#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "board.h"

// FUNCIONES PRIVADAS

/** Inicializa una celda con un tipo dado */
static Cell* cell_init(Type type)
{
  // Creo la celda
  Cell* cell = malloc(sizeof(Cell));

  // Inicializo sus variables
  cell -> next = NULL;
  cell -> type = type;

  // Retorno la celda
  return cell;
}

/** Copia una celda recursivamente y la retorna */
static Cell* cell_copy(Cell* cell)
{
  // Si es NULL, retorno NULL
  if (!cell) return NULL;

  // Sino, copio la celda
  Cell* copy = malloc(sizeof(Cell));
  copy -> type = cell -> type;
  copy -> next = cell_copy(cell -> next);

  // Retorno la copia
  return copy;
}

/** Libera recursivamente cada celda */
static void cell_destroy(Cell* cell)
{
  if (cell)
  {
    cell_destroy(cell -> next);
    free(cell);
  }
}

/** Retorna True si existe algo con propiedad prop en las celdad de cell */
static bool exists(Cell* cells, Property prop)
{
  Cell* current = cells;
  while (current)
  {
    if (is_rule(current -> type, prop))
    {
      return true;
    }
    current = current -> next;
  }
  return false;
}

/** Extrae todas las celdas que tienen la propiedad dada */
static Cell* extract(List* list, Property prop)
{
  // Lista inicial de elementos extraidos
  Cell* extracted = NULL;

  // Itero por las celdas de board
  Cell* prev = NULL;
  Cell* current = list -> first;
  while (current)
  {
    Cell* next = current -> next;

    // Si tiene la propiedad que busco
    if (is_rule(current -> type, prop))
    {
      // Agrego la celda a extracted
      current -> next = extracted;
      extracted = current;

      // Reconecto la lista original
      if (prev)
      {
        prev -> next = next;
      }
      else
      {
        list -> first = next;
      }
    }
    else
    {
      // Sino, me paro en el siguiente
      prev = current;
    }

    // Me muevo al siguiente
    current = next;
  }

  // Retorno la lista extracted
  return extracted;
}

/** Inserta las celdas cells al final de la lista */
static void insert(List* list, Cell* cells)
{
  if (!list -> first)
  {
    list -> first = cells;
  }
  else
  {
    Cell* current = list -> first;
    while (current -> next) current = current -> next;
    current -> next = cells;
  }
}

/** Mueve los types en moving hacia target en direccion dir */
static Cell* move_if_possible(Board* board, Vector target, Vector dir, List* moving)
{
  /*
  Genero 3 listas:
    returns = Celdas que no se pueden mover a target desde moving
    stays = Celdas que quedan en target (que estaban ahi o que vienen de moving)
    moves = Celdas que se mueven mas lejos
  */
  /*
  Prioridades:
    1: shut -> open (open -> shut)
    2: -> stop: break
    3: -> push
    4: mover
    5: neutralizaciones:
      melt -> hot (hot -> melt)
      you -> defeat (defeat -> you)
    6: you -> win (win -> you)
    7: merge
  */

  // Celdas en la posicion original
  List* targ_list = board -> matrix[target.row][target.col];

  // 1:
  // Caso shut -> open
  if (exists(moving -> first, SHUT) && exists(targ_list -> first, OPEN))
  {
    // Elimino los shuts y los destroys
    Cell* shut = extract(moving, SHUT);
    cell_destroy(shut);
    Cell* opened = extract(targ_list, OPEN);
    cell_destroy(opened);
  }
  // Caso open -> shut
  if (exists(moving -> first, OPEN) && exists(targ_list -> first, SHUT))
  {
    // Elimino los shuts y los destroys
    Cell* opened = extract(moving, OPEN);
    cell_destroy(opened);
    Cell* shut = extract(targ_list, SHUT);
    cell_destroy(shut);
  }

  // 2:
  // Caso stop, termina
  if (exists(targ_list -> first, STOP))
  {
    return moving -> first;
  }

  // 3: recursivo
  if (exists(targ_list -> first, PUSH))
  {
    // Obtengo elementos que se empujan desde targ_list
    List pushed;
    pushed.first = extract(targ_list, PUSH);

    // Obtengo elementos que no se pueden empujar recursivamente
    Cell* cannot = move_if_possible(board, vector_sum(dir, target), dir, &pushed);

    // Si tengo cannot entonces no puedo empujar
    if (cannot)
    {
      insert(targ_list, cannot);
      return moving -> first;
    }
  }

  // 4: mover
  insert(targ_list, moving -> first);

  // 5: Neutralizaciones
  if (exists(targ_list -> first, MELT) && exists(targ_list -> first, HOT))
  {
    Cell* melted = extract(targ_list, MELT);
    cell_destroy(melted);
  }
  if (exists(targ_list -> first, YOU) && exists(targ_list -> first, DEFEAT))
  {
    Cell* defeated = extract(targ_list, YOU);
    cell_destroy(defeated);
  }

  // 6: Ganar
  if (exists(targ_list -> first, WIN) && exists(targ_list -> first, YOU))
  {
    board -> is_win = true;
  }

  // 7: Merge, elimino todos los repetidos en una misma casilla
  Cell* current = targ_list -> first;
  while (current)
  {
    // Obtengo el type del actual
    Type type = current -> type;

    // Elimino todos los de este type en el resto de ls lista
    for (Cell* act = current; act && act -> next; act = act -> next)
    {
      if (act -> next -> type == type)
      {
        Cell* eliminated = act -> next;
        act -> next = eliminated -> next;
        free(eliminated);
      }
    }

    // Paso al siguiente
    current = current -> next;
  }

  return NULL;
}

/** Hace el movimiento sobre el tablero */
static void input(Board* board, Direction dir)
{
  // Segun la direccion obtengo el movimiento en el tablero, saltos de linea, etc...
  Vector mov;
  Vector jump;
  Vector init;
  uint8_t limit_mov;
  uint8_t limit_jump;
  switch (dir)
  {
    case UP:
      init.row = 0;
      init.col = 0;
      mov.row = 1;
      mov.col = 0;
      jump.row = - board -> height;
      jump.col = 1;
      limit_mov = board -> height - 1;
      limit_jump = board -> width - 1;
      break;
    case DOWN:
      mov.row = -1;
      mov.col = 0;
      jump.row = board -> height;
      jump.col = 1;
      init.row = board -> height - 1;
      init.col = 0;
      limit_mov = board -> height - 1;
      limit_jump = board -> width - 1;
      break;
    case RIGHT:
      mov.row = 0;
      mov.col = -1;
      jump.row = 1;
      jump.col = board -> width;
      init.row = 0;
      init.col = board -> width - 1;
      limit_mov = board -> width - 1;
      limit_jump = board -> height - 1;
      break;
    case LEFT:
      mov.row = 0;
      mov.col = 1;
      jump.row = 1;
      jump.col = - board -> width;
      init.row = 0;
      init.col = 0;
      limit_mov = board -> width - 1;
      limit_jump = board -> height - 1;
      break;
  }

  // Itero por el tablero haciendo los movimientos
  Vector current;
  current.row = init.row;
  current.col = init.col;
  for (int jumps = 0; jumps <= limit_jump; jumps++)
  {
    for (int movs = 0; movs <= limit_mov; movs++)
    {
      // Reviso si hay algun type con propiedad YOU
      if (exists(board -> matrix[current.row][current.col] -> first, YOU))
      {
        // Extraigo los you
        List yous;
        yous.first = extract(board -> matrix[current.row][current.col], YOU);

        // Hago el movimiento si es posible
        Vector dir = vector_negate(mov);
        Vector target = vector_sum(current, dir);
        Cell* leftovers = move_if_possible(board, target, dir, &yous);

        // Si no se pueden mover, los vuelvo a insertar
        if (leftovers)
        {
          insert(board -> matrix[current.row][current.col], leftovers);
        }
      }

      current.row += mov.row;
      current.col += mov.col;
    }
    current.row += jump.row;
    current.col += jump.col;
  }
}

// FUNCIONES PRUBLICAS

/** Inicializa el tablero a partir del archivo de reglas */
Board* board_init(char* filename)
{
  // Creo el tablero
  Board* board = malloc(sizeof(Board));

  // Lo creo como si es el primero
  board -> depth = 0;
  board -> is_win = false;
  board -> parent = NULL;

  // Abro el archivo de tablero
  FILE* file = fopen(filename, "r");

  // Leo las dimensiones del tablero y las guardo en el board
  fscanf(file, "%hhu %hhu", &board -> height, &board -> width);

  // Creo la matriz de celdas en que cada celda en NULL
  board -> matrix = calloc(board -> height, sizeof(Cell**));
  for (int row = 0; row < board -> height; row++)
  {
    board -> matrix[row] = calloc(board -> width, sizeof(Cell*));
  }

  // Leo las filas de celdas desde el archivo
  for (int row = 0; row < board -> height; row++)
  {
    // Leo cada elemento en la fila
    for (int col = 0; col < board -> width; col++)
    {
      Type type;
      fscanf(file, "%u", &type);

      board -> matrix[row][col] = malloc(sizeof(List));

      // Si hay algo, lo inserto en el tablero
      if (type)
      {
        board -> matrix[row][col] -> first = cell_init(type);
      }
      else
      {
        board -> matrix[row][col] -> first = NULL;
      }
    }
  }

  // Cierro el archivo
  fclose(file);

  // Retorno el tablero
  return board;
}

/** Copia el tablero */
Board* board_copy(Board* board)
{
  // Creo un nuevo tablero
  Board* copy = malloc(sizeof(Board));
  copy -> is_win = board -> is_win;

  // Traspaso el alto y el ancho
  copy -> height = board -> height;
  copy -> width = board -> width;

  // Copio la matriz
  copy -> matrix = calloc(copy -> height, sizeof(Cell**));
  for (int row = 0; row < board -> height; row++)
  {
    copy -> matrix[row] = calloc(copy -> width, sizeof(Cell*));
    for (int col = 0; col < copy -> width; col++)
    {
      copy -> matrix[row][col] = malloc(sizeof(List));
      copy -> matrix[row][col] -> first = cell_copy(board -> matrix[row][col] -> first);
    }
  }

  // Asigno como parent del tablero al tablero original
  copy -> parent = board;
  copy -> depth = copy -> parent -> depth + 1;

  // Retorno la copia
  return copy;
}

/** Hace un movimiento y retorna el tablero nuevo (hace una copia) */
Board* board_move(Board* board, Direction dir)
{
  // Copio el tablero y hago el movimiento en la copia
  Board* copy = board_copy(board);

  // Hago el movimiento en la copia
  input(copy, dir);

  // Retorno el tablero con el mivimiento hecho
  return copy;
}

/** Imprime el tablero */
void board_print(Board* board)
{
  printf("Depth: %uh\n", board -> depth);
  for (int row = 0; row < board -> height; row++)
  {
    for (int col = 0; col < board -> width; col++)
    {
      printf("(");
      Cell* current = board -> matrix[row][col] -> first;
      if (!current)
      {
        printf(" 0");
      }
      while (current)
      {
        printf(" %d", current -> type);
        current = current -> next;
      }
      printf(" ) ");
    }
    printf("\n");
  }
}

/** Compara 2 tableros primero comparando su funcion de hash */
bool board_compare(Board* b1, uint64_t h1, Board* b2, uint64_t h2)
{
  // Si los hashes son distintos, retorno false
  if (h1 != h2) return false;

  // Sino, comparo todas las posiciones iterando una por una
  for (int row = 0; row < b1 -> height; row++)
  {
    for (int col = 0; col < b1 -> width; col++)
    {
      // Listas
      List* l1 = b1 -> matrix[row][col];
      List* l2 = b2 -> matrix[row][col];

      // Veo cuales tipos estan en cada lista
      uint8_t t1[19] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0, 0, 0};
      uint8_t t2[19] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0, 0, 0};

      // Itero por la primera lista sumando a los contadores
      for (Cell* current = l1 -> first; current; current = current -> next)
      {
        t1[current -> type]++;
      }

      // Itero por la segunda lista sumando a los contadores
      for (Cell* current = l2 -> first; current; current = current -> next)
      {
        t2[current -> type]++;
      }

      // Comparo ambas tablas
      for (int i = 0; i < 19; i++)
      {
        if (t1[i] != t2[i]) return false;
      }
    }
  }

  // Si llego hasta aca es porque son iguales
  return true;
}

/** Libera el tablero */
void board_destroy(Board* board)
{
  // Itero por el tablero liberando las celdas
  for (int row = 0; row < board -> height; row++)
  {
    // Libero las celdas de la fila
    for (int col = 0; col < board -> width; col++)
    {
      cell_destroy(board -> matrix[row][col] -> first);
      free(board -> matrix[row][col]);
    }

    // Libero la fila
    free(board -> matrix[row]);
  }

  // Libero la matriz
  free(board -> matrix);

  // Libero el tablero
  free(board);
}
