#include "watcher.h"
#include <stdio.h>
#include <stdlib.h>

#define WATCHER "./beholder"

static FILE* watcher = NULL;

/** Abre una ventana para mostrar una matriz de las dimensiones dadas */
void watcher_open(int height, int width, int max_districts)
{
	char command[256];

	sprintf(command, "%s %d %d %d", WATCHER, height, width, max_districts);

	if(watcher) watcher_close();

	watcher = popen(command, "w");
}

/** Indica que tipo de votante tiene esta celda */
void watcher_set_cell_type(int row, int col, CellType cellType)
{
	if(watcher)
	{
		if(fprintf(watcher, "T %d %d %u\n", row, col, cellType) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Indica el número del distrito al que pertenece esta celda */
void watcher_set_cell_district(int row, int col, int district_id)
{
	if(watcher)
	{
		if(fprintf(watcher, "D %d %d %d\n", row, col, district_id) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Indica que partido está ganando en el distrito dado */
void watcher_set_district_color (int district_id, CellType winner)
{
	if(fprintf(watcher, "W %d %u\n", district_id, winner) < 0)
	{
		watcher_close();
	}
	else
	{
		fflush(watcher);
	}
}

/** Imprime el contenido de la ventana a una imagen .PNG */
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

/** Cierra y libera los recursos de la ventana */
void watcher_close()
{
	if (watcher)
	{
    if(fprintf(watcher, "%s\n", "X") >= 0)
    {
      fflush(watcher);
      pclose(watcher);
    }
    watcher = NULL;
  }
}
