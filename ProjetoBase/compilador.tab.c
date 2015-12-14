/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 6 "compilador.y" /* yacc.c:339  */

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


#line 638 "compilador.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "compilador.tab.h".  */
#ifndef YY_YY_COMPILADOR_TAB_H_INCLUDED
# define YY_YY_COMPILADOR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PROGRAM = 258,
    ABRE_PARENTESES = 259,
    FECHA_PARENTESES = 260,
    WRITE = 261,
    VIRGULA = 262,
    PONTO_E_VIRGULA = 263,
    DOIS_PONTOS = 264,
    PONTO = 265,
    T_BEGIN = 266,
    T_END = 267,
    VAR = 268,
    IDENT = 269,
    ATRIBUICAO = 270,
    NUMERO = 271,
    SOMA = 272,
    SUB = 273,
    MUL = 274,
    DIV = 275,
    IF = 276,
    THEN = 277,
    ELSE = 278,
    WHILE = 279,
    DO = 280,
    DIFE = 281,
    IGUAL = 282,
    MAEG = 283,
    MAIOR = 284,
    MENOR = 285,
    MEEG = 286,
    PROCEDURE = 287,
    FUNCTION = 288,
    LABEL = 289,
    GOTO = 290,
    READ = 291,
    AND = 292,
    OR = 293,
    TRUEV = 294,
    FALSEV = 295,
    LOWER_THAN_ELSE = 296
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_COMPILADOR_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 731 "compilador.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   139

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  79
/* YYNRULES -- Number of rules.  */
#define YYNRULES  123
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  197

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   595,   595,   602,   595,   611,   610,   618,   618,   621,
     621,   625,   633,   639,   639,   655,   659,   663,   664,   667,
     679,   667,   711,   709,   718,   722,   728,   734,   734,   738,
     738,   740,   740,   745,   745,   756,   756,   755,   758,   761,
     761,   764,   768,   769,   775,   773,   781,   786,   792,   793,
     798,   800,   800,   800,   804,   803,   829,   830,   831,   832,
     833,   838,   838,   840,   844,   849,   849,   863,   867,   877,
     867,   889,   899,   903,   903,   907,   911,   917,   923,   929,
     934,   942,   946,   953,   960,   967,   971,   978,   985,   992,
     995,   995,   997,   997,  1023,  1030,  1037,  1048,  1049,  1050,
    1051,  1052,  1053,  1055,  1059,  1059,  1062,  1068,  1072,  1072,
    1076,  1082,  1090,  1090,  1090,  1100,  1100,  1105,  1110,  1110,
    1113,  1115,  1120,  1115
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROGRAM", "ABRE_PARENTESES",
  "FECHA_PARENTESES", "WRITE", "VIRGULA", "PONTO_E_VIRGULA", "DOIS_PONTOS",
  "PONTO", "T_BEGIN", "T_END", "VAR", "IDENT", "ATRIBUICAO", "NUMERO",
  "SOMA", "SUB", "MUL", "DIV", "IF", "THEN", "ELSE", "WHILE", "DO", "DIFE",
  "IGUAL", "MAEG", "MAIOR", "MENOR", "MEEG", "PROCEDURE", "FUNCTION",
  "LABEL", "GOTO", "READ", "AND", "OR", "TRUEV", "FALSEV",
  "LOWER_THAN_ELSE", "$accept", "programa", "$@1", "$@2", "bloco", "$@3",
  "parte_declara_labels", "le_label_loop", "le_label", "parse_bloco",
  "proc_func_def_loop", "$@4", "proc_func_def", "proc_func_def_choose",
  "proc_com", "$@5", "$@6", "parse_proc_decl", "$@7", "func_type",
  "param_loop", "declara_param", "$@8", "$@9", "parse_declaracao", "$@10",
  "blocoProc", "$@11", "$@12", "parte_declara_vars", "$@13", "var",
  "declara_vars", "declara_var", "$@14", "lista_id_var", "lista_idents",
  "comando_composto", "comando_composto_loop", "comando", "$@15",
  "verify_goto", "$@16", "comando_goto", "parse_comando", "$@17",
  "procedure_function_call", "$@18", "$@19", "le_params", "get_param_loop",
  "$@20", "get_param", "expressaoAT", "parse_expressao", "expressao",
  "expressao_simples", "termo", "fator", "$@21", "$@22", "compara",
  "comando_write", "comando_write_loop", "comando_write_once",
  "comando_le", "le_loop", "le_simples", "cond_if", "if_then", "$@23",
  "$@24", "cond_else", "$@25", "internal", "comando_repetitivo",
  "com_while", "$@26", "$@27", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296
};
# endif

#define YYPACT_NINF -143

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-143)))

#define YYTABLE_NINF -69

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -143,     4,    10,  -143,     2,  -143,    15,    23,  -143,    60,
      30,    42,    35,  -143,    54,    62,  -143,  -143,    66,  -143,
    -143,    61,    54,    67,    65,    63,  -143,    66,  -143,  -143,
      68,    -5,  -143,    63,  -143,    59,  -143,    64,  -143,  -143,
    -143,    65,    69,  -143,    71,    72,  -143,     0,  -143,  -143,
    -143,  -143,  -143,    73,    74,  -143,  -143,    75,    77,    79,
    -143,  -143,  -143,  -143,    70,  -143,  -143,    78,    22,    80,
    -143,     1,     8,     1,  -143,    76,    32,    82,  -143,  -143,
       1,    83,    81,    84,  -143,  -143,  -143,  -143,  -143,  -143,
    -143,    -8,     5,  -143,    90,  -143,  -143,  -143,    85,  -143,
      33,  -143,    91,     0,  -143,    -3,  -143,  -143,    93,    89,
    -143,  -143,     1,    95,     1,     1,     1,     1,     1,     1,
       1,    99,     1,  -143,    86,  -143,  -143,  -143,  -143,  -143,
    -143,     1,    76,   100,    79,  -143,  -143,  -143,    87,    89,
      83,   101,  -143,  -143,     9,   102,    33,  -143,     5,     5,
       5,  -143,  -143,  -143,    90,  -143,  -143,     1,    -3,    -8,
      91,  -143,  -143,    -3,  -143,    93,  -143,   104,  -143,  -143,
     103,  -143,  -143,     1,  -143,   105,   107,    -8,  -143,  -143,
    -143,  -143,   106,   108,  -143,  -143,    -8,  -143,  -143,  -143,
    -143,  -143,  -143,     1,    65,  -143,  -143
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,     3,     0,     0,    49,     0,
       0,     0,     8,    48,     0,     0,    39,    11,    10,     4,
       5,     0,     0,     0,    13,     0,    40,    10,     7,     6,
       0,     0,    47,    41,    43,     0,     9,    63,    12,    17,
      18,    13,     0,    42,     0,     0,    61,   121,    14,    19,
      16,    46,    44,     0,     0,    54,   112,     0,     0,    53,
      60,    56,    57,    58,   117,    59,   120,     0,    26,     0,
      62,     0,    71,     0,    64,     0,    63,     0,   115,   111,
       0,    31,     0,     0,    24,    45,    90,    92,    94,    95,
      96,   106,    85,    89,   105,    65,    55,    67,     0,   113,
      81,   110,   109,   121,    50,   121,   122,    29,    28,     0,
      25,    20,     0,    71,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,     0,    98,    97,   101,    99,   100,
     102,     0,     0,     0,    53,   119,   118,   116,     0,     0,
      31,     0,    33,    32,    13,     0,    79,    93,    82,    83,
      84,    86,    87,    88,   105,   103,    66,     0,   121,    80,
     109,   107,    51,   121,    30,    28,    22,     0,    35,    38,
       0,    91,    77,     0,   104,     0,    75,    76,   114,   108,
     123,    27,    26,     0,    39,    21,    78,    70,    73,    72,
      23,    34,    36,     0,    13,    74,    37
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -143,  -143,  -143,  -143,  -143,  -143,   -35,    92,    94,  -142,
      88,  -143,  -143,  -143,  -143,  -143,  -143,  -143,  -143,   -65,
     -47,   -19,  -143,  -143,   -16,  -143,  -143,  -143,  -143,   -60,
    -143,  -143,  -143,    97,  -143,  -143,  -143,    96,    -9,   -46,
    -143,    51,  -143,  -143,  -143,  -143,    18,  -143,  -143,   -61,
    -143,  -143,  -143,     6,  -143,    53,   -73,   -69,   -64,  -143,
    -143,   -12,  -143,   -18,    17,  -143,   -25,     7,  -143,  -143,
    -143,  -143,  -143,  -143,  -129,  -143,  -143,  -143,  -143
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     6,    15,    24,    16,    23,    18,    29,
      30,    31,    41,    42,    50,    68,   144,    83,   182,    84,
     141,   108,   139,   109,   143,   167,   170,   184,   194,    20,
      21,    26,    33,    34,    69,    35,     9,   135,    77,   136,
      72,    47,    53,    60,    96,   122,    97,    98,   157,   175,
     189,   193,   176,   145,   172,    99,    91,    92,    93,   112,
     113,   131,    61,   121,    94,    62,   133,   102,    63,    64,
      73,   124,    79,   105,   137,    65,    66,    67,   138
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     100,    59,   169,    54,     3,    86,    54,   100,    37,   114,
     115,    55,   -68,     4,    55,    87,     5,    88,    56,     7,
     -15,    56,    -8,    95,   117,   118,    81,    39,    40,   178,
     116,    82,    57,    58,   180,    57,    58,     8,    12,   146,
      89,    90,   119,    14,   -52,   148,   149,   150,    46,   146,
     114,   115,   196,   151,   152,   153,    13,   134,   159,   125,
     126,   127,   128,   129,   130,    10,    44,    11,    45,    14,
      17,   116,    19,    22,    25,    28,   -15,    32,    71,    37,
      46,    75,    70,    49,   177,    51,    52,    76,    85,   123,
     101,    74,   111,    78,   104,   110,   107,   120,   132,   -68,
     186,   140,    80,   142,   155,   161,   166,   171,   158,   168,
     187,   185,   163,   183,   188,    82,    27,   190,   181,    36,
     177,   165,   191,   164,   192,   162,    38,   103,   156,    48,
      43,   147,   195,   106,   173,   179,   174,   154,     0,   160
};

static const yytype_int16 yycheck[] =
{
      73,    47,   144,     6,     0,     4,     6,    80,    11,    17,
      18,    14,     4,     3,    14,    14,    14,    16,    21,     4,
      11,    21,    13,    15,    19,    20,     4,    32,    33,   158,
      38,     9,    35,    36,   163,    35,    36,    14,     8,   112,
      39,    40,    37,    34,    12,   114,   115,   116,    16,   122,
      17,    18,   194,   117,   118,   119,    14,   103,   131,    26,
      27,    28,    29,    30,    31,     5,     7,     7,     9,    34,
      16,    38,    10,     7,    13,     8,    11,    14,     4,    11,
      16,     4,     9,    14,   157,    14,    14,     8,     8,     4,
      14,    16,     8,    23,    12,    14,    13,     7,     7,     4,
     173,     8,    24,    14,     5,     5,     5,     5,    22,   144,
       5,     8,    25,     9,     7,     9,    22,   182,   165,    27,
     193,   140,    14,   139,   184,   134,    30,    76,   122,    41,
      33,   113,   193,    80,   146,   160,   154,   120,    -1,   132
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    43,    44,     0,     3,    14,    45,     4,    14,    78,
       5,     7,     8,    14,    34,    46,    48,    16,    50,    10,
      71,    72,     7,    49,    47,    13,    73,    50,     8,    51,
      52,    53,    14,    74,    75,    77,    49,    11,    79,    32,
      33,    54,    55,    75,     7,     9,    16,    83,    52,    14,
      56,    14,    14,    84,     6,    14,    21,    35,    36,    81,
      85,   104,   107,   110,   111,   117,   118,   119,    57,    76,
       9,     4,    82,   112,    16,     4,     8,    80,    23,   114,
      24,     4,     9,    59,    61,     8,     4,    14,    16,    39,
      40,    98,    99,   100,   106,    15,    86,    88,    89,    97,
      98,    14,   109,    83,    12,   115,    97,    13,    63,    65,
      14,     8,   101,   102,    17,    18,    38,    19,    20,    37,
       7,   105,    87,     4,   113,    26,    27,    28,    29,    30,
      31,   103,     7,   108,    81,    79,    81,   116,   120,    64,
       8,    62,    14,    66,    58,    95,    98,    88,    99,    99,
      99,   100,   100,   100,   106,     5,    95,    90,    22,    98,
     109,     5,    80,    25,    66,    63,     5,    67,    48,    51,
      68,     5,    96,   103,   105,    91,    94,    98,   116,   108,
     116,    62,    60,     9,    69,     8,    98,     5,     7,    92,
      61,    14,    71,    93,    70,    91,    51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    44,    45,    43,    47,    46,    48,    48,    49,
      49,    50,    51,    53,    52,    52,    54,    55,    55,    57,
      58,    56,    60,    59,    59,    61,    61,    62,    62,    64,
      63,    65,    63,    67,    66,    69,    70,    68,    68,    72,
      71,    73,    74,    74,    76,    75,    77,    77,    78,    78,
      79,    80,    80,    80,    82,    81,    81,    81,    81,    81,
      81,    84,    83,    83,    85,    87,    86,    86,    89,    90,
      88,    88,    91,    93,    92,    92,    94,    95,    96,    96,
      97,    97,    98,    98,    98,    98,    99,    99,    99,    99,
     101,   100,   102,   100,   100,   100,   100,   103,   103,   103,
     103,   103,   103,   104,   105,   105,   106,   107,   108,   108,
     109,   110,   112,   113,   111,   115,   114,   114,   116,   116,
     117,   119,   120,   118
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,    10,     0,     4,     4,     0,     3,
       0,     1,     2,     0,     3,     0,     2,     1,     1,     0,
       0,     7,     0,     6,     1,     2,     0,     3,     0,     0,
       3,     0,     2,     0,     4,     0,     0,     5,     1,     0,
       2,     2,     2,     1,     0,     5,     3,     1,     3,     1,
       5,     4,     1,     0,     0,     3,     1,     1,     1,     1,
       1,     0,     3,     0,     2,     0,     3,     1,     0,     0,
       5,     0,     2,     0,     3,     0,     1,     2,     2,     0,
       3,     1,     3,     3,     3,     1,     3,     3,     3,     1,
       0,     4,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     3,     0,     1,     5,     3,     0,
       1,     2,     0,     0,     6,     0,     3,     0,     1,     1,
       1,     0,     0,     6
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 595 "compilador.y" /* yacc.c:1646  */
    { 
                nivel_lexico_atual=0;
                num_vars=0;
                desl=0;
                nivel=0;
                geraCodigo (NULL, "INPP",NULL,NULL,NULL); 
             }
#line 1963 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 602 "compilador.y" /* yacc.c:1646  */
    {adiciona_ts(PROG,NULL);}
#line 1969 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 604 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "PARA",NULL,NULL,NULL); 
             }
#line 1977 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 611 "compilador.y" /* yacc.c:1646  */
    { 
                reza();
              }
#line 1985 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 625 "compilador.y" /* yacc.c:1646  */
    {
            adiciona_item_lista();
            adiciona_label();
            remove_item_lista(lr->fim->identificador);
          }
#line 1995 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 633 "compilador.y" /* yacc.c:1646  */
    { doDmem(); }
#line 2001 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 639 "compilador.y" /* yacc.c:1646  */
    { 
              if(!rtprdone){
                if(lr!=NULL && lr->fim!=NULL)
                 remove_item_lista(lr->fim->identificador);
                adiciona_item_lista();
                geraCodigo (NULL, "DSVS",lr->fim->identificador,NULL,NULL);
              }
            }
#line 2014 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 649 "compilador.y" /* yacc.c:1646  */
    {
              if(!lr->fim->decl){
                geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                lr->fim->decl=1;
              }
            }
#line 2025 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 663 "compilador.y" /* yacc.c:1646  */
    {tempCategoria=PROC;}
#line 2031 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 664 "compilador.y" /* yacc.c:1646  */
    {tempCategoria=FUN;}
#line 2037 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 667 "compilador.y" /* yacc.c:1646  */
    {
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
#line 2053 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 679 "compilador.y" /* yacc.c:1646  */
    {
            char lexico[4];
            sprintf(lexico, "%d", nivel_lexico_atual);
            geraCodigo (rots, "ENPR",lexico,NULL,NULL); 
            adiciona_item_lista();
            lr->fim->decl=1;
          }
#line 2065 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 688 "compilador.y" /* yacc.c:1646  */
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
#line 2089 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 711 "compilador.y" /* yacc.c:1646  */
    {
                    procedure->param=(lista_params *) malloc(sizeof(lista_params));
                    memcpy(procedure->param,lp,sizeof(lista_params)); //Procedure definido com os par
                    adiciona_deslocamento_param();
                 }
#line 2099 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 722 "compilador.y" /* yacc.c:1646  */
    {
            tipo_variavel tipo= retornaTipo();
            procedure->tipo = tipo;
            lp->inicio=NULL;
            lp->fim=NULL;
          }
#line 2110 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 728 "compilador.y" /* yacc.c:1646  */
    {
            lp->inicio=NULL;
            lp->fim=NULL;
          }
#line 2119 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 738 "compilador.y" /* yacc.c:1646  */
    {passado=REFERENCIA;}
#line 2125 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 740 "compilador.y" /* yacc.c:1646  */
    {passado=VALOR;}
#line 2131 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 745 "compilador.y" /* yacc.c:1646  */
    {
              free(paramName);
              paramName = (char *) malloc (256 * sizeof(char));
              strcpy(paramName,token);
            }
#line 2141 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 751 "compilador.y" /* yacc.c:1646  */
    {adiciona_param_ts(PF,NULL,passado);}
#line 2147 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 756 "compilador.y" /* yacc.c:1646  */
    {desl=0;}
#line 2153 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 756 "compilador.y" /* yacc.c:1646  */
    { reza(); }
#line 2159 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 761 "compilador.y" /* yacc.c:1646  */
    {num_vars=0;}
#line 2165 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 775 "compilador.y" /* yacc.c:1646  */
    { 
                adiciona_tipo_ts(num_vars);
              }
#line 2173 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 782 "compilador.y" /* yacc.c:1646  */
    { /* insere última vars na tabela de símbolos */ 
               adiciona_ts(VS,NULL);
               num_vars++;
              }
#line 2182 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 786 "compilador.y" /* yacc.c:1646  */
    { /* insere vars na tabela de símbolos */
                adiciona_ts(VS,NULL);
                num_vars++;
              }
#line 2191 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 804 "compilador.y" /* yacc.c:1646  */
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
#line 2220 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 838 "compilador.y" /* yacc.c:1646  */
    {
              define_enrt();
            }
#line 2228 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 844 "compilador.y" /* yacc.c:1646  */
    {desv_rot();}
#line 2234 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 849 "compilador.y" /* yacc.c:1646  */
    {
            
            if(procedure!=NULL && !no_proc){
              yyerror("Tipo de operação nao permitida");
            }
          }
#line 2245 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 855 "compilador.y" /* yacc.c:1646  */
    {
            if(resultExpected != compItem){
              yyerror("Operação não permitida\n");
            }
            armi_armz();
            compItem=-1;
          }
#line 2257 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 867 "compilador.y" /* yacc.c:1646  */
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
#line 2272 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 877 "compilador.y" /* yacc.c:1646  */
    {
            n_param_atual=0;
            param_chamada=1;
            temp=(lista_params *) malloc(sizeof(lista_params));
            memcpy(temp,procedure->param,sizeof(lista_params));
          }
#line 2283 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 883 "compilador.y" /* yacc.c:1646  */
    {
            free(temp);
            param_chamada=0;
            verifica_chama_proc();
          }
#line 2293 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 889 "compilador.y" /* yacc.c:1646  */
    { 
            
            if(procedure!=NULL && do_verify){
              n_param_atual=0;
              verifica_chama_proc();
            }
          }
#line 2305 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 903 "compilador.y" /* yacc.c:1646  */
    {
            compItem=-1;
            temp->inicio=temp->inicio->itemProx;
          }
#line 2314 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 911 "compilador.y" /* yacc.c:1646  */
    {n_param_atual++;}
#line 2320 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 924 "compilador.y" /* yacc.c:1646  */
    {
      compItem=BOOLEAN;
      geraCodigo(NULL,lista_comparacoes->fim->identificador,NULL,NULL);
      remove_item_lista_comparacoes(lista_comparacoes->fim->identificador);
    }
#line 2330 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 936 "compilador.y" /* yacc.c:1646  */
    {
                 compItem=BOOLEAN;
                 geraCodigo(NULL,lista_comparacoes->fim->identificador,NULL,NULL);
                 remove_item_lista_comparacoes(lista_comparacoes->fim->identificador);
               }
#line 2340 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 946 "compilador.y" /* yacc.c:1646  */
    {
              if(compItem!=-1 && compItem!=INTEGER){
                  yyerror("Operação não permitida\n");
                }
              compItem=INTEGER;
              geraCodigo (NULL, "SOMA",NULL,NULL,NULL); 
              printf ("+\n"); }
#line 2352 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 953 "compilador.y" /* yacc.c:1646  */
    {
              if(compItem!=-1 && compItem!=INTEGER){
                  yyerror("Operação não permitida\n");
                }
              compItem=INTEGER;
              geraCodigo (NULL, "SUBT",NULL,NULL,NULL); 
              printf ("-"); }
#line 2364 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 960 "compilador.y" /* yacc.c:1646  */
    {
              if(compItem!=-1 && compItem!=BOOLEAN){
                  yyerror("Operação não permitida\n");
                }
              compItem=BOOLEAN;
              geraCodigo (NULL, "DISJ",NULL,NULL,NULL); 
              printf ("OR"); }
#line 2376 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 971 "compilador.y" /* yacc.c:1646  */
    {
               if(compItem!=-1 && compItem!=INTEGER){
                  yyerror("Operação não permitida\n");
                }
               compItem=INTEGER;
              geraCodigo (NULL, "MULT",NULL,NULL,NULL); 
              printf ("*"); }
#line 2388 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 978 "compilador.y" /* yacc.c:1646  */
    {
               if(compItem!=-1 && compItem!=INTEGER){
                  yyerror("Operação não permitida\n");
                }
               compItem=INTEGER;
              geraCodigo (NULL, "DIVI",NULL,NULL,NULL); 
              printf ("/"); }
#line 2400 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 985 "compilador.y" /* yacc.c:1646  */
    {
               if(compItem!=-1 && compItem!=BOOLEAN){
                  yyerror("Operação não permitida\n");
                }
               compItem=BOOLEAN;
              geraCodigo (NULL, "CONJ",NULL,NULL,NULL); 
              printf ("AND"); }
#line 2412 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 995 "compilador.y" /* yacc.c:1646  */
    {compItem=-1;}
#line 2418 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 997 "compilador.y" /* yacc.c:1646  */
    {
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
#line 2448 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1023 "compilador.y" /* yacc.c:1646  */
    {
                if(compItem!=-1 && compItem!=INTEGER){
                  yyerror("Operação não permitida");
                }
                compItem = INTEGER;
                geraCodigo (NULL, "CRCT",token,NULL,NULL); 
              }
#line 2460 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1030 "compilador.y" /* yacc.c:1646  */
    {
                if(compItem!=-1 && compItem!=BOOLEAN){
                  yyerror("Operação não permitida");
                }
                compItem = BOOLEAN;
                geraCodigo (NULL, "CRCT","1",NULL,NULL); 
              }
#line 2472 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1037 "compilador.y" /* yacc.c:1646  */
    {
                if(compItem!=-1 && compItem!=BOOLEAN){
                  yyerror("Operação não permitida");
                }
                compItem = BOOLEAN;
                geraCodigo(NULL, "CRCT","0",NULL,NULL); 
              }
#line 2484 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1048 "compilador.y" /* yacc.c:1646  */
    {define_comp("CMIG");}
#line 2490 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1049 "compilador.y" /* yacc.c:1646  */
    {define_comp("CMDG");}
#line 2496 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1050 "compilador.y" /* yacc.c:1646  */
    {define_comp("CMMA");}
#line 2502 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1051 "compilador.y" /* yacc.c:1646  */
    {define_comp("CMME");}
#line 2508 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1052 "compilador.y" /* yacc.c:1646  */
    {define_comp("CMAG");}
#line 2514 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1053 "compilador.y" /* yacc.c:1646  */
    {define_comp("CMEG");}
#line 2520 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1062 "compilador.y" /* yacc.c:1646  */
    {
                        geraCodigo (NULL, "IMPR",NULL,NULL,NULL);
                      }
#line 2528 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1076 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo(NULL,"LEIT",NULL,NULL,NULL);
              cria_valores_armz(VS);
              geraCodigo (NULL, "ARMZ",param1Aux,param2Aux,NULL);
            }
#line 2538 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1083 "compilador.y" /* yacc.c:1646  */
    { 
              geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
              lr->fim->decl=1;
              remove_item_lista(lr->fim->identificador);
            }
#line 2548 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1090 "compilador.y" /* yacc.c:1646  */
    {resultExpected=BOOLEAN; compItem=-1;}
#line 2554 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1090 "compilador.y" /* yacc.c:1646  */
    {
                if(resultExpected!=compItem){
                  yyerror("Operação não permitida, é necessario que o retorno da expressao do if seja booleano");
                }
                adiciona_item_lista();
                geraCodigo (NULL, "DSVF",lr->fim->identificador,NULL,NULL);
              }
#line 2566 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1100 "compilador.y" /* yacc.c:1646  */
    {
                define_else();
              }
#line 2574 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1115 "compilador.y" /* yacc.c:1646  */
    {
                adiciona_item_lista();
                geraCodigo (lr->fim->identificador, "NADA",NULL,NULL,NULL);
                lr->fim->decl=1;
              }
#line 2584 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1120 "compilador.y" /* yacc.c:1646  */
    {
                adiciona_item_lista();
                geraCodigo (NULL, "DSVF",lr->fim->identificador,NULL,NULL);
              }
#line 2593 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1123 "compilador.y" /* yacc.c:1646  */
    { geraCodigo (NULL, "DSVS",lr->fim->itemAnt->identificador,NULL,NULL);
                              remove_item_lista(lr->fim->itemAnt->identificador);
                              geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                              lr->fim->decl=1;
                              if(lr->fim->itemAnt != NULL)
                                lr->fim=lr->fim->itemAnt;
                        }
#line 2605 "compilador.tab.c" /* yacc.c:1646  */
    break;


#line 2609 "compilador.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1133 "compilador.y" /* yacc.c:1906  */


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

