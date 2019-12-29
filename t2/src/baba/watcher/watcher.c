#include "watcher.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define WATCHER "./beholder"

static FILE* watcher = NULL;

/** Abre un watcher de las dimensiones especificadas */
void watcher_open(int height, int width)
{
	// return;
  char command[256];
  sprintf(command, "%s %d %d", WATCHER, height, width);

  if(watcher) watcher_close();

  watcher = popen(command, "w");
}

/** Limpia la ventana */
static void watcher_clear ()
{
	if(watcher)
	{
		if(fprintf(watcher, "C\n") < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Actualiza la ventana */
static void watcher_refresh()
{
	if(watcher)
	{
		if(fprintf(watcher, "R\n") < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}



/** Indica que una celda dada contiene un objeto del tipo dado */
static void watcher_set_cell(int row, int col, Type type)
{
	if(watcher)
	{
		if(fprintf(watcher, "D %d %d %u\n", row, col, type) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}


/** Dibuja el tablero completo */
void watcher_draw_board(Board* board)
{
  watcher_clear();
  for(int row = 0; row < board -> height; row++)
  {
    for(int col = 0; col < board -> width; col++)
    {
      Cell* current = board -> matrix[row][col] -> first;

      while(current)
      {
        watcher_set_cell(row, col, current -> type);

        current = current -> next;
      }
    }
  }
  watcher_refresh();
}

void watcher_snapshot(char* filename)
{
	if(watcher)
	{
		if(fprintf(watcher, "S %s\n", filename) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Cierra el watcher */
void watcher_close()
{
  if(watcher)
  {
    if(fprintf(watcher, "%s\n", "X") >= 0)
    {
      fflush(watcher);
      pclose(watcher);
    }
    watcher = NULL;
  }
}
