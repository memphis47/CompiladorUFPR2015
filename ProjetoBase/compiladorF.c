
/* -------------------------------------------------------------------
 *            Aquivo: compilador.c
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [15/03/2012, 08h:22m]
 *
 * -------------------------------------------------------------------
 *
 * Funções auxiliares ao compilador
 *
 * ------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"


/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

FILE* fp=NULL;
void geraCodigo (char* rot, char* comando,char* param1,char* param2,char* param3) {

  if (fp == NULL) {
    fp = fopen ("MEPA", "w");
  }

  printf("Comando Traduzido: %s\n",comando);

  if ( rot == NULL ) {
    if(param1 == NULL){
      fprintf(fp, "    %s\n", comando); fflush(fp);
    }
    else{
      if(param2 == NULL){
        fprintf(fp, "    %s %s\n", comando,param1); fflush(fp);
      }
      else{
        if(param3 == NULL){
          fprintf(fp, "    %s %s,%s\n", comando,param1,param2); fflush(fp);
        }
        else{
          fprintf(fp, "    %s %s,%s,%s\n", comando,param1,param2,param3); fflush(fp);
        }
      }
    }

  } else {
    if(param1 == NULL){
      fprintf(fp, "%s: %s \n", rot, comando); fflush(fp);
    }
    else{
      if(param2 == NULL){
         fprintf(fp, "%s: %s %s\n", rot, comando,param1); fflush(fp);
      }
      else{
        if(param3 == NULL){
           fprintf(fp, "%s: %s %s,%s\n", rot, comando,param1,param2); fflush(fp);
        }
        else{
          fprintf(fp, "%s: %s %s,%s,%s\n", rot, comando,param1,param2,param3); fflush(fp);
        }
      }
    }
  }
}

int imprimeErro ( char* erro ) {
  fprintf (stderr, "Erro na linha %d - %s\n", nl, erro);
  exit(-1);
}
