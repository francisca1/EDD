#pragma once
#include <stdbool.h>

/** Todos los posibles tipos de objetos */
typedef enum
{
  EMPTY = 0,
  BABA = 1,
  KEKE = 2,
  MEME = 3,
  WALL = 4,
  KEY = 5,
  DOOR = 6,
  ICE = 7,
  LAVA = 8,
  FLAG = 9,
  STAR = 10,
  JELLY = 11,
  CRAB = 12,
  ROCK = 13,
  SKULL = 14,
  ALGAE = 15,
  BRICK = 16,
  PILLAR = 17,
  HEDGE = 18
} Type;

/** Propiedades de los objetos */
typedef enum
{
  YOU = 0,
  STOP = 1,
  PUSH = 2,
  OPEN = 3,
  SHUT = 4,
  WIN = 5,
  DEFEAT = 6,
  MELT = 7,
  HOT = 8
} Property;

/** Lista ligada de reglas */
typedef struct rule
{
  /** Siguiente regla */
  struct rule* next;
  /** Propiedad */
  Property prop;
} Rule;

Rule* rules[19];

/** Retorna True si el type dado tiene la propiedad dada */
bool is_rule(Type type, Property prop);

/** Inicializa las reglas a partir del archivo de reglas */
void initialize_rules(char* filename);

/** Libera las reglas */
void destroy_rules();
