#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

struct tarea{
  char tipo[10];
  int identificador;
  char descripcion[100];
  char propietario[20];
  int prioridad;
  char estado[20];
  char plazo[20];
  char dependencias[20];
  struct tarea *siguiente;
};


typedef struct tarea Tarea;


Tarea *cargar_tareas(FILE*); // Crea una lista de nodos enlazados con la informacion de un fichero
char *fecha_ordenador(); // Devuelve una cadena de caracteres con la fecha del ordenador
int *ar_identificadores(Tarea*); // Devuelve un array de enteros con los identificadores de la lista de nodos
int *ar_dependencias_dir(char*); // Convierte una cadena de caracteres numericos separados por comas en un array de enteros
int *ar_dependencias_indir(int*, Tarea*); // Determina de que tareas depende indirectamente una
long diftiempo(char*); // Devuelve el numero de segundos de diferencia entre la fecha del ordenador y la introducida por el usuario
void caratula(int ); // Imprime la caractula del gestor de tareas
Tarea *actualizar(Tarea *); // Modifica el campo 'estado' de la lista de tareas si es necesario
int *quitar_repetidos(int *, int); // Sustituye los enteros repetidos por un numero negativo 
int *dep_inv(int, Tarea*); // Determina que tareas dependen de la que se desea mostrar
void imprimir_tareas(Tarea *, FILE *); // Imprime por fichero una lista de tareas seleccionadas
int nuevo_identificador(int *); // Determina un identificador libre parea asignarselo a una tarea nueva
Tarea *seleccionar_tareas(Tarea *, char *, char *); // Selecciona las tareas que cumplen el usuario y el estado
void error_memoria(); // Finaliza el programa si ha ocurrido un problema al reservar memoria
int *anexar_tarea(Tarea *, Tarea *, int *); // Enlaza la segunda tarea al final de la lista apuntada por la primera y añade el identificador de la nueva tarea
void liberar_lista(Tarea *); // Libera la memoria de la lista apuntada por la tarea
char *strtolower(char *); // Convierte un string a minusculas
void error_abrir(char *); // Finaliza el programa si hubo un error al abrir un fichero
void error_cerrar(char *); // Finaliza el programa si hubo un error al cerrar un fichero
int *quitar_tarea(int *, int ); // Sustituye el identificador de la tarea borrada por un -1
int comprobarfecha(char *); // Comprueba si la fecha es valida
int bisiesto(int ); // Determina si un año es bisiesto
int comprueba(int, int, int); // Comprueba si la fecha tiene coherencia

Tarea *crear_tarea(char *, Tarea *, int *);
int ver_tarea(Tarea *, char *);
int listar_tareas(Tarea *, char *);
int salir(Tarea *, int, int *);
