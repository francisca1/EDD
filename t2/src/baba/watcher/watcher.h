#pragma once

#include "../board/board.h"


/** Abre una interfaz de las dimensiones especificadas */
void watcher_open(int height, int width);

/** Dibuja el tablero completo */
void watcher_draw_board(Board* board);

/** Guarda una imagen .png con el estado actual de la interfaz */
void watcher_snapshot(char* filename);

/** Cierra la interfaz */
void watcher_close();
