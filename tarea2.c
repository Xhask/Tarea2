#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  char director[300];
  char genre[100]; // Nuevo campo para almacenar el género
  float rating;
  int year;
} Film;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por director");
  puts("4) Buscar por género");
  puts("5) Buscar por década");
  puts("6) Buscar por rango de calificaciones");
  puts("7) Buscar por década y género");
  puts("8) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void cargar_peliculas(Map *pelis_byid) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);          // Asigna ID
    strcpy(peli->title, campos[5]);       // Asigna título
    strcpy(peli->director, campos[14]);   // Asigna director
    strcpy(peli->genre, campos[9]);       // Asigna género
    peli->year = atoi(campos[10]);        // Asigna año, convirtiendo de cadena a entero

    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
}

/**
 * Busca y muestra la información de una película por su ID en un mapa.
 */
void buscar_por_id(Map *pelis_byid) {
  char id[100]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(pelis_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli =
        pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d\n", peli->title, peli->year);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}

/**
 * Busca y muestra la información de películas por su género en un mapa.
 */
void buscar_por_genero(Map *pelis_byid) {
  char genero[100]; // Buffer para almacenar el género de la película

  // Solicita al usuario el género de la película
  printf("Ingrese el género de la película: ");
  scanf("%s", genero); // Lee el género del teclado

  // Itera sobre el mapa para buscar y mostrar películas del género ingresado
  MapPair *pair = map_first(pelis_byid);
  int found = 0; // Variable para rastrear si se encontró alguna película del género
  while (pair != NULL) {
    Film *peli = pair->value;
    // Compara el género de la película actual con el género ingresado por el usuario
    if (strcmp(peli->genre, genero) == 0) {
      // Si el género coincide, muestra la información de la película
      printf("ID: %s, Título: %s, Director: %s, Año: %d\n", peli->id, peli->title,
             peli->director, peli->year);
      found = 1; // Indica que se encontró al menos una película del género
    }
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }

  // Si no se encontraron películas del género ingresado, informa al usuario
  if (!found) {
    printf("No se encontraron películas del género %s\n", genero);
  }
}

/**
 * Busca y muestra la información de películas por su director en un mapa.
 */
void buscar_por_director(Map *pelis_byid) {
  char director[300]; // Buffer para almacenar el nombre del director

  // Solicita al usuario el nombre del director
  printf("Ingrese el nombre del director: ");
  scanf("%s", director); // Lee el nombre del director del teclado

  // Itera sobre el mapa para buscar y mostrar películas del director ingresado
  MapPair *pair = map_first(pelis_byid);
  int found = 0; // Variable para rastrear si se encontró alguna película del director
  while (pair != NULL) {
    Film *peli = pair->value;
    // Compara el nombre del director de la película actual con el nombre ingresado por el usuario
    if (strcmp(peli->director, director) == 0) {
      // Si el director coincide, muestra la información de la película
      printf("ID: %s, Título: %s, Año: %d, Género: %s\n", peli->id, peli->title,
             peli->year, peli->genre);
      found = 1; // Indica que se encontró al menos una película del director
    }
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }

  // Si no se encontraron películas del director ingresado, informa al usuario
  if (!found) {
    printf("No se encontraron películas del director %s\n", director);
  }
}

/**
 * Busca y muestra la información de películas por década en un mapa.
 */
void buscar_por_decada(Map *pelis_byid) {
  char decada[5]; // Buffer para almacenar el nombre de la década

  // Solicita al usuario el nombre de la década
  printf("Ingrese la década (ejemplo: 1980s, 2010s): ");
  scanf("%s", decada); // Lee el nombre de la década del teclado

  // Itera sobre el mapa para buscar y mostrar películas de la década ingresada
  MapPair *pair = map_first(pelis_byid);
  int found = 0; // Variable para rastrear si se encontró alguna película de la década
  while (pair != NULL) {
    Film *peli = pair->value;
    // Extrae el año de la película actual y calcula la década correspondiente
    int decada_peli = peli->year / 10 * 10;
    char decada_peli_str[5];
    sprintf(decada_peli_str, "%ds", decada_peli); // Convierte la década a string

    // Compara la década de la película actual con la década ingresada por el usuario
    if (strcmp(decada_peli_str, decada) == 0) {
      // Si la década coincide, muestra la información de la película
      printf("ID: %s, Título: %s, Director: %s, Género: %s\n", peli->id, peli->title,
             peli->director, peli->genre);
      found = 1; // Indica que se encontró al menos una película de la década
    }
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }

  // Si no se encontraron películas de la década ingresada, informa al usuario
  if (!found) {
    printf("No se encontraron películas de la década %s\n", decada);
  }
}

/**
 * Busca y muestra la información de películas por rango de calificaciones en un mapa.
 */
void buscar_por_rango_calificaciones(Map *pelis_byid) {
  float rango_min, rango_max; // Variables para almacenar el rango de calificaciones

  // Solicita al usuario el rango de calificaciones
  printf("Ingrese el rango de calificaciones (ejemplo: 6.0-6.4): ");
  scanf("%f-%f", &rango_min, &rango_max); // Lee el rango de calificaciones del teclado

  // Itera sobre el mapa para buscar y mostrar películas dentro del rango de calificaciones
  MapPair *pair = map_first(pelis_byid);
  int found = 0; // Variable para rastrear si se encontró alguna película dentro del rango
  while (pair != NULL) {
    Film *peli = pair->value;
    // Compara la calificación de la película actual con el rango ingresado por el usuario
    if (peli->rating >= rango_min && peli->rating <= rango_max) {
      // Si la calificación está dentro del rango, muestra la información de la película
      printf("ID: %s, Título: %s, Director: %s, Año: %d\n", peli->id, peli->title,
             peli->director, peli->year);
      found = 1; // Indica que se encontró al menos una película dentro del rango
    }
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }

  // Si no se encontraron películas dentro del rango de calificaciones, informa al usuario
  if (!found) {
    printf("No se encontraron películas dentro del rango de calificaciones %.1f-%.1f\n", rango_min, rango_max);
  }
}

/**
 * Busca y muestra la información de películas por década y género en un mapa.
 */
void buscar_por_decada_y_genero(Map *pelis_byid) {
  char decada[5]; // Buffer para almacenar el nombre de la década
  char genero[100]; // Buffer para almacenar el género de la película

  // Solicita al usuario la década y el género de la película
  printf("Ingrese la década (ejemplo: 1980s, 2010s): ");
  scanf("%s", decada); // Lee la década del teclado
  printf("Ingrese el género de la película: ");
  scanf("%s", genero); // Lee el género del teclado

  // Itera sobre el mapa para buscar y mostrar películas de la década y género ingresados
  MapPair *pair = map_first(pelis_byid);
  int found = 0; // Variable para rastrear si se encontró alguna película del género y década
  while (pair != NULL) {
    Film *peli = pair->value;
    // Extrae el año de la película actual y calcula la década correspondiente
    int decada_peli = peli->year / 10 * 10;
    char decada_peli_str[5];
    sprintf(decada_peli_str, "%ds", decada_peli); // Convierte la década a string

    // Compara la década y el género de la película actual con la década y género ingresados por el usuario
    if (strcmp(decada_peli_str, decada) == 0 && strcmp(peli->genre, genero) == 0) {
      // Si la década y el género coinciden, muestra la información de la película
      printf("ID: %s, Título: %s, Director: %s, Calificación: %.1f\n", peli->id, peli->title,
             peli->director, peli->rating);
      found = 1; // Indica que se encontró al menos una película del género y década
    }
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }

  // Si no se encontraron películas del género y década ingresados, informa al usuario
  if (!found) {
    printf("No se encontraron películas del género %s de la década %s\n", genero, decada);
  }
}

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);

  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid);
      break;
    case '2':
      buscar_por_id(pelis_byid);
      break;
    case '3':
      buscar_por_director(pelis_byid);
      break;
    case '4':
      buscar_por_genero(pelis_byid);
      break;
    case '5':
      buscar_por_decada(pelis_byid);
      break;
    case '6':
      buscar_por_rango_calificaciones(pelis_byid);
      break;
    case '7':
      buscar_por_decada_y_genero(pelis_byid);
      break;
    default:
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  // Liberar la memoria utilizada por el mapa y las películas almacenadas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    free(pair->value); // Liberar la memoria de la película
    pair = map_next(pelis_byid); // Avanzar al siguiente par en el mapa
  }
  map_destroy(pelis_byid); // Liberar la memoria del mapa

  return 0;
}