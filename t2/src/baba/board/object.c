#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Crea la regla dada */
static void rule_init(Type type, Property prop)
{
  // Creo la regla
  Rule* rule = malloc(sizeof(Rule));

  // Agrego la propiedad a las propiedades del tipo dado
  rule -> prop = prop;
  rule -> next = rules[type];
  rules[type] = rule;
}

/** Libero recursivamente las reglas de la lista */
static void rule_destroy(Rule* rule)
{
  if (rule)
  {
    rule_destroy(rule -> next);
    free(rule);
  }
}

/** Retorna True si el type dado tiene la propiedad dada */
bool is_rule(Type type, Property prop)
{
  // Obtengo la lista ligada del typo dado
  Rule* rule = rules[type];

  // Itero por las reglas buscando la propiedad dada
  while (rule)
  {
    if (rule -> prop == prop)
    {
      return true;
    }
    rule = rule -> next;
  }
  return false;
}

/** Inicializa las reglas a partir del archivo de reglas */
void initialize_rules(char* filename)
{
  // Abro el archivo
  FILE* file = fopen(filename, "r");

  // Leo el numero de reglas
  int rule_count;
  fscanf(file, "%d", &rule_count);

  // Itero por las reglas creandolas
  for (int i = 0; i < rule_count; i++)
  {
    // Type y Property
    char type_name[7];
    char prop_name[7];
    fscanf(file, "%s IS %s", type_name, prop_name);

    // Obtengo el type segun el nombre
    Type type;
    if (!strcmp(type_name, "BABA")) type = BABA;
    else if (!strcmp(type_name, "KEKE")) type = KEKE;
    else if (!strcmp(type_name, "MEME")) type = MEME;
    else if (!strcmp(type_name, "WALL")) type = WALL;
    else if (!strcmp(type_name, "KEY")) type = KEY;
    else if (!strcmp(type_name, "DOOR")) type = DOOR;
    else if (!strcmp(type_name, "ICE")) type = ICE;
    else if (!strcmp(type_name, "LAVA")) type = LAVA;
    else if (!strcmp(type_name, "FLAG")) type = FLAG;
    else if (!strcmp(type_name, "STAR")) type = STAR;
    else if (!strcmp(type_name, "JELLY")) type = JELLY;
    else if (!strcmp(type_name, "CRAB")) type = CRAB;
    else if (!strcmp(type_name, "ROCK")) type = ROCK;
    else if (!strcmp(type_name, "SKULL")) type = SKULL;
    else if (!strcmp(type_name, "ALGAE")) type = ALGAE;
    else if (!strcmp(type_name, "BRICK")) type = BRICK;
    else if (!strcmp(type_name, "PILLAR")) type = PILLAR;
    else if (!strcmp(type_name, "HEDGE")) type = HEDGE;
    else
    {
      printf("ERROR: type invalido\n");
      exit(1);
    }

    // Obtengo la propiedad a partir del nombre
    Property prop;
    if (!strcmp(prop_name, "YOU")) prop = YOU;
    else if (!strcmp(prop_name, "STOP")) prop = STOP;
    else if (!strcmp(prop_name, "PUSH")) prop = PUSH;
    else if (!strcmp(prop_name, "OPEN")) prop = OPEN;
    else if (!strcmp(prop_name, "SHUT")) prop = SHUT;
    else if (!strcmp(prop_name, "WIN")) prop = WIN;
    else if (!strcmp(prop_name, "DEFEAT")) prop = DEFEAT;
    else if (!strcmp(prop_name, "MELT")) prop = MELT;
    else if (!strcmp(prop_name, "HOT")) prop = HOT;
    else
    {
      printf("%s\n", prop_name);
      printf("ERROR: property invalido\n");
      exit(1);
    }

    // Creo regla
    rule_init(type, prop);
  }

  // Cierro el archivo
  fclose(file);
}

/** Libera las reglas */
void destroy_rules()
{
  // Libero cada lista ligada
  for (int i = 0; i < 19; i++)
  {
    rule_destroy(rules[i]);
  }
}
