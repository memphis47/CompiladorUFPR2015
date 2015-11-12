
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

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES 
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO

%%

programa    :{ 
                num_vars=0;
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
              }

              comando_composto 
              ;




parte_declara_vars:  var 
;


var         : { } VAR declara_vars
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
                char *param1 = (char *) malloc (4 * sizeof(char));
                sprintf(param1, "%d", num_vars);
                num_vars=0;
                geraCodigo (NULL, "AMEM",param1,NULL,NULL); 
              }
              PONTO_E_VIRGULA
;

tipo        : IDENT
;

lista_id_var: lista_id_var VIRGULA IDENT 
              { /* insere última vars na tabela de símbolos */ 
                if(tbs->topo_pilha == NULL){
                  item auxItem = (item *) malloc (sizeof(item));
                  tbs->topo_pilha->itemProx = auxItem;
                  auxItem->itemAnt = tbs->topo_pilha;
                  auxItem->itemProx = NULL;
                  tbs->topo_pilha = auxItem;
                }
                else
                  tbs->itens = (item *) realloc (tbs->itens,sizeof(item));
                tbs->itens[tbs->n_itens].identificador = (char *) malloc (256 * sizeof(char));
                tbs->itens[tbs->n_itens].categoria = (char *) malloc (256 * sizeof(char));
                printf("\n\nToken: %s\n\n",token);
                strcpy(tbs->itens[tbs->n_itens].identificador,token);
                strcpy(tbs->itens[tbs->n_itens].categoria,"Variavel Simples");
                tbs->itens[tbs->n_itens].nivel_lexico = 0;
                tbs->itens[tbs->n_itens].deslocamento = num_vars;
                tbs->n_itens = tbs->n_itens++;
                num_vars++;
              }
            | IDENT { /* insere vars na tabela de símbolos */
                if(tbs->itens == NULL){
                  tbs->itens = (item *) malloc (sizeof(item));
                }
                else
                  tbs->itens = (item *) realloc (tbs->itens,sizeof(item));
                tbs->itens[tbs->n_itens].identificador = (char *) malloc (256 * sizeof(char));
                tbs->itens[tbs->n_itens].categoria = (char *) malloc (256 * sizeof(char));
                printf("\n\nToken: %s\n\n",token);
                strcpy(tbs->itens[tbs->n_itens].identificador,token);
                strcpy(tbs->itens[tbs->n_itens].categoria,"Variavel Simples");
                tbs->itens[tbs->n_itens].nivel_lexico = 0;
                tbs->itens[tbs->n_itens].deslocamento = num_vars;
                tbs->n_itens = tbs->n_itens++;
                num_vars++;
              }
;

lista_idents: lista_idents VIRGULA IDENT  
            | IDENT
;


comando_composto: T_BEGIN comandos T_END 

comandos:    
;


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

