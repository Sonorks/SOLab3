#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "parser.h"

#define TAM 100

int main ()
{
  char ** items;
  int i, num, background;
  char expresion[TAM];
  int salir = 0;

  while(salir==0){

  printf ("teclea una orden$ ");
  fgets (expresion, TAM, stdin);

  num = separaItems (expresion, &items, &background);

  if(strcmp(items[0],"mydir")==0){
	printf("La orden fue %s.\n", items[0]);
	}
  else if(strcmp(items[0],"mypwd")==0){
	printf("La orden fue %s.\n", items[0]);
	char cwd[1024];
	if(getcwd(cwd,sizeof(cwd)) != NULL){
		fprintf(stdout, "%s\n",cwd);
	}
	else
		perror("getcwd() error\n");
	}
  else if(strcmp(items[0],"mycd")==0){
	printf("La orden fue %s.\n", items[0]);
	}
  else if(strcmp(items[0],"mycp")==0){
	printf("La orden fue %s.\n", items[0]);
	}
  else if(strcmp(items[0],"myexit")==0){
	printf("La orden fue %s.\n", items[0]);
	salir=1;
	}
  else if(strcmp(items[0],"myecho")==0){
	printf("%s.\n", items[1]);
	}
  else if(strcmp(items[0],"myclr")==0){
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	}
  else if(strcmp(items[0],"mykill")==0){
	printf("La orden fue %s.\n", items[0]);
	}
  else if(strcmp(items[0],"mytime")==0){
  	struct tm* ptm;
  	struct timeval tv;
  	char tiempoString[40];
  	int tiempo=0;
	int time=gettimeofday(&tv,NULL);
	if(time==0){
		ptm= localtime(&tv.tv_sec);
		strftime (tiempoString,sizeof(tiempoString), "%Y-%m-%d %H:%M:%S",ptm);
		printf("%s\n",tiempoString);
	}
	}
  else if(strcmp(items[0],"mypause")==0){
	printf("La orden fue %s.\n", items[0]);
	}

  }
  return 0;
}
