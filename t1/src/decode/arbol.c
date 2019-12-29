#include <stdlib.h>
#include <stdio.h>
#include "arbol.h"

Nodo *arbol_init(int valor){
    Nodo *nodo = malloc(sizeof(Nodo));
    nodo->cero = NULL;
    nodo->uno = NULL;
    nodo->valor = valor;
    nodo->count = 1;
    return nodo;
}

void agregar_palabra(Nodo *Raiz, uint8_t *palabra, int largo){
    Nodo *padre = Raiz;
    Nodo *aux;
    for (int i = 0; i < largo; i++)
    {
        // por cada letra en la palabra
        if (palabra[i] ==(uint8_t)0) // si es 0
        {
            if (padre->cero) // reviso si tiene hijo 0
            {
                padre->cero->count++; // si tiene, le sumo 1 al contador
            } else{
                aux = arbol_init(0);
                padre->cero = aux;
            }
            padre = padre->cero;
        }
        else
        {   // si es 1

            if (padre->uno) // reviso si tiene hijo 1
            {
                padre->uno->count++; // si tiene, le sumo 1 al contador
            } else{
                aux = arbol_init(1);
                padre->uno = aux;
            }
            padre = padre->uno;
        }
    }
    
}

void destruir_arbol(Nodo *raiz){
    if (raiz != NULL)
    {
        destruir_arbol(raiz->cero);
        destruir_arbol(raiz->uno);
        free(raiz);
    }
}

int buscar_palabra(uint8_t *palabra, int largo, Nodo *arbol, int actual){
    if (!arbol)
    {
        //si no hay nodo, retorno que no esta la palabra
        return 0;
    }
    if (largo == 0)
    {
        return arbol->count;
    }

    if (palabra[actual]==0)
    {
        // si la letra actual es un 0 busco en el subarbol 0.
        return buscar_palabra(palabra, largo - 1, arbol->cero, actual + 1);
    } else {
        // si la letra actual es 1 busco en el subarbol 1.
        return buscar_palabra(palabra, largo - 1, arbol->uno, actual + 1);
    }
    
}

void print_arbol(Nodo *arbol){
    printf("************************\n");
    if (!arbol)
    {
        return;
    }
    printf("padre: %d count: %d \n", arbol->valor, arbol->count);
    if (arbol->cero && arbol->uno)
    {
        printf("h1 : %d c1:%d  h2: %d c2: %d\n", arbol->cero->valor, arbol->cero->count, arbol->uno->valor, arbol->uno->count);
        print_arbol(arbol->cero);
        print_arbol(arbol->uno);
        return;
    }
    else if (arbol->cero)
    {
        printf("h: %d count: %d\n", arbol->cero->valor, arbol->cero->count);
        print_arbol(arbol->cero);
        return;
    }
    else if (arbol->uno)
    {
        printf("h %d\n count: %d\n", arbol->uno->valor, arbol->uno->count);
        print_arbol(arbol->uno);
        return;
    }
    else
    {
        return;
    }
    printf("************************\n");
}