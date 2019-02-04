#include "funciones.h"

Tarea *cargar_tareas(FILE *p_fic){           //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  getc(p_fic);
  if(feof(p_fic)) return NULL;   //Comprobamos si no hay tareas 
  rewind(p_fic);
  Tarea *p_lista, *p_aux;
  p_lista=malloc(sizeof(Tarea));
  if(p_lista == NULL) error_memoria();
  p_aux=p_lista;
  while(!feof(p_fic)){
    fgets(p_aux->tipo, 11, p_fic);
    fscanf(p_fic,"%i",&p_aux->identificador);
    fgetc(p_fic);
    fgets(p_aux->descripcion, 101, p_fic);
    fgets(p_aux->propietario, 21, p_fic);
    fscanf(p_fic,"%i", &p_aux->prioridad);
    fgetc(p_fic); 
    fgets(p_aux->estado, 21, p_fic);
    fgets(p_aux->plazo, 21, p_fic);
    fgets(p_aux->dependencias, 21, p_fic);
    fgetc(p_fic);
    fgetc(p_fic);
    if(!feof(p_fic)){       //Comprobamos si se llego o no al final del fichero, es decir, si hay mas tareas o no
      (p_aux->siguiente)=malloc(sizeof(Tarea));
      if(p_aux->siguiente == NULL) error_memoria();
      p_aux=p_aux->siguiente;
    }
    else p_aux->siguiente = NULL;    //Si se llega la final del fichero se pone el ultimo nodo de la lista apuntando a NULL
  }
  //Si se ejecuta correctamente se creara la lista de nodos con la informacion de las tareas del fichero y la funcion devuelve el puntero al inicio de la lista 
  
  return p_lista;
}


char *fecha_ordenador(){         //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//       
  time_t tiempo;      // Variable capaz de representar el tiempo (generalmente en segundos)
  char *fechayhora=(char*)malloc(20*sizeof(char));
  if(fechayhora == NULL) error_memoria();
  struct tm *p_tiempo;   // Estructura contenida en la libreria time.h cuyos campos corresponden al dia, mes, año, hora, minutos...
  tiempo = time(NULL); // Obtenemos la hora del ordenador en formato time_t (segundos)
  p_tiempo = localtime(&tiempo); // Obtenemos un strutc tm cuyos campos son completados a partir del time_t(tiempo) obtenido previamente 
  strftime(fechayhora, 21, "%d/%m/%Y %H:%M\n", p_tiempo); // Copia en la cadena fechayhora en el formato indicado por " " los datos del struct apuntado por p-tiempo
  return fechayhora;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//

long diftiempo(char *info){    //Recibe como parametro la cadena de caracteres que conforman la fecha introducida 
  long dif;
  char *p = info, *aux;     // Hacemos que el puntero p apunte al comienzo de la cadena de caracteres recibida 
  struct tm fecha1, fecha2; 
  
  fecha1.tm_mday = atoi(p);         //Dia del mes
  p= p+3;
  fecha1.tm_mon = atoi(p);          //Mes 
  p=p+3;
  fecha1.tm_year= atoi(p) - 1900;   //Año 
  p = p+5;
  fecha1.tm_hour= atoi(p);          //Hora
  p = p+3;
  fecha1.tm_min = atoi(p);          //Minutos 
  fecha1.tm_sec = 0;                
  
  // Completamos el struct tm con los datos obtenidos de la fecha y la hora introducida por el usuario 
  
  p = fecha_ordenador(); // Hacemos que el puntero p apunte al comienzo de la cadena con la fecha del ordenador  
  aux = p;                            //Realizamos el mismo proceso que con la fecha introducida
  fecha2.tm_mday = atoi(aux);
  aux= p+3;
  fecha2.tm_mon = atoi(aux);
  aux=aux+3;
  fecha2.tm_year= atoi(aux) - 1900;
  aux = aux+5;
  fecha2.tm_hour= atoi(aux);
  aux = aux+3;
  fecha2.tm_min = atoi(aux);
  fecha2.tm_sec = 0;
  // Completamos un struct tm con los datos de la fecha del ordenador
  
  free(p); // Liberamos la zona de memoria del malloc de la funcion fecha_ordenador
  dif = difftime(mktime(&fecha1), mktime(&fecha2)); // Calculamos la diferencia en seg entre las dos fechas a partir de notaciones compactas (mktime)
  // El resultado sera positivo si metemos una fecha posterior 
  return dif;
}


int *ar_identificadores(Tarea *p_inicio){  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  int *iden, i=0;
  iden=(int*)malloc(sizeof(int));
  if(iden == NULL) error_memoria();
  while(p_inicio!=NULL){ // Recorremos la lista de nodos hasta el final de la misma 
    iden=realloc(iden, (i+1)*sizeof(int)); // Redimensionamos el array a medida que hay mas tareas
    if(iden == NULL) error_memoria();
    *(iden+i)=p_inicio->identificador;     //Introducimos el identificador de la tarea en la ultima posicion del array de enteros 
    p_inicio=p_inicio->siguiente;      
    i++;
  }
  iden=realloc(iden, (i+1)*sizeof(int)); // Reservamos una posicion mas
  if(iden == NULL) error_memoria();      // En la que introduciremos un 0 para marcar el final del array         
  *(iden+i) = 0;  
  return iden;
}

int *ar_dependencias_dir(char *ar){  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  int *dep=malloc(sizeof(int));
  if(dep == NULL) error_memoria();
  int i=0, j=0;
  while(*(ar+i) != '\0') {    //Recorremos la cadena de caracteres hasta el final
    dep=realloc(dep, (j+1)*sizeof(int));
    if(dep == NULL) error_memoria();                                 
   *(dep+j) = atoi(ar+i);                        //Vamos convirtiendo las dependencias en enteros y guardando estos en un array
    while(*(ar+i) != ',' && *(ar+i) != '\n'){
      i++;
    }
    i++; //Aumentamos la variable i  para desplazarnos por la cadena de caracteres 
    j++;  // Incrementamos la variable j para poder ir reservando memoria a medida que nos movemos por el array de enteros
  }      
  dep=realloc(dep, (j+1)*sizeof(int));
  if (dep == NULL) error_memoria();
  *(dep+j) = 0; // Ponemos un 0 en la ultima posicion del array de enteros para indicar el final
  return dep;     //Devolvemos el puntero apuntando al inicio del array de enteros 
}

int *ar_dependencias_indir(int *dep_dir, Tarea *inicio) {  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  int i=0, flag=0;
  int *dep_indir;
  Tarea *aux;
  char cad[80];
  while(*(dep_dir+i) != 0) { // Recorremos al array de enteros(dependencias directas) hasta el 0(que introdujimos en la funcion anterior) 
    aux = inicio;
    while(aux != NULL){ // Recorremos la lista de tareas hasta el final 
      if(*(dep_dir+i) == aux->identificador){ // Buscamos la tarea cuyo identificador sea el mismo con que el estamos operando
	if(flag==0){
	  if(strcmp(aux->dependencias, "-\n") != 0){    // Comprobamos si la tarea tiene dependencias
	    flag=1;
	    strcpy(cad, aux->dependencias);
	  }
	}  
               //Copiamos la dependencias en cad si es la primera vez que realizamos esta operacion(primer identificador de una tarea con dependencias)
      
	else{
	  if(strcmp(aux->dependencias, "-\n") != 0) // Comprobamos si la tarea tiene dependencias
	    strcat(cad,aux->dependencias);
	}
	// Si ya hemos operado con otro identificador previamente concatenamos las dependencias de la tarea con las que ya habiamos copiado en cad
	break;
      }
      aux=aux->siguiente;
    }
    i++;   //Aumentamos i para recorrer el array de enteros 
  }
  dep_indir=ar_dependencias_dir(cad); // Convertimos la cadena de caracteres en una de enteros
  return dep_indir;
}


void caratula(int longuitud){     //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  int lineas, caracteres;
  if(longuitud > 145) longuitud = 145;
  if(longuitud < 80) longuitud = 80;
  system("clear all");
  for(lineas=0;lineas<2;lineas++){
    for(caracteres=0;caracteres<longuitud;caracteres++)
      printf("*");
    printf("\n");
  }
  for(caracteres=0;caracteres<(longuitud/2)-16;caracteres++) printf("*");
  printf(" GESTOR DE TAREAS DE UNA AGENDA ");
  for(caracteres=0;caracteres<(longuitud/2)-16;caracteres++) printf("*");
  if(longuitud % 2) printf("*");
  printf("\n");
  for(lineas=0;lineas<2;lineas++){
    for(caracteres=0;caracteres<longuitud;caracteres++)
      printf("*");
    printf("\n");
  }
  char *fecha=fecha_ordenador();
  printf("Fecha: %s", fecha);   //Sacamos por pantalla la fecha y hora actual del ordenador(en el momento de ejecucion)
  free(fecha);  //Liberamos memoria
  return;
}


Tarea *actualizar(Tarea *inicio){    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  if(inicio == NULL) return NULL;
  Tarea *aux;
  aux=inicio;
  while(aux != NULL){
    if(diftiempo(aux->plazo) < 0 && strcmp(aux->tipo, "simple\n") != 0){
      strcpy(aux->estado, "terminada\n");          //Comprobamos si el plazo de la tarea es posterior a la fecha actual y si es asi 
    }                                              // copiamos en el campo estado "terminada\n"
    aux=aux->siguiente;
  }
  return inicio;
}

int *dep_inv (int iden, Tarea *inicio){
  int *inv;
  int *aux;
  int i, j=0;
  inv=malloc(sizeof(int));        //Reservamos memoria para la cadena de enteros 
  if(inv == NULL) error_memoria();
  while(inicio != NULL){ // Recorremos la lista de tareas
    aux=ar_dependencias_dir(inicio->dependencias); // Conseguimos las dependencias de la tarea con la que estamos operando
    i=0;
    while(*(aux+i) != 0){      // Recorremos el array de dependencias obtenido previamente
      if(*(aux+i) == iden){    
	inv=realloc(inv, (j+1)*sizeof(int));   //Si el identificador recibido como parametro esta entre las dependencias de la tarea 
	if(inv == NULL) error_memoria();      // incluimos el identificador de la tarea en la cadena de enteros y redimensionamos esta 
	*(inv+j) = inicio->identificador;
	j++;
	break;
      }
 
      i++;
    }
    free(aux);
    inicio=inicio->siguiente;
  }
  inv=realloc(inv, (j+1)*sizeof(int));  //Cuando ya hayamos recorrido toda la lista de tareas introducimos un 0 al final de la lista de enteros 
  if(inv == NULL) error_memoria();        //para marcar el final de esta 
  *(inv+j) = 0; 
  return inv;    //Devolvemos el puntero inv que apunta al comienzo de la lista de enteros 
}


int *quitar_repetidos(int *array,int numerotareas) {  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  int flag;
  int i;
  int h;
  for(i=1;i<numerotareas;i++){ 
    flag=0;
    for(h=0;h<numerotareas;h++){ // Recorrremos  el array de enteros 
      if(flag==1 && array[h]==i){ 
	array[h]=-1;	
	continue;
      }
      // Sustituimos los identificadores repetidos por un -1 
      if(array[h]==i){ 
	flag=1;
      }  
        //Con el flag indicamos que es la primera vez que encontramos el identificador
    } 
  } 
  return array;   //Devolvemos el puntero al principio de la cadena de enteros ya sin identificadores repetidos
}


void imprimir_tareas(Tarea *inicio, FILE *fic){   //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  while(inicio != NULL){
    fputs(inicio->tipo, fic);               ///Guardaremos la lista de tareas en el fichero tareas.txt
    fprintf(fic, "%i\n", inicio->identificador);
    fputs(inicio->descripcion, fic);
    fputs(inicio->propietario, fic);
    fprintf(fic, "%i\n", inicio->prioridad);
    fputs(inicio->estado, fic);
    fputs(inicio->plazo, fic);
    fputs(inicio->dependencias, fic);
    if(inicio->siguiente != NULL) fprintf(fic, "*\n"); // Pondremos un * en el fichero siempre y cuando no sea la ultima tarea 
    inicio=inicio->siguiente;
  }
  return;
}
 

Tarea *seleccionar_tareas(Tarea *inicio, char *usuario, char *estado){  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  Tarea *seleccionadas=NULL;
  Tarea *aux;
  char estado_aux[10];
  if(inicio == NULL) return NULL;
  strcpy(estado_aux, estado);
  do{
    if(strcmp(estado, "todas\n") == 0) strcpy(estado_aux, inicio->estado);
    /* En caso de que se seleccionen todas se copia en estado_aux el estado de la tarea con la que se esta trabajando */
    if(strcmp(strtolower(inicio->propietario), strtolower(usuario)) == 0 && strcmp(inicio->estado, estado_aux) == 0){ 
   // Comprobamos si la tarea pertenece al usuario y si tiene el estado seleccionado 

      if(seleccionadas == NULL){             //Si es la primera tarea que cogemos reservamos memoria y tendremos el puntero
	seleccionadas=malloc(sizeof(Tarea));  // seleccionadas al principio de la lista que crearemos 
	if(seleccionadas == NULL) error_memoria();
	*seleccionadas=*inicio;     //Copiamos el struct 
	aux=seleccionadas;
      }

      else{
	aux->siguiente=malloc(sizeof(Tarea));
	if(aux->siguiente == NULL) error_memoria();
	aux=aux->siguiente;
 	*aux=*inicio;     //Copiaremos los demas structs en el caso de que cumplan los requisitos 
      }
    
    }
    inicio=inicio->siguiente;
  }
  while(inicio != NULL);
  if(seleccionadas != NULL) aux->siguiente=NULL;
  return seleccionadas;
}


int nuevo_identificador(int *iden){   //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  int j=1,i=0,flag=0;
  while(1){
    flag=0;
    i=0;
    while(*(iden+i) != 0){
	if(*(iden+i) == j){
	  flag=1;        // Indicamos cuando nos encontramos con el identificador 
	  break;
	}
      i++;
    }
    if(flag == 0){
      break;            //Salimos del while cuando la variable j no coincida con ninguno de los identificadores existentes 
    }          
    j++;
  }
  return j;
}


void error_memoria(){      //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  printf("\n\vERROR AL RESERVAR MEMORIA\n\vPulsar ENTER para finalizar el programa.");
  while(getchar() != '\n');
  system("clear all");
  exit(EXIT_FAILURE);
}

int *anexar_tarea(Tarea *inicio, Tarea *nueva_tarea, int *iden){  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  int i=0;
  while(inicio->siguiente != NULL) inicio=inicio->siguiente;
  inicio->siguiente=nueva_tarea;
  //Incluimos la nueva tarea al final de la lista de nodos 

  while(*(iden+i) != 0){
    i++;
  }
  *(iden+i) = nueva_tarea->identificador; // Sustituimos el 0 final por el identificador de la tarea nueva 
  i++;
  iden=realloc(iden, (i+1)*sizeof(int));
  if(iden == NULL) error_memoria();
  *(iden+i) = 0;   //Volvemos a introducir el 0 para marcar el final
  return iden;
}

void liberar_lista(Tarea *inicio){      //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  Tarea *aux;
  while(inicio != NULL){
    aux=inicio->siguiente;
    free(inicio);
    inicio=aux;
  }
  return;         //Liberamos la lista de nodos 
}

char *strtolower(char *str){        //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  int i=0;
  while(str[i] != '\0'){
    str[i]=tolower(str[i]);
    i++;
  }
  return str;         //Convertimos a minuscula la cadena recibida
}

void error_abrir(char *nombre){    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  printf("ERROR: No fue posible abrir el fichero: %s\n\vPulsar ENTER para iniciar la salida del programa.", nombre);
  while(getchar() != '\n');
  system("clear all");
  exit(EXIT_FAILURE);
}

void error_cerrar(char *nombre){  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  printf("ERROR: No fue posible cerrar el fichero: %s\n\vPulsar ENTER para iniciar la salida del programa.", nombre);
  while(getchar() != '\n');
  system("clear");
  exit(EXIT_FAILURE);
}

int *quitar_tarea(int *iden, int tarea){  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  int i=0;
  while(*(iden+i) != 0){
    if(*(iden+i) == tarea){
      *(iden+i) = -1;
      break;
    }
    i++;
  }
  return iden;
}

int comprobarfecha(char *str){         //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  char *p=str;
  struct tm fecha1;
  int flag;
  fecha1.tm_mday=atoi(p);
  p=p+3;
  fecha1.tm_mon=atoi(p);
  p=p+3;
  fecha1.tm_year=atoi(p)-1900;
  p=p+5;
  fecha1.tm_hour=atoi(p);
  p=p+3;
  fecha1.tm_min=atoi(p);
  fecha1.tm_sec=0;
  flag=comprueba(fecha1.tm_mday,fecha1.tm_mon,fecha1.tm_year);
  return flag;
}                  //Comprobamos si la fecha es posterior a la actual mas 7 dias 



int bisiesto(int year)    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
 {
  int esbisiesto = 0;
  
  if (year % 400 == 0){
    esbisiesto = 1;}
  else if ( year % 4 == 0 && year % 100 != 0 ){
    esbisiesto = 1;}
  return esbisiesto;                           //Comprobamos si el año es bisiesto
}                                


int comprueba(int dia,int mes,int ano){  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  int correcto = 0;
  if (ano >= 100 && ano < 200 ){
    correcto = 1;					   
  }
  if (mes > 12 || mes < 0){                  //Comprobamos si la fecha es correcta(devolviendo un 1) o si la fecha es incorrecta(devolviendo un 0)
    correcto = 0;
    return correcto;
  }
  if(dia >= 1 && dia <=31){
    correcto = 1;
  }
  else{
    correcto=0; 
    return correcto;
  }
  if((mes==1 || mes==3 || mes==5 || mes==7 || mes==8 || mes==10 ||mes==12) && (dia>=1 && dia <=31)){
    correcto = 1;
  }else{     
    if((mes==4 || mes==6 || mes==9 || mes==11 ) && (dia>=1 && dia <=30)){
      correcto = 1;
    }else{
      if(mes==2 && (dia>=1 && dia <=29)){
	if(dia == 29){
	  if (bisiesto(ano)==1){
	    correcto=1;
	  }else{
	    correcto =0;
	    return correcto;
	  }
	}
      }
      else{
	correcto =0; return correcto;}      
    }
  }
  return correcto;
}


Tarea *crear_tarea(char *usuario, Tarea *p_inicio, int *iden){   //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  system("clear");
  printf("\t\t\tCREAR TAREA\n\v");
  Tarea *n_tarea;
  n_tarea=malloc(sizeof(Tarea));
  if(n_tarea == NULL) error_memoria();
  // Reservamos memoria y comprobamos si se ha podido hacer la reserva  

  printf("Introducir los datos de la nueva tarea:");
  do{
    printf("\n\v\tIntroducir tipo de tarea (simple/con plazo): ");
    fgets(n_tarea->tipo, 21, stdin);
    if(strcmp(n_tarea->tipo, "simple\n") != 0 && strcmp(n_tarea->tipo, "con plazo\n") != 0) 
      printf("\tTipo de tarea no valida.");
  }
  while(strcmp(n_tarea->tipo, "simple\n") != 0 && strcmp(n_tarea->tipo, "con plazo\n") != 0);
  if(strcmp(n_tarea->tipo, "con plazo\n") == 0){
    do{
    printf("\n\tIntroducir plazo (dd/mm/aaaa hh:mm): ");
    fgets(n_tarea->plazo, 21, stdin);
    if(comprobarfecha(n_tarea->plazo) == 0) printf("\tFecha no valida.\n");
    }
    while(comprobarfecha(n_tarea->plazo) == 0);
  }
  else strcpy(n_tarea->plazo, "-\n");
  printf("\n\tIntroducir descripcion de la tarea: ");
  fgets(n_tarea->descripcion, 101, stdin);
  do{
    printf("\n\tIntroducir prioridad (1-10): ");
    scanf(" %i", &n_tarea->prioridad);
    while(getchar() != '\n');
    if(n_tarea->prioridad <= 0 || n_tarea->prioridad > 10) printf("\tPrioridad no valida.\n");
  }
  while(n_tarea->prioridad <= 0 || n_tarea->prioridad > 10);
  if(p_inicio == NULL){
    printf("\n\vAVISO: Primera tarea de la lista de tareas. Por defecto no se establecen dependencias.\n\vPulsar ENTER para continuar con la creacion del nuevo nodo.");
    while(getchar() != '\n');
    strcpy(n_tarea->dependencias, "-\n");
  }
  else{
    printf("\n\tIntroducir dependencias (0 si no se establecen dependencias): ");
    fgets(n_tarea->dependencias, 21, stdin);
    if(strcmp(n_tarea->dependencias, "0\n") == 0 || strcmp(n_tarea->dependencias, " \n") == 0) strcpy(n_tarea->dependencias, "-\n");
  }
  //Informacion introducida por el usuario
  
  int i=0, j=0, flag=0;
  Tarea *p_lista=p_inicio;
  while(p_lista != NULL){
    if(strcmp(strtolower(p_lista->descripcion), strtolower(n_tarea->descripcion)) == 0 && strcmp(p_lista->propietario, usuario) == 0){ 
      printf("\n\vAVISO: Tarea ya existente.\n\vPulsar ENTER para volver al menu principal del programa.");
      while(getchar() != '\n');
      free(n_tarea);
      return NULL;
    }
    p_lista = p_lista->siguiente;     //Recorremos la lista de tareas para comprobar si ya existe la que acabamos de crear 
  }
  if(diftiempo(n_tarea->plazo)-7*24*60*60 <= 0 && strcmp(n_tarea->tipo, "con plazo\n") == 0){ 
    printf("\n\vAVISO: La fecha debe ser posterior a la actual.\n\vPulsar ENTER para volver al menu principal del programa.");
    while(getchar() != '\n');
    free(n_tarea);                      //AVISO de que el plazo debe ser posterior al actual
    return NULL;
  }
  if(strcmp(n_tarea->dependencias, "-\n") != 0){
    int *dep;
    dep=ar_dependencias_dir(n_tarea->dependencias);
    while(*(dep+i) != 0){
      flag=0;
      j=0;
      while(*(iden+j) != 0){
	if(*(dep+i) == *(iden+j)){              //Recorremos la lista de enteros comprobando que las dependencias introducidas corresponden con 
	  flag=1;                               // identificadores de tareas existentes en la lista 
	  break;
	}
	j++;
      }
	if(flag == 0){
	  printf("\n\vAVISO: Alguna de las dependencias indicadas en incorrecta.\n\vPulsar ENTER para volver al menu principal del programa.");
	  free(dep);
	  free(n_tarea);
	  while(getchar() != '\n');
	  return NULL;
	}
      i++;
    }
    free(dep);
  }
  

  n_tarea->identificador=nuevo_identificador(iden);
  strcpy(n_tarea->propietario, usuario);
  strcpy(n_tarea->estado, "pendiente\n");
  n_tarea->siguiente = NULL;
  //Completamos los campos del struct con los datos que genera el ordenador automaticamente 

  printf("\nTarea creada con el identificador %i\n\v", n_tarea->identificador);
  printf("Pulsar ENTER para volver al menu principal.");
  while(getchar() != '\n');
  return n_tarea;
}

int ver_tarea(Tarea *p_lista, char *usuario){    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  system("clear");
  printf("\t\t\tVER TAREA\n\v");
  if(p_lista == NULL){
    printf("\n\vAVISO: Lista de tareas vacias.\n\vPulsar ENTER para volver al menu principal del programa.");
    while(getchar() != '\n');
    return 0;
  }
  int tarea, flag=0;
  Tarea *aux=p_lista;
  printf("Introducir el identificador de la tarea: ");
  scanf(" %i", &tarea);
  while(getchar() != '\n');
  while(p_lista != NULL){ // Recorremos la lista de tareas
    if(tarea == p_lista->identificador){ // hasta encontrar la tarea que coincide con el identificador
      if(strcmp(strtolower(p_lista->propietario), strtolower(usuario)) != 0){ // Aviso de tarea no perteneciente al usuario
	printf("\n\vAVISO: Operacion no permitida sobre esta tarea.\n\vPulsar ENTER para volver al menu principal del programa.");
	while(getchar() != '\n');
	return 0;
      }
      printf("\n\tDescripcion: %s", p_lista->descripcion);
      printf("\n\tPrioridad: %i\n", p_lista->prioridad);
      printf("\n\tEstado: %s", p_lista->estado);
      printf("\n\tTipo: %s", p_lista->tipo);
      if(strcmp(p_lista->tipo, "con plazo\n") == 0){ // Comprobamos si es con plazo
	printf("\n\tPlazo: %s", p_lista->plazo);
	if(strcmp(p_lista->estado, "pendiente\n") == 0){ // Si ademas la tarea no esta terminada calculamos los dias que restan para que termine
	  long dias=diftiempo(p_lista->plazo)/(24*60*60);   // y los sacamos por pantalla 
	  printf("\n\tDias para terminar la tarea: %li\n", dias); 
	}
      }
      if(strcmp(p_lista->dependencias, "-\n") != 0){    //Vemos si tiene dependencias
	printf("\n\tTareas de la que depende la tarea:\n\t\tDirectamente:");
	int *dep_dir=ar_dependencias_dir(p_lista->dependencias); // Creamos el array de enteros de las dependencias directas y los sacamos por pantalla 
	int i=0;                                                
	while (*(dep_dir+i) != 0) {                                
	  printf(" %i", *(dep_dir+i));                           
	  i++;
	}
	printf("\n\t\tIndirectamente:");
	int *dep_indir_rep=ar_dependencias_indir(dep_dir, aux); // Determinamos las dependencias indirectas con repeticiones
	if(*dep_indir_rep == 0) printf(" -\n"); // Imprimimos un guion en caso de que no dependa indirectamente de ninguna tarea
	else{
	  i=0;
	  while(*(dep_indir_rep+i) != 0) i++;
	  int *dep_indir=quitar_repetidos(dep_indir_rep, i); // Cambiamos los identificadores repetidos por -1
	  i=0;
	  while(*(dep_indir+i) != 0){
	    if(*(dep_indir+i) > 0 && *(dep_indir+i) != p_lista->identificador)
 // Sacamos por pantalla  aquellas que son positivas(de ahi que introdujesemos -1 en los repetidos)  y que no sean la misma que el identificador
	      printf(" %i", *(dep_indir+i));
	    i++;
	  }
	  free(dep_indir);
	}
	free(dep_dir);
	free(dep_indir_rep);
      }
      else{
	printf("\n\tTareas de la que depende la tarea:\n\t\tDirectamente: -\n\t\tIndirectamente: -\n");
 // Imprimimos guiones en caso de que no dependa de ninguna tarea
      }
      printf ("\n\tTareas dependientes directamente de la tarea:");
      int *dep;
      int k=0;
      dep=dep_inv(p_lista->identificador, aux); // Determinamos  las tareas de las que depende
      if(*dep == 0) printf(" -"); // Si no hay imprimo un guion
      else{
	while(*(dep +k) !=  0){
	  printf(" %i", *(dep+k)); // Imprimo los identificadores de estas tareas
	  k++;
	}
      }
      free(dep);
      flag=1; // Marcamos que se ha encontrado la tarea
      break;
    }
    p_lista=p_lista->siguiente;
  }
  if(flag == 0){
    printf("\n\vAVISO: Tarea no existente.\n\vPulsar ENTER para volver al menu principal del programa."); // Aviso de que la tarea no existe
    while(getchar() != '\n');
    return 0;
  }
  printf("\n\vPulsar ENTER para volver al menu principal del programa.");
  while(getchar()!='\n');
  return 1;
}


int listar_tareas(Tarea *inicio, char *usuario){    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//
  system("clear");
  printf("\t\t\tLISTAR TAREAS\n\v");
  if(inicio == NULL){
    printf("\n\vAVISO: Lista de tareas vacia.\n\vPulsar ENTER para volver al menu principal del programa.");
    while(getchar() != '\n');
    return 0;
  }
  printf("\n\t1) Pendientes\n\t2) Terminadas\n\t3) Todas\n\t0) Cancelar\n\v\t¿Tareas a presentar? ");
  char tipo;
  do{
    scanf(" %s", &tipo);
    while(getchar() != '\n');
    if(tipo == '0') return 1; // Si elegimos cancelar vuelve al menu principal 
    if(tipo != '1' && tipo != '2' && tipo != '3') printf("\n\v\tELECCION INCORRECTA, PULSAR 1,2,3 o 0: "); 
// Si se pulsa una opcion invalida se muestra este mensaje
    else break; // Rompe el bucle si se pulsa una de las opciones validas
  }
  while(tipo != '1' && tipo != '2' && tipo != '3' && tipo != '0'); // Repito el bucle mientras no se seleccione una de las opciones validas

  char modo;
  system("clear");
  printf("\t\t\tLISTAR TAREAS\n\v");
  printf("\n\t1) Por pantalla\n\t2) A fichero\n\t0) Cancelar\n\v\t¿Presentacion de resultado? ");
  do{
    scanf(" %c", &modo);
    while(getchar() != '\n');
    if(modo == '0') return 1; // Retornamos al menu principal si escogemos cancelar
    if(modo != '1' && modo != '2') printf("\n\v\tELECCION INCORRECTA, PULSAR 1,2 o 0: "); // Mostramos  el mensaje si se elige una opcion no valida
    else break; // Rompo el bucle cuando se pulse una opcion valida
  }
  while(modo != '1' && modo != '2' && modo != '0'); // Repito el bucle mientras no se seleccione una de las opciones validas
  Tarea *sel_inicio;
  switch(tipo){
  case '1':{
    sel_inicio=seleccionar_tareas(inicio, usuario, "pendiente\n"); // Selecciono las tareas pendientes del usuario
    break;
  }
  case '2':{
    sel_inicio=seleccionar_tareas(inicio, usuario, "terminada\n"); // Selecciono las tareas terminadas del usuario
    break;
  }
  case '3':{
    sel_inicio=seleccionar_tareas(inicio, usuario, "todas\n"); // Selecciono todas las tareas del usuario
    break;
  }
  }
  if(sel_inicio == NULL){
    printf("\n\vAVISO: El usuario no tiene tareas de ese tipo o no tiene tareas.\n\v");
 // Aviso de que no se encontraron tareas con las especificaciones indicadas anteriormente
    printf("Pulsar ENTER para volver al menu principal del programa.");
    while(getchar() != '\n');
    return 0;
  }

  if(modo == '1'){
    system("clear");
    printf("\t\t\tLISTAR TAREAS\n\v");
    imprimir_tareas(sel_inicio, stdout); // Sacamos por pantalla las tareas seleccionadas
  }
  if(modo == '2'){
    printf("\n\v\tIndicar el identificador del fichero(nombre con extension .txt): ");
    char salida[20];
    fgets(salida, 20, stdin);
    salida[strlen(salida)-1] = '\0';
    strcpy(salida, strtolower(salida)); // Convertimos el nombre del fichero en minusculas
    if(salida[strlen(salida)-1] != 't' || salida[strlen(salida)-2] != 'x' || salida[strlen(salida)-3] != 't' || salida[strlen(salida)-4] != '.'){
 // Comprobamos si la extension es correcta
      liberar_lista(sel_inicio);
      printf("\n\vAVISO: Extension de fichero de salida no valida.\n\vPulsar ENTER para volver al menu principal.");
      while(getchar() != '\n');
      return 0;
    }
    FILE *fic=fopen(salida, "w");
    if(fic == NULL) error_abrir(salida);
    imprimir_tareas(sel_inicio, fic); // Imprimimos las tareas seleccionadas en el fichero creado
    if(fclose(fic) != 0) error_cerrar(salida);
  }
  liberar_lista(sel_inicio);
  printf("\nPulsar ENTER para volver al menu principal.");
  while(getchar() != '\n');
  return 1;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//

int salir(Tarea *orig, int flag, int *iden){ // Le mandamos los punteros de las listas de structs y el flag para saber si se ha creado una nueva tarea  
  system("clear");
  if(flag==0) exit(EXIT_SUCCESS); 
  char decision;
  printf("\t\t\tSALIR DEL PROGRAMA\n\v\t1)   Guardar cambios\n\t2)   Descartar cambios\n\t0)   Cancelar\n\v\t¿Opcion? "); 
  do{
    scanf(" %c", &decision);
    while(getchar() != '\n');
    if(decision != '1' && decision != '2' && decision != '0') printf("\n\tELECCION INCORRECTA, PULSAR 1,2 o 0: ");
  }
  while(decision != '1' && decision != '2' && decision != '0'); // Repito el bucle mientras que nos se pulse una opcion disponible
  switch(decision) {
  case '1':{ 
    FILE *fic;
    fic=fopen("tareas.txt","w");
    if(fic == NULL) error_abrir("tareas.txt");
    imprimir_tareas(orig, fic); // Sobreescribo las tareas
    if(fclose(fic) != 0) error_cerrar("tareas.txt");
    free(iden);
    liberar_lista(orig);
    printf("\nCambios guardados con exito.\n\vPulsar ENTER para salir del programa.");
    while(getchar() != '\n');
    system("clear");
    exit(EXIT_SUCCESS);
  }
  case '2':{
    free(iden);
    liberar_lista(orig);
    printf("\nCambios descartados con exito.\n\vPulsar ENTER para salir del programa.");
    while(getchar() != '\n');
    system("clear");
    exit(EXIT_SUCCESS);
  }
  case '0': 
    break;   
  }
  return 1;
}

