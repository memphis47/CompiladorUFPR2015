
// Testar se funciona corretamente o empilhamento de parâmetros
// passados por valor ou por referência.


%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "tabela.h"

int num_vars;
int desl;
typedef struct itemLista itemLista;


struct itemLista
{
  char *identificador;
  itemLista *itemAnt;
  itemLista *itemProx;
};

typedef struct lista_rotulos
{
  int n_rotulos;
  itemLista *fim;
  itemLista *inicio;
} lista_rotulos;



tabela_simbolos *tbs;
lista_rotulos *lr;
char *param1;
char *param2;
char *param3;
char *param1Aux;
char *param2Aux;
char *param3Aux;
char *ident;
int desl;
int rotNumber;

item * procura_tbsimb(char * token){
  item *itemAtual = tbs->topo_pilha;
  while(itemAtual != NULL){
    if(strcmp (token, itemAtual->identificador) == 0)
      return itemAtual;
    itemAtual = itemAtual->itemAnt;
  }
  return NULL;
}


void gera_comando_crvl(char * token){
  item *item = procura_tbsimb(token);
  if(item!=NULL){
    free(param1);
    free(param2);
    free(param3);
    param1 = (char *) malloc (4 * sizeof(char));
    param2 = (char *) malloc (4 * sizeof(char));
    sprintf(param1, "%d", item->nivel_lexico);
    sprintf(param2, "%d", item->deslocamento);
    geraCodigo (NULL, "CRVL",param1,param2,NULL); 
  } 
}

void adiciona_item_lista(){
  itemLista *auxItem = (itemLista *) malloc (sizeof(itemLista));

  auxItem->itemAnt = lr->fim;
  auxItem->itemProx = NULL;
  char rtn[4];
  sprintf(rtn, "%d", rotNumber);
  char rot[]="R";
  strcat(rot, rtn);
  auxItem->identificador = (char *) malloc (256 * sizeof(char));
  strcpy(auxItem->identificador,rot);
  lr->n_rotulos = lr->n_rotulos++;
  if(lr->inicio == NULL){
    lr->inicio = auxItem; 
    lr->fim = auxItem;
  }
  else{
    lr->fim= auxItem;
  }
  rotNumber++;
}


%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES WRITE
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO
%token NUMERO SOMA SUB MUL DIV
%token IF THEN ELSE
%token DIFE IGUAL MAEG MAIOR MENOR MEEG

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

programa    :{ 
                num_vars=0;
                desl=0;
                geraCodigo (NULL, "INPP",NULL,NULL,NULL); 
             }
             PROGRAM IDENT 
             ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
             bloco PONTO {
              geraCodigo (NULL, "PARA",NULL,NULL,NULL); 
             }
;

bloco       : 
              parte_declara_vars
              { 
                free(param1);
                free(param2);
                free(param3);
                param1 = (char *) malloc (4 * sizeof(char));
                sprintf(param1, "%d", num_vars);
                num_vars=0;
                geraCodigo (NULL, "AMEM",param1,NULL,NULL); 
              }

              comando_composto 
              ;




parte_declara_vars:  var 
;


var         : { } VAR declara_vars {
                }
            |
;

declara_vars:
              declara_vars declara_var |
              declara_var 
;

declara_var : { } 
              lista_id_var DOIS_PONTOS 
              tipo 
              { /* AMEM */
               
              }
              PONTO_E_VIRGULA
;

tipo        : IDENT
;

lista_id_var: lista_id_var VIRGULA IDENT 
              { /* insere última vars na tabela de símbolos */ 
                item *auxItem = (item *) malloc (sizeof(item));
                auxItem->itemAnt = tbs->topo_pilha;
                auxItem->itemProx = NULL;
                

                auxItem->identificador = (char *) malloc (256 * sizeof(char));
                auxItem->categoria = (char *) malloc (256 * sizeof(char));
                strcpy(auxItem->identificador,token);
                strcpy(auxItem->categoria,"Variavel Simples");
                auxItem->nivel_lexico = 0;
                auxItem->deslocamento = desl;
                tbs->n_itens = tbs->n_itens++;
                tbs->topo_pilha = auxItem;
                num_vars++;
                desl ++;
              }
            | IDENT { /* insere vars na tabela de símbolos */
                item *auxItem = (item *) malloc (sizeof(item));
                auxItem->itemAnt = tbs->topo_pilha;
                auxItem->itemProx = NULL;
                

                auxItem->identificador = (char *) malloc (256 * sizeof(char));
                auxItem->categoria = (char *) malloc (256 * sizeof(char));
                strcpy(auxItem->identificador,token);
                strcpy(auxItem->categoria,"Variavel Simples");
                auxItem->nivel_lexico = 0;
                auxItem->deslocamento = desl;
                tbs->n_itens = tbs->n_itens++;
                tbs->topo_pilha = auxItem;
                num_vars++;
                desl ++;
              }
;

lista_idents: lista_idents VIRGULA IDENT  
            | IDENT
;

// Comandos a serem executados.

comando_composto: T_BEGIN comandos_total T_END 


comandos_total: comandos //ver empty

comandos : 
           comandos comando | comando

comando:
          IDENT{
            item *itema = procura_tbsimb(token);
            if(itema!=NULL){
              free(param1Aux);
              free(param2Aux);
              free(param3Aux);
              param1Aux = (char *) malloc (4 * sizeof(char));
              param2Aux = (char *) malloc (4 * sizeof(char));
              sprintf(param1Aux, "%d", itema->nivel_lexico);
              sprintf(param2Aux, "%d", itema->deslocamento);
            }
            //TODO: caso não encontrar mostrar msg de erro.
          } ATRIBUICAO expr PONTO_E_VIRGULA{
            geraCodigo (NULL, "ARMZ",param1Aux,param2Aux,NULL);
          }
          | comando_write
          | cond_if

;

expr       ://ABRE_PARENTESES expr FECHA_PARENTESES|
            expr SOMA termo {
              geraCodigo (NULL, "SOMA",NULL,NULL,NULL); 
              printf ("+\n"); }
            |expr SUB termo {
              geraCodigo (NULL, "SUBT",NULL,NULL,NULL); 
              printf ("-"); } 
            |termo
;

termo      : //ABRE_PARENTESES expr FECHA_PARENTESES|
             termo MUL fator  {
              geraCodigo (NULL, "MULT",NULL,NULL,NULL); 
              printf ("*"); }| 
             termo DIV fator  {
              geraCodigo (NULL, "DIVI",NULL,NULL,NULL); 
              printf ("/"); }|
             fator
;

fator      :  ABRE_PARENTESES expr FECHA_PARENTESES|
              IDENT {
                gera_comando_crvl(token);
              }
              |NUMERO{
                geraCodigo (NULL, "CRCT",token,NULL,NULL); 
              }
;

comando_write : WRITE ABRE_PARENTESES IDENT {
                  item *item = procura_tbsimb(token);
                  if(item!=NULL){
                    free(param1);
                    free(param2);
                    free(param3);
                    param1 = (char *) malloc (4 * sizeof(char));
                    param2 = (char *) malloc (4 * sizeof(char));
                    sprintf(param1, "%d", item->nivel_lexico);
                    sprintf(param2, "%d", item->deslocamento);
                    geraCodigo (NULL, "CRVL",param1,param2,NULL);
                    geraCodigo (NULL, "IMPR",NULL,NULL,NULL);
                  } 
                } FECHA_PARENTESES PONTO_E_VIRGULA |
                WRITE ABRE_PARENTESES NUMERO FECHA_PARENTESES PONTO_E_VIRGULA 


cond_if     : if_then cond_else 
            { 

            }
;

if_then     : IF expressao {
                adiciona_item_lista();
                geraCodigo (NULL, "DSVF",rot,NULL,NULL);
              }
             THEN internal_if{
                adiciona_item_lista();
                geraCodigo (NULL, "DSVS",rot,NULL,NULL);
             }
;

cond_else   : ELSE{
                geraCodigo (lr->topo_pilha->identificador, "NADA",NULL,NULL);
                lr->topo_pilha=lr->topo_pilha->itemAnt
              } internal_if {
                
                geraCodigo (NULL, "DSVS",rot,NULL,NULL);}
            | %prec LOWER_THAN_ELSE
;

internal_if: comandos | T_BEGIN comandos T_END

expressao   : ABRE_PARENTESES prior2 FECHA_PARENTESES

prior2  :     prior2 IGUAL prior1 {geraCodigo (NULL, "CMIG",NULL,NULL,NULL);}|
              prior2 DIFE  prior1 {geraCodigo (NULL, "CMDG",NULL,NULL,NULL);}|
              prior1

prior1  :     prior1 MAIOR final {geraCodigo (NULL, "CMMA",NULL,NULL,NULL);}|
              prior1 MENOR final {geraCodigo (NULL, "CMME",NULL,NULL,NULL);}|
              prior1 MAEG  final {geraCodigo (NULL, "CMAG",NULL,NULL,NULL);}| 
              prior1 MEEG  final {geraCodigo (NULL, "CMEG",NULL,NULL,NULL);}|
              final

final   :     IDENT{gera_comando_crvl(token);} |
              NUMERO{geraCodigo (NULL, "CRCT",token,NULL,NULL);}

%%

main (int argc, char** argv) {
   FILE* fp;
   extern FILE* yyin;

   if (argc<2 || argc>2) {
         printf("usage compilador <arq>a %d\n", argc);
         return(-1);
      }

   fp=fopen (argv[1], "r");
   if (fp == NULL) {
      printf("usage compilador <arq>b\n");
      return(-1);
   }


/* -------------------------------------------------------------------
 *  Inicia a Tabela de Símbolos
 * ------------------------------------------------------------------- */
   tbs = (tabela_simbolos *) malloc (sizeof(tabela_simbolos));
   lr = (lista_rotulos *) malloc (sizeof(lista_rotulos));
   tbs->topo_pilha = NULL;
   lr->inicio = NULL;

   tbs->fim_pilha = NULL;
   lr->fim = NULL;

   tbs->itens = NULL;

   tbs->n_itens = 0;
   lr->n_rotulos = 0;

   yyin=fp;
   yyparse();

   return 0;
}

