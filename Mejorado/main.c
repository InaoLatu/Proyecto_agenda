#include "funciones.h"
#define SALIR '0'
#define CREAR '1'
#define VER '2'
#define LISTAR '3'
#define ELIMINAR '4'
#define PERSONALIZAR '5'

int main(int argc, char *argv[]){
  char usuario[20];
  char opcion;
  int flag=0;
  Tarea *inicio=NULL, *nueva_tarea;
  FILE *fic;
  int longuitud;
  if(argc == 2){
    longuitud = atoi(argv[1]);
  }
  else{
    longuitud = 80;
  }
  caratula(longuitud);
  fic=fopen("tareas.txt","r");
  if(fic == NULL){
    printf("\n\vEl fichero tareas.txt no existe, la informacion se almacenara en uno nuevo.");
  }
  else{
    inicio=cargar_tareas(fic);
    inicio=actualizar(inicio);
    fclose(fic);
  }
  printf("\n\v\tIntroduzca su identificador: ");
  fgets(usuario, 20, stdin);
  int *iden=ar_identificadores(inicio);
  do{
    system("clear");
    caratula(longuitud);
    printf("\n\tAgenda:\n\v\t1)   Crear tarea\n\t2)   Ver tarea\n\t3)   Listar tareas\n\t4)   Eliminar tarea\n\t5)   Personalizar\n\t0)   Salir\n\v\tSelecciona operacion: ");
    do{
      scanf(" %c", &opcion);
      while(getchar() != '\n');
      if(opcion != CREAR && opcion != VER && opcion != LISTAR && opcion != ELIMINAR && opcion != SALIR && opcion != PERSONALIZAR) printf("\n\v\tELECCION INCORRECTA, PULSAR 1,2,3,4,5 o 0: ");
    }
    while(opcion != CREAR && opcion != VER && opcion != LISTAR && opcion != ELIMINAR && opcion != SALIR && opcion != PERSONALIZAR);
    switch(opcion){
    case CREAR:{
      nueva_tarea=crear_tarea(usuario, inicio, iden);
      if(nueva_tarea != NULL && inicio != NULL){
	iden=anexar_tarea(inicio, nueva_tarea, iden);
	flag=1;
      }
      if(nueva_tarea != NULL && inicio == NULL) {
	inicio = nueva_tarea;
	*iden = nueva_tarea->identificador;
	iden=realloc(iden, 2*sizeof(iden));
	*(iden+1) = 0;
	flag=1;
      }
      break;
    }
    case VER:{
      ver_tarea(inicio, usuario);
      break;
    }
    case LISTAR:{
      listar_tareas(inicio, usuario);
      break;
    }
    case ELIMINAR:{
      inicio=eliminar_tarea(inicio, iden, usuario, &flag);
      break;
    }
    case PERSONALIZAR:{
      personalizar();
      break;
    }
    case SALIR:{
      salir(inicio, flag, iden);
      break;
    }
  }
}
while(1);
return 0;
}
