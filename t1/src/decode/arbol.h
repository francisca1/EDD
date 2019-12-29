#pragma once
#include <stdint.h>

typedef struct Nodo
{

    uint8_t valor;
    int count;
    struct Nodo *cero;
    struct Nodo *uno;

} Nodo;

void agregar_palabra(Nodo *Raiz, uint8_t *palabra, int largo);
void destruir_arbol(Nodo *raiz);
int buscar_palabra(uint8_t *palabra, int largo, Nodo *arbol, int actual);
void print_arbol(Nodo *arbol);
Nodo *arbol_init(int valor);