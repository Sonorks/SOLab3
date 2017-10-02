#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "parser.h"
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <signal.h>
#include <syscall.h>
#include <sys/types.h>


#define TAM 100


static volatile sig_atomic_t done_waiting = 0;

static void handler()
{
  printf("Para salir escriba myexit.\n");
  fprintf(stdout, "User@MyCli:# ");
}
static void handlerPause()
{
  done_waiting=1;
}

void mydir(char* directorio, int n);
void mypwd(int opt);
void mytime();
void mypause();
void mycd(char** items, int n);
void mycp(char** items);

int main ()
{
  signal(SIGINT, handler);
  char ** items;
  int num, background;
  char expresion[TAM];
  int salir = 0;

  while(salir==0){
  mypwd(2);
  fgets (expresion, TAM, stdin);

  num = separaItems (expresion, &items, &background);

  if(strcmp(items[0],"mydir")==0){
  mydir(items[1],num);
  }
  else if(strcmp(items[0],"mypwd")==0){
  mypwd(1);
  }
  else if(strcmp(items[0],"mycd")==0){//listo
    if(background==1){
      pid_t hijo = fork();
      if(hijo==0){
        mycd(items,num);
      }
    }
    else
      mycd(items,num);
  }
  else if(strcmp(items[0],"mycp")==0){//listo
    if(background==1){
      pid_t hijo = fork();
      if(hijo==0){
        mycp(items);
      }
    }
    else
      mycp(items);
  }
  else if(strcmp(items[0],"myexit")==0){
  printf("Saliendo del CLI.\n");
  salir=1;
  }
  else if(strcmp(items[0],"myecho")==0){
  for (int i = 1; i < num; i++) {
       printf("%s ", items[i]);
      }
    printf("\n");
  }
  else if(strcmp(items[0],"myclr")==0){
  printf("\e[1H\e[2J");
  }
  else if(strcmp(items[0],"mykill")==0){
    /*printf("%s %s\n",items[1],items[2]);
    int status;
    pid_t returnKillValue;
    pid_t pidFork = fork();
    returnKillValue = kill(items[2], items[1]);
    if(returnKillValue){
            printf("No se pudo matar el proceso.\n");
            waitpid(items[2], &status, 0);
        } else {
            printf("Proceso eliminado.\n");
        }
  //kill(items[2],items[1]);*/
    if(strcmp(items[1],"SIGKILL")==0){
      pid_t i = fork();
      if( i == 0){
        char* sh = "/home/lis/Desktop/SOJulian/SOLab3/mykill";
        char* args[2];
        args[0]=items[2];
        args[1]=NULL;
        printf("Hola desde el hijo.\n");
        execv(sh, args);
        //_exit(1);
        /*char comando[100]="sh mykill.sh ";
        strcat(comando,items[2]);
        printf("%s",comando);
        system(comando);*/
      }
    }
  }
  else if(strcmp(items[0],"mytime")==0){
    mytime();
  }
  else if(strcmp(items[0],"mypause")==0){
    if(background==1){
      pid_t hijo = fork();
      if(hijo==0){
        mypause();
      }
    }
    else
      mypause();
  }
  else
    printf("bash: %s: No se encontró la orden\n",items[0] );

  }
  return 0;
}

void mytime(){
  struct tm* ptm;
    struct timeval tv;
    char tiempoString[40];
  int time=gettimeofday(&tv,NULL);
  if(time==0){
    ptm= localtime(&tv.tv_sec);
    strftime (tiempoString,sizeof(tiempoString), "%Y-%m-%d %H:%M:%S",ptm);
    printf("%s\n",tiempoString);
  }
}

void mypwd(int opt){
  char cwd[1024];

  if(getcwd(cwd,sizeof(cwd)) != NULL){
    if(opt == 1){
      fprintf(stdout, "%s\n",cwd);
    }
    else if (opt == 2){
      fprintf(stdout, "\033[33mUser@MyCli\033[0m:\033[32m%s\033[0m# ",cwd);
    }
  }
  else
    perror("getcwd() error\n");
}


void mydir(char* directorio,int n) {
  DIR *dire;
  struct dirent *ent;
  if (n!=1) dire = opendir (directorio);
  else dire = opendir (".");

  if (dire == NULL)
    perror("No se puede abrir o no existe ese directorio");

  FILE *archivo;
  while ((ent = readdir (dire)) != NULL)
    {
      if ( (strcmp((*ent).d_name, ".")!=0) && (strcmp((*ent).d_name, "..")!=0) ){
      archivo=fopen((*ent).d_name, "r");
      if (archivo){
          fseek(archivo, 0L, SEEK_END);//Saber bien el funcionamiento de fseek()
          fclose(archivo);
          printf ("%s\n", (*ent).d_name);
        }
      }
    }
  closedir (dire);
}
void mypause(){
  signal(SIGTSTP, handlerPause);
  done_waiting = 0;
  while ( !done_waiting )
    ;
}

void mycd(char** items, int n) {
  if (n<=2) {
    if (chdir(items[1])!=0)
      printf("Error. No existe o no es posible cambiar al directorio \n");
  }
  else
    printf("Error. Ingrese una ruta adecuada.\n");
}

void mycp(char** items){
  FILE *arch1 , *arch2;
   int datos =0;
   arch1 = fopen(items[1], "r");
   arch2 = fopen(items[2], "w");
   while ( (datos = fgetc (arch1)) != EOF ) {
       fputc(datos, arch2 );
   }
   fclose(arch1);
   fclose(arch2);
}