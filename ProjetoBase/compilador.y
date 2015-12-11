
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
lista_params *lp;
lista_params *temp;
char *param1;
char *param2;
char *param3;
char *param1Aux;
char *param2Aux;
char *param3Aux;
char *ident;
char* rots;
char* paramName;
int desl;
int rotNumber;
int nivel;
int nivel_lexico_atual;
int n_param_atual;
int param_chamada = 0;
int do_verify = 0;
int no_proc=0;
tipo_parametro passado;
tipo_variavel  compItem = -1;

item *procedure;
item *tempItemArmz;
item *tempFunc;
categorias tempCategoria;


int yyerror (char *msg) {
  printf("%s", msg);
  exit(1);
}

tipo_variavel retornaTipo(){
  if (strcmp(token, "integer") == 0) 
    return INTEGER;
    
  else if (strcmp(token, "boolean") == 0)
    return BOOLEAN;
    
  else if (strcmp(token, "char") == 0)
    return CHAR;
    
  else /* default: */
    yyerror("Tipo não definido");
}

item * procura_tbsimb(char * token, categorias cat){
  item *itemAtual = tbs->topo_pilha;
  while(itemAtual != NULL){
    if(strcmp (token, itemAtual->identificador) == 0 
    && itemAtual->categoria == cat){
      return itemAtual;
    }
    itemAtual = itemAtual->itemAnt;
  }
  return NULL;
}


int gera_comando_crvl(categorias cat){
  item *item = procura_tbsimb(token,cat);
  if(item!=NULL){
    free(param1);
    free(param2);
    free(param3);
    param1 = (char *) malloc (4 * sizeof(char));
    param2 = (char *) malloc (4 * sizeof(char));
    sprintf(param1, "%d", item->nivel_lexico);
    sprintf(param2, "%d", item->deslocamento);
    if(param_chamada && temp->inicio->passagem == REFERENCIA){ //chamada de parametro passado por referencia
      if(item->tipo == temp->inicio->tipo){
        geraCodigo (NULL, "CREN",param1,param2,NULL);
      }
      else{
        printf("ERRO: Tipo passado: %d\n",item->tipo);
        printf("Tipo esperado %d\n",temp->inicio->tipo);
        yyerror("");
      }
    }
    else if(param_chamada==0 && cat==PF && item->passagem == REFERENCIA){
      geraCodigo (NULL, "CRVI",param1,param2,NULL); 
    }
    else{
      geraCodigo (NULL, "CRVL",param1,param2,NULL); 
    }
    return 1;
  } 
  return 0;
  
}

void adiciona_item_lista(){
  itemLista *auxItem = (itemLista *) malloc (sizeof(itemLista));
  
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
    lr->fim->itemProx = auxItem;
    auxItem->itemAnt = lr->fim;
    lr->fim = auxItem;
  }
  rotNumber++;
}

void remove_item_lista(char *token){
  itemLista *itemAtual = lr->fim;
  while(itemAtual != NULL){
    if(strcmp (token, itemAtual->identificador) == 0){
      if(itemAtual->itemAnt!=NULL)
        itemAtual->itemAnt->itemProx = itemAtual->itemProx;
      else{
        lr->inicio = itemAtual->itemProx;
      }
      if(itemAtual->itemProx!=NULL){
        itemAtual->itemProx->itemAnt = itemAtual->itemAnt;
      }
      else{
        lr->fim = itemAtual->itemAnt;
      }
    }
    itemAtual = itemAtual->itemAnt;
  }
}

int remove_ts(){
  item *itemAtual = tbs->topo_pilha;
  int count = 0;
  while(itemAtual!= NULL && itemAtual->categoria == VS 
  && itemAtual->nivel_lexico == nivel_lexico_atual){
    item *aux = itemAtual;
    itemAtual = itemAtual->itemAnt;
    free(aux);
    count++;
  }
  tbs->topo_pilha = itemAtual;
  return count;
}

void adiciona_ts(categorias cat,char *rot){
  item *auxItem = (item *) malloc (sizeof(item));
  auxItem->itemAnt = tbs->topo_pilha;
  auxItem->itemProx = NULL;
  
  auxItem->tipo = -1;
  auxItem->identificador = (char *) malloc (256 * sizeof(char));
  strcpy(auxItem->identificador,token);
  auxItem->categoria = cat;
  auxItem->nivel_lexico = nivel_lexico_atual;
  auxItem->deslocamento = desl;
  auxItem->passagem = -1;
  if(rot!=NULL){
    auxItem->rotulo = (char *) malloc (256 * sizeof(char));
    strcpy(auxItem->rotulo,rot);
  }
  tbs->n_itens = tbs->n_itens++;
  if(cat == PROC || cat == FUN){
    procedure=auxItem;
  }
  tbs->topo_pilha = auxItem;
  desl ++;
}


void adiciona_param_ts(categorias cat,char *rot, tipo_parametro passagem){
  item *auxItem = (item *) malloc (sizeof(item));
  auxItem->itemAnt = tbs->topo_pilha;
  auxItem->itemProx = NULL;
  
  auxItem->identificador = (char *) malloc (256 * sizeof(char));
  strcpy(auxItem->identificador,paramName);
  auxItem->categoria = cat;
  auxItem->nivel_lexico = nivel_lexico_atual;
  if(rot!=NULL){
    auxItem->rotulo = (char *) malloc (256 * sizeof(char));
    strcpy(auxItem->rotulo,rot);
  }
  auxItem->tipo = retornaTipo();
  auxItem->passagem = passagem; 
  tbs->n_itens = tbs->n_itens++;
  tbs->topo_pilha = auxItem;
  if(lp->inicio==NULL){
    item *inicio=(item *) malloc(sizeof(item));
    memcpy(inicio,auxItem,sizeof(item));
    inicio->itemAnt=inicio;
    inicio->itemProx=inicio;
    lp->fim=inicio;
    lp->inicio=inicio;
  }
  else{
    item *fim=(item *) malloc(sizeof(item));
    memcpy(fim,auxItem,sizeof(item));
    fim->itemAnt=lp->fim;
    fim->itemProx=NULL;
    lp->fim->itemProx=fim;
    lp->fim = fim;
  }
  desl ++;
}


void adiciona_tipo_ts(int nvars){
  item *itemAtual = tbs->topo_pilha;
  tipo_variavel tipo= retornaTipo();
  while(itemAtual!= NULL && itemAtual->categoria == VS
        && itemAtual->tipo == -1){
    itemAtual->tipo = tipo;
    itemAtual = itemAtual->itemAnt;
  }
}



void adiciona_deslocamento_param(){
  item *itemAtual = tbs->topo_pilha;
  int i=-4;
  int param=0;
  while(itemAtual!= NULL && itemAtual->categoria != PROC && itemAtual->categoria != FUN){
    if(itemAtual->categoria==PF){
      itemAtual->deslocamento = i;
    }
    itemAtual = itemAtual->itemAnt;
    i--;
    param++;
  }
  if(itemAtual->categoria == FUN){
    procedure->deslocamento=i;
  }
  procedure->param->n_param=param;
}

/* yyerror()
  YYERROR
  */

int procura_compara(categorias cat){
  item *itemAtual=procura_tbsimb(token,cat);
  if(compItem!=-1 && itemAtual!=NULL){
    if(compItem != itemAtual->tipo){
      yyerror("\n\nOperação não permitida, valores com tipos diferentes\n\n");
    }
    return 1;
  }
  else if(itemAtual!=NULL){
    compItem = itemAtual->tipo;
    return 1;
  }
  else{
    return 0;
  }
}

void reza(){
  if(num_vars>0){
    free(param1);
    free(param2);
    free(param3);
    param1 = (char *) malloc (4 * sizeof(char));
    sprintf(param1, "%d", num_vars);
    num_vars=0;
    geraCodigo (NULL, "AMEM",param1,NULL,NULL); 
  }  
}

void doDmem(){
  num_vars=remove_ts();
  if(num_vars>0){
    param1 = (char *) malloc (4 * sizeof(char));
    sprintf(param1, "%d", num_vars);
    geraCodigo (NULL, "DMEM",param1,NULL,NULL);
  }
}

char * retornaRotulo(){
  char rtn[4];
  sprintf(rtn, "%d", rotNumber);
  char rot[]="R";
  strcat(rot, rtn);
  char *strRot = (char *) malloc (256 * sizeof(char));
  strcpy(strRot,rot);
  return strRot;
  
}

item* cria_valores_armz(categorias cat){
  item *item = procura_tbsimb(token,cat);
  if(item!=NULL){
    
    free(param1Aux);
    free(param2Aux);
    free(param3Aux);
    
    param1Aux = (char *) malloc (4 * sizeof(char));
    param2Aux = (char *) malloc (4 * sizeof(char));
    sprintf(param1Aux, "%d", item->nivel_lexico);
    sprintf(param2Aux, "%d", item->deslocamento);
  }
  else if(cat != PF){
    return cria_valores_armz(PF);
  }
  return item;
}

void carrega_valor_imprime(){
  
  geraCodigo (NULL, "IMPR",NULL,NULL,NULL);
}

void verifica_chama_proc(){
  if(procedure->param!=NULL &&  n_param_atual!=procedure->param->n_param){
    printf("ERRO: Número de parametros passados: %d\n",n_param_atual);
    printf("Número de parametros esperados: %d\n",procedure->param->n_param);
    yyerror("");
  }
  free(param2);
  param2 = (char *) malloc (4 * sizeof(char));
  sprintf(param2, "%d", nivel_lexico_atual);
  
  geraCodigo (NULL, "CHPR",procedure->rotulo,param2,NULL);
}

void define_else(){
  char rtn[4];
  sprintf(rtn, "%d", rotNumber);
  char rot[]="R";
  strcat(rot, rtn);
  char *strRot = (char *) malloc (256 * sizeof(char));
  strcpy(strRot,rot);
  
  geraCodigo (NULL, "DSVS",strRot,NULL,NULL);
  geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
  remove_item_lista(lr->fim->identificador);
  if(lr->fim->itemAnt != NULL)
    lr->fim=lr->fim->itemAnt;
  adiciona_item_lista();
}

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES WRITE
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO
%token NUMERO SOMA SUB MUL DIV
%token IF THEN ELSE
%token WHILE DO
%token DIFE IGUAL MAEG MAIOR MENOR MEEG
%token PROCEDURE
%token FUNCTION
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

programa    :{ 
                nivel_lexico_atual=0;
                num_vars=0;
                desl=0;
                nivel=0;
                geraCodigo (NULL, "INPP",NULL,NULL,NULL); 
             }
             PROGRAM IDENT{adiciona_ts(PROG,NULL);}
             ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
             bloco PONTO {
              geraCodigo (NULL, "PARA",NULL,NULL,NULL); 
             }
;

bloco       : 
              parte_declara_vars { 
                reza();
              }
              parse_bloco
;

parse_bloco:
            {
              adiciona_item_lista();
              geraCodigo (NULL, "DSVS",lr->fim->identificador,NULL,NULL);
            } 
            proc_func_def_loop
            {
              geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
            }
            
            comando_composto{ doDmem(); }
            |
            comando_composto{ doDmem(); }


;

proc_func_def_loop:
                  proc_func_def proc_func_def_loop |
;

proc_func_def:
              proc_func_def_choose proc_com
;

proc_func_def_choose:
              PROCEDURE{tempCategoria=PROC;}  | 
              FUNCTION{tempCategoria=FUN;}
;

proc_com: IDENT{
            nivel_lexico_atual++;
            free(rots);
            rots=(char *) malloc (5* sizeof(char));
            rots=retornaRotulo(); 
            adiciona_ts(tempCategoria,rots);
          } 
          parse_proc_decl PONTO_E_VIRGULA
          {
            char lexico[4];
            sprintf(lexico, "%d", nivel_lexico_atual);
            geraCodigo (rots, "ENPR",lexico,NULL,NULL); 
            adiciona_item_lista();
          } 
          
          blocoProc PONTO_E_VIRGULA
          {
            remove_item_lista(lr->fim->identificador);
            char lexico[4];
            sprintf(lexico, "%d", nivel_lexico_atual);
            geraCodigo (NULL, "RTPR",lexico,"0",NULL); 
            nivel_lexico_atual--;
          }
;

parse_proc_decl: ABRE_PARENTESES 
                 declara_param param_loop 
                 FECHA_PARENTESES {
                    procedure->param=(lista_params *) malloc(sizeof(lista_params));
                    memcpy(procedure->param,lp,sizeof(lista_params)); //Procedure definido com os par
                    adiciona_deslocamento_param();
                 }
                 func_type
;

func_type:
          DOIS_PONTOS IDENT{
            tipo_variavel tipo= retornaTipo();
            procedure->tipo = tipo;
            lp->inicio=NULL;
            lp->fim=NULL;
          } |
          {
            lp->inicio=NULL;
            lp->fim=NULL;
          }
;

param_loop: PONTO_E_VIRGULA declara_param param_loop | 
;

declara_param:
            VAR{passado=REFERENCIA;}
            parse_declaracao|
            {passado=VALOR;}
            parse_declaracao
;

parse_declaracao:
            IDENT{
              free(paramName);
              paramName = (char *) malloc (256 * sizeof(char));
              strcpy(paramName,token);
            }
            DOIS_PONTOS 
            IDENT{adiciona_param_ts(PF,NULL,passado);}
;

blocoProc: 
              {desl=0;}parte_declara_vars { reza(); } 
              parse_bloco|
              parse_bloco
;

parte_declara_vars:  var 
;


var         : VAR declara_vars
;

declara_vars:
              declara_vars declara_var |
              declara_var 
;

declara_var :  
              lista_id_var DOIS_PONTOS 
              IDENT 
              { 
                adiciona_tipo_ts(num_vars);
              }
              PONTO_E_VIRGULA
;

lista_id_var: lista_id_var VIRGULA IDENT 
              { /* insere última vars na tabela de símbolos */ 
               adiciona_ts(VS,NULL);
               num_vars++;
              }
            | IDENT { /* insere vars na tabela de símbolos */
                adiciona_ts(VS,NULL);
                num_vars++;
              }
;

lista_idents: lista_idents VIRGULA IDENT  
            | IDENT
;

// Comandos a serem executados.

comando_composto: T_BEGIN comando comando_composto_loop T_END ;

comando_composto_loop: PONTO_E_VIRGULA comando comando_composto_loop | PONTO_E_VIRGULA | 
; 

comando:  IDENT
          {
            procedure = procura_tbsimb(token,PROC);
            no_proc=0;
            if(procedure==NULL){
              procedure = procura_tbsimb(token,FUN);
              if(procedure == NULL){
                tempItemArmz = cria_valores_armz(VS);
              }
              no_proc=1;
            }
          }
          parse_comando
          | comando_write
          | cond_if
          | comando_repetitivo

;

parse_comando:
          
          ATRIBUICAO {
            if(procedure!=NULL && !no_proc){
              yyerror("Tipo de operação nao permitida");
            }
          }
          expressao_simples { 
            compItem = -1;
            if(tempItemArmz->passagem == REFERENCIA)
              geraCodigo (NULL, "ARMI",param1Aux,param2Aux,NULL);
            else{
              geraCodigo (NULL, "ARMZ",param1Aux,param2Aux,NULL);
            }
          }
          |
          procedure_function_call
;

procedure_function_call:
          {
            if(procedure==NULL){
                yyerror("Tipo de operação nao permitida");
            }
            if(procedure->categoria==FUN){
              param1Aux = (char *) malloc (4 * sizeof(char));
              sprintf(param1Aux, "%d", 1);
              geraCodigo (NULL, "AMEM",param1Aux,NULL);
            }
          }
          ABRE_PARENTESES{
            n_param_atual=0;
            param_chamada=1;
            temp=(lista_params *) malloc(sizeof(lista_params));
            memcpy(temp,procedure->param,sizeof(lista_params));
          } le_params FECHA_PARENTESES
          {
            free(temp);
            param_chamada=0;
            verifica_chama_proc();
          }
          |
          { 
            
            if(procedure!=NULL && do_verify){
              n_param_atual=0;
              verifica_chama_proc();
            }
          }
;

le_params:
          get_param get_param_loop
;

get_param_loop: 
          VIRGULA{
            temp->inicio=temp->inicio->itemProx;
          }
          le_params |
;

get_param:
          expressao_simples{n_param_atual++;}
;

expressao_simples ://ABRE_PARENTESES expressao_simples FECHA_PARENTESES|
            expressao_simples SOMA termo {
              geraCodigo (NULL, "SOMA",NULL,NULL,NULL); 
              printf ("+\n"); }
            |expressao_simples SUB termo {
              geraCodigo (NULL, "SUBT",NULL,NULL,NULL); 
              printf ("-"); } 
            |termo
;

termo      : //ABRE_PARENTESES expressao_simples FECHA_PARENTESES|
             termo MUL fator  {
              geraCodigo (NULL, "MULT",NULL,NULL,NULL); 
              printf ("*"); }| 
             termo DIV fator  {
              geraCodigo (NULL, "DIVI",NULL,NULL,NULL); 
              printf ("/"); }|
             fator
;

fator      :  ABRE_PARENTESES expressao_simples FECHA_PARENTESES
              |IDENT {
                if(!procura_compara(VS)){
                  if(!procura_compara(PF)){
                    if(!procura_compara(FUN)){
                      printf("Valor %s não encontrado\n\n",token);
                      yyerror("");
                    }
                  }
                }
                
                if(!gera_comando_crvl(VS)){
                  if(!gera_comando_crvl(PF)){
                    procedure = procura_tbsimb(token,FUN);
                    if(procedure == NULL){
                      printf("Valor %s não encontrado\n\n",token);
                      yyerror("");
                    }
                    do_verify=1;
                  }
                  else
                    do_verify=0;
                }
                else{
                  do_verify=0;
                }
              }
              procedure_function_call
              |NUMERO{
                geraCodigo (NULL, "CRCT",token,NULL,NULL); 
              }
;

comando_write : WRITE ABRE_PARENTESES expressao_simples {
                  carrega_valor_imprime();
                } FECHA_PARENTESES |
                WRITE ABRE_PARENTESES NUMERO FECHA_PARENTESES
;


cond_if     : if_then cond_else 
            { 
               geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                lr->fim=lr->fim->itemAnt;
            }
;

if_then     : IF expressao {
                adiciona_item_lista();
                geraCodigo (NULL, "DSVF",lr->fim->identificador,NULL,NULL);
              }
             THEN internal
;

cond_else   : ELSE{
                define_else();
              } 
              internal 
              
            | %prec LOWER_THAN_ELSE
            | PONTO_E_VIRGULA
            
              
;

internal: comando  | comando_composto  ;

comando_repetitivo:
                     com_while ;

com_while:    {
                adiciona_item_lista();
                geraCodigo (lr->fim->identificador, "NADA",NULL,NULL,NULL);
              }
              WHILE expressao {
                adiciona_item_lista();
                geraCodigo (NULL, "DSVF",lr->fim->identificador,NULL,NULL);
              } DO internal { geraCodigo (NULL, "DSVS",lr->fim->itemAnt->identificador,NULL,NULL);
                              remove_item_lista(lr->fim->itemAnt->identificador);
                              geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                              if(lr->fim->itemAnt != NULL)
                                lr->fim=lr->fim->itemAnt;
                        }
;

expressao   : ABRE_PARENTESES prior2 FECHA_PARENTESES | prior2;

prior2  :     prior2 IGUAL prior1 {geraCodigo (NULL, "CMIG",NULL,NULL,NULL);}|
              prior2 DIFE  prior1 {geraCodigo (NULL, "CMDG",NULL,NULL,NULL);}|
              prior1
;

prior1  :     prior1 MAIOR expressao_simples {geraCodigo (NULL, "CMMA",NULL,NULL,NULL);}|
              prior1 MENOR expressao_simples {geraCodigo (NULL, "CMME",NULL,NULL,NULL);}|
              prior1 MAEG  expressao_simples {geraCodigo (NULL, "CMAG",NULL,NULL,NULL);}| 
              prior1 MEEG  expressao_simples {geraCodigo (NULL, "CMEG",NULL,NULL,NULL);}|
              expressao_simples
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
   lr = (lista_rotulos *) malloc (sizeof(lista_rotulos));
   lp = (lista_params *) malloc (sizeof(lista_params));
   tbs->topo_pilha = NULL;
   lr->inicio = NULL;
   lp->inicio = NULL;

   tbs->fim_pilha = NULL;
   lr->fim = NULL;
   lp->fim = NULL;

   tbs->itens = NULL;

   tbs->n_itens = 0;
   lr->n_rotulos = 0;
   lp->n_param = 0;

   yyin=fp;
   yyparse();

   return 0;
}

