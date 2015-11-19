
// Testar se funciona corretamente o empilhamento de parâmetros
// passados por valor ou por referência.


%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"

int num_vars;
int desl;
typedef struct item item;

struct item
{
  char *identificador;
  char *categoria;
  int  nivel_lexico;
  int  deslocamento;
  char *passagem;
  char *rotulo;
  char *tipo; // mudar depois para enum
  item *itemAnt;
  item *itemProx;
};

typedef struct tabela_simbolos
{
  int n_itens;
  item *topo_pilha;
  item *fim_pilha;
  item *itens;

} tabela_simbolos;

tabela_simbolos *tbs;
char *param1;
char *param2;
char *param3;
char *param1Aux;
char *param2Aux;
char *param3Aux;
char *ident;
int desl;

item * procura_tbsimb(char * token){
  item *itemAtual = tbs->topo_pilha;
  while(itemAtual != NULL){
    if(strcmp (token, itemAtual->identificador) == 0)
      return itemAtual;
    itemAtual = itemAtual->itemAnt;
  }
  return NULL;
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

expr       :ABRE_PARENTESES expr FECHA_PARENTESES|
            expr SOMA termo {
              geraCodigo (NULL, "SOMA",NULL,NULL,NULL); 
              printf ("+\n"); }
            |expr SUB termo {
              geraCodigo (NULL, "SUBT",NULL,NULL,NULL); 
              printf ("-"); } 
            |termo
;

termo      : ABRE_PARENTESES expr FECHA_PARENTESES|
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
              printf("OLA1\n");
            }
;

if_then     : IF expressao 
            {
             printf("OLA1\n");
            }
             THEN comandos
            {
             printf("OLA2\n");
            }
            |IF expressao 
            {
             printf("OLA3\n");
            }
             THEN T_BEGIN comandos T_END
            {
              printf("OLA4\n");
            }
;

cond_else   : ELSE comandos | ELSE T_BEGIN comandos T_END
            | %prec LOWER_THAN_ELSE
;

expressao   : ABRE_PARENTESES prior2 FECHA_PARENTESES

prior2  :     prior2 IGUAL prior1  |
              prior2 DIFE  prior1 |
              prior1

prior1  :     prior1 MAIOR final |
              prior1 MENOR final |
              prior1 MAEG  final | 
              prior1 MEEG  final |
              final

final   :     IDENT |
              NUMERO

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
   tbs->topo_pilha = NULL;
   tbs->fim_pilha = NULL;
   tbs->itens = NULL;
   tbs->n_itens = 0;
   yyin=fp;
   yyparse();

   return 0;
}

