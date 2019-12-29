#ifndef T3_LIB_CELLTYPE
#define T3_LIB_CELLTYPE

/* Specify the different types of cells
 */
typedef enum
{
  EMPTY, // La celda no tiene votantes (0)
  NONE, // La celda no apoya ningun partido (1)
  PARTY_A, // La celda apoya al partido A (2)
  PARTY_B // La celda apoya al partido B (3)
} CellType;

typedef struct celda {
   int row;
   int col;
}celda;

#endif /* end of include guard: T3_LIB_CELLTYPE */


