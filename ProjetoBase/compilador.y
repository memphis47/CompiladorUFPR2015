
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
typedef struct itemLista itemLista;

struct itemLista
{
  char *identificador;
  int decl;
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
tabela_simbolos *tbgoto;
tabela_simbolos *tbgotopendentes;
lista_rotulos *lr;
lista_rotulos *lista_comparacoes;
lista_params *lp;
lista_params *temp;
char *param1;
char *param2;
char *param3;
char *param1Aux;
char *param2Aux;
char *param3Aux;
char *comparacao;
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
int rtprdone= 0;
int compara=0;
tipo_parametro passado;
tipo_variavel  compItem = -1;
tipo_variavel  resultExpected = -1;

item *procedure;
item *procedureSimpleExpr;
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

item * procura_tbsimb(char * token){
  item *itemAtual = tbs->topo_pilha;
  while(itemAtual != NULL){
    if(strcmp (token, itemAtual->identificador) == 0){
      return itemAtual;
    }
    itemAtual = itemAtual->itemAnt;
  }
  return NULL;
}

item * procura_tbsimb_categoria(categorias cat){
  item *itemAtual = tbs->topo_pilha;
  while(itemAtual != NULL){
    if(itemAtual->categoria == cat){
      return itemAtual;
    }
    itemAtual = itemAtual->itemAnt;
  }
  return NULL;
}

item * procura_tbgoto(char * token, categorias cat){
  item *itemAtual = tbgoto->topo_pilha;
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
  item *item = procura_tbsimb(token);
  if(item!=NULL && item->categoria == cat){
    param1 = (char *) malloc (4 * sizeof(char));
    param2 = (char *) malloc (4 * sizeof(char));
    sprintf(param1, "%d", item->nivel_lexico);
    sprintf(param2, "%d", item->deslocamento);
    if(param_chamada && temp->inicio->passagem == REFERENCIA){ //chamada de parametro passado por referencia
      if(item->tipo == temp->inicio->tipo){
        if(item->passagem==VALOR || item->passagem==-1)
          geraCodigo (NULL, "CREN",param1,param2,NULL);
        else if(item->passagem == REFERENCIA)
          geraCodigo (NULL, "CRVL",param1,param2,NULL);
      }
      else{
        printf("ERRO: Tipo passado: %d\n",item->tipo);
        printf("Tipo esperado %d\n",temp->inicio->tipo);
        yyerror("");
      }
    }
    else if(param_chamada && temp->inicio->passagem == VALOR){ //chamada de parametro passado por referencia
      if(item->tipo == temp->inicio->tipo){
        if(item->passagem==VALOR || item->passagem==-1)
          geraCodigo (NULL, "CRVL",param1,param2,NULL);
        else if(item->passagem == REFERENCIA)
          geraCodigo (NULL, "CRVI",param1,param2,NULL);
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
  lr->fim->decl=0;
  rotNumber++;
}

void adiciona_item_lista_comparacao(char *comp){
  itemLista *auxItem = (itemLista *) malloc (sizeof(itemLista));


  auxItem->identificador = (char *) malloc (256 * sizeof(char));
  strcpy(auxItem->identificador,comp);
  

  if(lista_comparacoes->inicio == NULL){
    lista_comparacoes->inicio = auxItem; 
    lista_comparacoes->fim = auxItem;
  }
  else{
    lista_comparacoes->fim->itemProx = auxItem;
    auxItem->itemAnt = lista_comparacoes->fim;
    lista_comparacoes->fim = auxItem;
  }
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

void remove_item_lista_comparacoes(char *token){
  
  lista_comparacoes->fim=lista_comparacoes->fim->itemAnt;
  if(lista_comparacoes->fim !=NULL)
    lista_comparacoes->fim->itemProx=NULL;
  else{
    lista_comparacoes->inicio=NULL;
  }
  
}

int remove_ts(){
  item *itemAtual = tbs->topo_pilha;
  int count = 0;
  while(itemAtual!= NULL 
  && (itemAtual->nivel_lexico > nivel_lexico_atual 
  || itemAtual->categoria == VS || itemAtual->categoria == PF)){
    item *aux = itemAtual;
    if(itemAtual->categoria==VS)
      count++;
    itemAtual = itemAtual->itemAnt;
    free(aux);
    
  }
  tbs->topo_pilha = itemAtual;
  return count;
}

void verifica_existencia(){
  item* item=procura_tbsimb(token);
  if(item == NULL){
    item=procura_tbsimb(token);
    if(item!= NULL){
      if(item->nivel_lexico == nivel_lexico_atual)
        yyerror("Variavel já declarada\n");
    }
  }
  else{
    if(item->nivel_lexico == nivel_lexico_atual)
        yyerror("Variavel já declarada\n");
  }
}

void adiciona_ts(categorias cat,char *rot){
  verifica_existencia();
  item *auxItem = (item *) malloc (sizeof(item));
  auxItem->itemAnt = tbs->topo_pilha;
  auxItem->itemProx = NULL;
  
  auxItem->tipo = -1;
  auxItem->identificador = (char *) malloc (256 * sizeof(char));
  strcpy(auxItem->identificador,token);
  auxItem->categoria = cat;
  auxItem->nivel_lexico = nivel_lexico_atual;
  if(cat==VS){
    auxItem->deslocamento = desl;
    desl ++;
  }
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
  
}

void adiciona_label(){
  item *auxItem = (item *) malloc (sizeof(item));
  auxItem->itemAnt = tbgoto->topo_pilha;
  auxItem->itemProx = NULL;
  auxItem->tipo = -1;
  auxItem->identificador = (char *) malloc (256 * sizeof(char));
  strcpy(auxItem->identificador,token);
  auxItem->rotulo = (char *) malloc (256 * sizeof(char));
  strcpy(auxItem->rotulo,lr->fim->identificador);
  auxItem->categoria = LAB;
  auxItem->nivel_lexico = nivel_lexico_atual;
  auxItem->passagem = -1;
  auxItem->decl = 0;
  auxItem->pendente = 1;
  tbgoto->n_itens = tbgoto->n_itens++;
  tbgoto->topo_pilha = auxItem;
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
  item *itemAtual=procura_tbsimb(token);
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
    param1 = (char *) malloc (4 * sizeof(char));
    sprintf(param1, "%d", num_vars);
    num_vars=0;
    geraCodigo (NULL, "AMEM",param1,NULL,NULL); 
  }  
}

void verifyLabels(){
  item *itemAtual = tbgoto->topo_pilha;
  while(itemAtual!= NULL && itemAtual->nivel_lexico == nivel_lexico_atual){
      if(itemAtual->pendente){
        printf("ERRO: Label %s foi chamada porem não foi instanciada\n",itemAtual->identificador);
        yyerror("");
      }
    itemAtual = itemAtual->itemAnt;
  }
}

void doDmem(){
  verifyLabels();
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
  
  item *item = procura_tbsimb(token);
  
  if(item!=NULL && item->categoria == cat){
    
    
    param1Aux = (char *) malloc (4 * sizeof(char));
    param2Aux = (char *) malloc (4 * sizeof(char));
    sprintf(param1Aux, "%d", item->nivel_lexico);
    sprintf(param2Aux, "%d", item->deslocamento);
    
  }
  return item;
}

void verifica_chama_proc(){
  if(procedure->param!=NULL &&  n_param_atual!=procedure->param->n_param){
    printf("ERRO: Número de parametros passados: %d\n",n_param_atual);
    printf("Número de parametros esperados: %d\n",procedure->param->n_param);
    yyerror("");
  }
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
  // if(lr->fim->itemAnt != NULL)
  //   lr->fim=lr->fim->itemAnt;
  adiciona_item_lista();
}

int getNumVars(){
  item *itemAtual = tbs->topo_pilha;
  int vars=0;
  printf("\n\n\nNivel Lexico Atual: %d\n",nivel_lexico_atual);
   printf("Item nl: %d\n",itemAtual->nivel_lexico);
  while(itemAtual!= NULL 
        && itemAtual->nivel_lexico >= nivel_lexico_atual){
    printf("Item Identificador: %s\n",itemAtual->identificador);
    if(itemAtual->categoria == VS && itemAtual->nivel_lexico == nivel_lexico_atual)
      vars++;
    itemAtual = itemAtual->itemAnt;
  }
  printf("\n\n\n\n");
  return vars;
}

void define_enrt(){
  item* item = procura_tbgoto(token,LAB);
  
  if(item!=NULL){
    if(!item->decl){
      char lexico[4];
      char nVars[4];
      sprintf(lexico, "%d", nivel_lexico_atual);
      item->pendente=0;
      sprintf(nVars, "%d", getNumVars());
      geraCodigo(item->rotulo,"ENRT",lexico,nVars,NULL);
      item->decl=1;
    }
    else
      yyerror("Label declarado anteriormente\n");
  }
  else
    yyerror("Label não encontrada\n");
}

void desv_rot(){
  item* item = procura_tbgoto(token,LAB);
  
  if(item!=NULL){
    char atLexico[4];
    char destlexico[4];
    sprintf(atLexico, "%d", nivel_lexico_atual);
    sprintf(destlexico, "%d", item->nivel_lexico);
    if(!item->decl)
      item->pendente=1;
    geraCodigo(NULL,"DSVR",item->rotulo,destlexico,atLexico);
  }
}

void armi_armz(){
  compItem = -1;
  if(procedure==NULL && tempItemArmz!=NULL && tempItemArmz->passagem == REFERENCIA){
    
      param1Aux = (char *) malloc (4 * sizeof(char));
      param2Aux = (char *) malloc (4 * sizeof(char));
    if(tempItemArmz!=NULL){
      sprintf(param1Aux, "%d", tempItemArmz->nivel_lexico);
      sprintf(param2Aux, "%d", tempItemArmz->deslocamento);
    }  
    geraCodigo (NULL, "ARMI",param1Aux,param2Aux,NULL);
  }
  else{
      param1Aux = (char *) malloc (4 * sizeof(char));
      param2Aux = (char *) malloc (4 * sizeof(char));
    if(procedure!=NULL && tempItemArmz==NULL){
      sprintf(param1Aux, "%d", procedure->nivel_lexico);
      sprintf(param2Aux, "%d", procedure->deslocamento);
    }
    else if(tempItemArmz!=NULL){
      sprintf(param1Aux, "%d", tempItemArmz->nivel_lexico);
      sprintf(param2Aux, "%d", tempItemArmz->deslocamento);
    }
    geraCodigo (NULL, "ARMZ",param1Aux,param2Aux,NULL);
    procedure==NULL;
    tempItemArmz=NULL;
  }
}

void define_comp(char * value){
  adiciona_item_lista_comparacao(value);    
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
%token LABEL GOTO
%token READ
%token AND OR TRUEV FALSEV
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
              parte_declara_labels
              parte_declara_vars { 
                reza();
              }
              parse_bloco
;

parte_declara_labels:
              LABEL le_label le_label_loop PONTO_E_VIRGULA|
;

le_label_loop: VIRGULA le_label le_label_loop |
;

le_label:
          NUMERO {
            adiciona_item_lista();
            adiciona_label();
            remove_item_lista(lr->fim->identificador);
          }
;

parse_bloco:
            proc_func_def_loop  comando_composto{ doDmem(); }


;

proc_func_def_loop:
            { 
              if(!rtprdone){
                if(lr!=NULL && lr->fim!=NULL)
                 remove_item_lista(lr->fim->identificador);
                adiciona_item_lista();
                geraCodigo (NULL, "DSVS",lr->fim->identificador,NULL,NULL);
              }
            } 
            proc_func_def 
            proc_func_def_loop 
            {
              if(!lr->fim->decl){
                geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                lr->fim->decl=1;
              }
            }
            |
;

proc_func_def:
              proc_func_def_choose proc_com
;

proc_func_def_choose:
              PROCEDURE{tempCategoria=PROC;}  | 
              FUNCTION{tempCategoria=FUN;}
;

proc_com: IDENT{
            rtprdone=0;
            free(ident);
            ident = (char *) malloc (256*sizeof(char));
            strcpy(ident,token);
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
            lr->fim->decl=1;
          } 
          
          blocoProc PONTO_E_VIRGULA
          {
            remove_item_lista(lr->fim->identificador);
            char lexico[4];
            sprintf(lexico, "%d", nivel_lexico_atual);
            char np[4];
            procedure = procura_tbsimb_categoria(PROC);
            if(procedure==NULL){
              procedure = procura_tbsimb_categoria(FUN);
              if(procedure==NULL)
                yyerror("Valor nao encontrado\n\n");
            }
            if(procedure->param!=NULL)
              sprintf(np, "%d", procedure->param->n_param);
            else
              sprintf(np, "%d", 0);
            geraCodigo (NULL, "RTPR",lexico,np,NULL); 
            nivel_lexico_atual--;
            rtprdone=1;
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
                 |
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
              parte_declara_labels
              {desl=0;}parte_declara_vars { reza(); } 
              parse_bloco|
              parse_bloco
;

parte_declara_vars:  {num_vars=0;}var 
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

comando_composto:  T_BEGIN verify_goto comando comando_composto_loop T_END ;

comando_composto_loop: PONTO_E_VIRGULA verify_goto comando comando_composto_loop | PONTO_E_VIRGULA | 
; 

comando:  IDENT
          {
            
            procedure = procura_tbsimb(token);
            no_proc=0;
            
            if(procedure != NULL && procedure->categoria !=PROC){
              if(procedure->categoria != FUN){
                procedure=NULL;
                tempItemArmz = cria_valores_armz(VS);
                resultExpected= tempItemArmz->tipo;
              }
              else{
                tempItemArmz=NULL;
                resultExpected= procedure->tipo;
              }
              no_proc=1;
            }
            else{
              resultExpected= procedure->tipo;
              do_verify=1;
              tempItemArmz=NULL;
            }
            compItem=-1;
          }
          parse_comando
          | comando_write
          | comando_le
          | cond_if
          | comando_repetitivo
          | comando_goto

;

verify_goto:
            NUMERO {
              define_enrt();
            } DOIS_PONTOS | 
;

comando_goto:
            GOTO NUMERO {desv_rot();}
;

parse_comando:
          
          ATRIBUICAO {
            
            if(procedure!=NULL && !no_proc){
              yyerror("Tipo de operação nao permitida");
            }
          }
          expressaoAT {
            if(resultExpected != compItem){
              yyerror("Operação não permitida\n");
            }
            armi_armz();
            compItem=-1;
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
            compItem=-1;
            temp->inicio=temp->inicio->itemProx;
          }
          le_params |
;

get_param:
          expressao_simples{n_param_atual++;}
;



expressaoAT :
            expressao_simples 
            parse_expressao

;

parse_expressao:
    compara 
    expressao_simples {
      compItem=BOOLEAN;
      geraCodigo(NULL,lista_comparacoes->fim->identificador,NULL,NULL);
      remove_item_lista_comparacoes(lista_comparacoes->fim->identificador);
    }
    |

;

expressao   :
               expressao_simples 
               compara 
               expressao_simples {
                 compItem=BOOLEAN;
                 geraCodigo(NULL,lista_comparacoes->fim->identificador,NULL,NULL);
                 remove_item_lista_comparacoes(lista_comparacoes->fim->identificador);
               }
               |
               expressao_simples 
;

expressao_simples ://ABRE_PARENTESES expressao_simples FECHA_PARENTESES|
            expressao_simples SOMA termo {
              if(compItem!=-1 && compItem!=INTEGER){
                  yyerror("Operação não permitida\n");
                }
              compItem=INTEGER;
              geraCodigo (NULL, "SOMA",NULL,NULL,NULL); 
              printf ("+\n"); }
            |expressao_simples SUB termo {
              if(compItem!=-1 && compItem!=INTEGER){
                  yyerror("Operação não permitida\n");
                }
              compItem=INTEGER;
              geraCodigo (NULL, "SUBT",NULL,NULL,NULL); 
              printf ("-"); } 
            |expressao_simples OR termo {
              if(compItem!=-1 && compItem!=BOOLEAN){
                  yyerror("Operação não permitida\n");
                }
              compItem=BOOLEAN;
              geraCodigo (NULL, "DISJ",NULL,NULL,NULL); 
              printf ("OR"); }
            |termo
;

termo      : //ABRE_PARENTESES expressao_simples FECHA_PARENTESES|
             termo MUL fator  {
               if(compItem!=-1 && compItem!=INTEGER){
                  yyerror("Operação não permitida\n");
                }
               compItem=INTEGER;
              geraCodigo (NULL, "MULT",NULL,NULL,NULL); 
              printf ("*"); }| 
             termo DIV fator  {
               if(compItem!=-1 && compItem!=INTEGER){
                  yyerror("Operação não permitida\n");
                }
               compItem=INTEGER;
              geraCodigo (NULL, "DIVI",NULL,NULL,NULL); 
              printf ("/"); }|
             termo AND fator  {
               if(compItem!=-1 && compItem!=BOOLEAN){
                  yyerror("Operação não permitida\n");
                }
               compItem=BOOLEAN;
              geraCodigo (NULL, "CONJ",NULL,NULL,NULL); 
              printf ("AND"); }|
             fator
;

fator      :  ABRE_PARENTESES{compItem=-1;} expressaoAT FECHA_PARENTESES
              | 
              IDENT {
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
                    procedure = procura_tbsimb(token);
                    if(procedure->categoria != FUN){
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
                if(compItem!=-1 && compItem!=INTEGER){
                  yyerror("Operação não permitida");
                }
                compItem = INTEGER;
                geraCodigo (NULL, "CRCT",token,NULL,NULL); 
              }
              |TRUEV{
                if(compItem!=-1 && compItem!=BOOLEAN){
                  yyerror("Operação não permitida");
                }
                compItem = BOOLEAN;
                geraCodigo (NULL, "CRCT","1",NULL,NULL); 
              }
              |FALSEV{
                if(compItem!=-1 && compItem!=BOOLEAN){
                  yyerror("Operação não permitida");
                }
                compItem = BOOLEAN;
                geraCodigo(NULL, "CRCT","0",NULL,NULL); 
              }
;


compara:
        IGUAL{define_comp("CMIG");}|
        DIFE {define_comp("CMDG");}|
        MAIOR{define_comp("CMMA");}|
        MENOR{define_comp("CMME");}|
        MAEG {define_comp("CMAG");}|
        MEEG {define_comp("CMEG");}  
        
comando_write : WRITE ABRE_PARENTESES comando_write_once comando_write_loop 
                FECHA_PARENTESES
;

comando_write_loop : VIRGULA comando_write_once comando_write_loop |
;

comando_write_once :  expressao_simples {
                        geraCodigo (NULL, "IMPR",NULL,NULL,NULL);
                      }
;

comando_le:
            READ ABRE_PARENTESES le_simples le_loop FECHA_PARENTESES
;

le_loop:
        VIRGULA le_simples le_loop |
;

le_simples:
            IDENT{
              geraCodigo(NULL,"LEIT",NULL,NULL,NULL);
              cria_valores_armz(VS);
              geraCodigo (NULL, "ARMZ",param1Aux,param2Aux,NULL);
            }

cond_if     : if_then cond_else 
            { 
              geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
              lr->fim->decl=1;
              remove_item_lista(lr->fim->identificador);
            }
;

if_then     : IF {resultExpected=BOOLEAN; compItem=-1;} expressao {
                if(resultExpected!=compItem){
                  yyerror("Operação não permitida, é necessario que o retorno da expressao do if seja booleano");
                }
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
            
              
;

internal: comando  | comando_composto  ;

comando_repetitivo:
                     com_while ;

com_while:    {
                adiciona_item_lista();
                geraCodigo (lr->fim->identificador, "NADA",NULL,NULL,NULL);
                lr->fim->decl=1;
              }
              WHILE expressao {
                adiciona_item_lista();
                geraCodigo (NULL, "DSVF",lr->fim->identificador,NULL,NULL);
              } DO  internal { geraCodigo (NULL, "DSVS",lr->fim->itemAnt->identificador,NULL,NULL);
                              remove_item_lista(lr->fim->itemAnt->identificador);
                              geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                              lr->fim->decl=1;
                              if(lr->fim->itemAnt != NULL)
                                lr->fim=lr->fim->itemAnt;
                        }
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
   tbgoto = (tabela_simbolos *) malloc (sizeof(tabela_simbolos));
   tbgotopendentes = (tabela_simbolos *) malloc (sizeof(tabela_simbolos));
   lr = (lista_rotulos *) malloc (sizeof(lista_rotulos));
   lista_comparacoes = (lista_rotulos *) malloc (sizeof(lista_rotulos));
   lp = (lista_params *) malloc (sizeof(lista_params));
   tbs->topo_pilha = NULL;
   tbgoto->topo_pilha = NULL;
   tbgotopendentes->topo_pilha = NULL;
   lr->inicio = NULL;
   lista_comparacoes->inicio = NULL;
   lp->inicio = NULL;

   tbs->fim_pilha = NULL;
   tbgoto->fim_pilha = NULL;
   tbgotopendentes->fim_pilha = NULL;
   lr->fim = NULL;
   lista_comparacoes->fim = NULL;
   lp->fim = NULL;

   tbs->itens = NULL;
   tbgoto->itens = NULL;
   tbgotopendentes->itens = NULL;

   tbs->n_itens = 0;
   lr->n_rotulos = 0;
   lp->n_param = 0;

   yyin=fp;
   yyparse();

   return 0;
}

