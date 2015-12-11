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


#line 447 "compilador.tab.c" /* yacc.c:339  */

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
    LOWER_THAN_ELSE = 289
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

#line 533 "compilador.tab.c" /* yacc.c:358  */

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
#define YYLAST   128

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  66
/* YYNRULES -- Number of rules.  */
#define YYNRULES  101
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  166

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   401,   401,   408,   401,   416,   416,   423,   428,   423,
     434,   440,   440,   444,   448,   449,   452,   460,   452,   479,
     477,   488,   494,   500,   500,   504,   504,   506,   506,   511,
     511,   521,   521,   521,   523,   526,   530,   534,   535,   541,
     539,   547,   552,   558,   559,   564,   566,   566,   566,   570,
     569,   582,   583,   584,   590,   590,   604,   608,   618,   608,
     630,   640,   644,   644,   647,   651,   655,   658,   661,   665,
     668,   671,   674,   675,   675,   702,   707,   707,   710,   714,
     721,   721,   728,   728,   733,   734,   739,   739,   742,   744,
     748,   744,   759,   759,   761,   762,   763,   766,   767,   768,
     769,   770
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
  "LOWER_THAN_ELSE", "$accept", "programa", "$@1", "$@2", "bloco", "$@3",
  "parse_bloco", "$@4", "$@5", "proc_func_def_loop", "proc_func_def",
  "proc_func_def_choose", "proc_com", "$@6", "$@7", "parse_proc_decl",
  "$@8", "func_type", "param_loop", "declara_param", "$@9", "$@10",
  "parse_declaracao", "$@11", "blocoProc", "$@12", "$@13",
  "parte_declara_vars", "var", "declara_vars", "declara_var", "$@14",
  "lista_id_var", "lista_idents", "comando_composto",
  "comando_composto_loop", "comando", "$@15", "parse_comando", "$@16",
  "procedure_function_call", "$@17", "$@18", "le_params", "get_param_loop",
  "$@19", "get_param", "expressao_simples", "termo", "fator", "$@20",
  "comando_write", "$@21", "cond_if", "if_then", "$@22", "cond_else",
  "$@23", "internal", "comando_repetitivo", "com_while", "$@24", "$@25",
  "expressao", "prior2", "prior1", YY_NULLPTR
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
     285,   286,   287,   288,   289
};
# endif

#define YYPACT_NINF -137

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-137)))

#define YYTABLE_NINF -90

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -137,     8,    35,  -137,    -3,  -137,    57,    50,  -137,     5,
      59,    56,    67,  -137,    68,    71,  -137,  -137,  -137,    68,
    -137,    23,  -137,    72,  -137,    70,    73,    29,  -137,   -17,
    -137,  -137,  -137,    81,  -137,    30,    78,  -137,  -137,    10,
    -137,  -137,    64,  -137,  -137,  -137,   -17,    75,    82,    33,
      47,    44,  -137,  -137,    48,    49,  -137,  -137,    45,    25,
       7,    79,  -137,  -137,  -137,    30,    72,  -137,  -137,  -137,
    -137,    44,    87,    48,  -137,  -137,  -137,    89,     2,    -1,
      90,    44,    44,    44,    44,    74,    44,    44,    44,    44,
      44,    44,    78,  -137,     3,  -137,  -137,    91,     2,  -137,
      92,    44,  -137,  -137,  -137,  -137,    49,    49,  -137,  -137,
       3,    25,    25,    48,    48,    48,    48,  -137,  -137,  -137,
    -137,    76,    85,    94,  -137,    48,    44,  -137,     3,  -137,
      95,    86,  -137,   100,    99,    48,  -137,    86,    85,   102,
    -137,  -137,    46,  -137,  -137,  -137,  -137,    95,  -137,   101,
    -137,   103,    67,    44,  -137,   104,    98,  -137,  -137,  -137,
     105,  -137,  -137,    72,  -137,  -137
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,     3,     0,     0,    44,     0,
       0,     0,     0,    43,     0,     0,     5,    35,    42,    36,
      38,     0,     4,     7,    37,     0,     0,    89,     6,    12,
      10,    41,    39,     0,    49,     0,    48,    51,    52,    84,
      53,    88,     0,    14,    15,     8,    12,     0,     0,     0,
      60,     0,    73,    75,   101,    68,    71,    80,    93,    96,
      47,     0,    85,    82,    79,     0,     0,    11,    16,    13,
      40,     0,    75,    76,    54,    50,    56,     0,   101,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    45,    89,    90,     9,     0,     0,    78,
       0,     0,    58,    72,    92,    74,    66,    67,    69,    70,
      89,    95,    94,    99,    97,    98,   100,    46,    87,    86,
      83,     0,    27,     0,    77,    55,     0,    81,    89,    25,
      24,     0,    17,     0,    64,    65,    91,     0,    27,     0,
      29,    28,     7,    59,    62,    61,    26,    24,    19,     0,
      34,     0,     0,     0,    23,    22,     0,    18,    32,    63,
       0,    20,    30,     7,    21,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -137,  -137,  -137,  -137,  -137,  -137,  -136,  -137,  -137,    53,
    -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,   -39,   -29,
    -137,  -137,   -23,  -137,  -137,  -137,  -137,   -37,  -137,  -137,
      97,  -137,  -137,  -137,   -65,    26,   -24,  -137,  -137,  -137,
      37,  -137,  -137,   -33,  -137,  -137,  -137,   -49,    -8,    -5,
    -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -105,  -137,
    -137,  -137,  -137,    58,    77,   -11
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     6,    15,    23,    28,    29,    66,    45,
      46,    47,    69,    97,   142,   123,   155,   161,   139,   130,
     137,   131,   141,   149,   151,   152,   163,    16,    17,    19,
      20,    48,    21,     9,    30,    61,   119,    50,    75,   101,
      76,    77,   126,   133,   145,   153,   134,    54,    55,    56,
      80,    37,   100,    38,    39,    85,    64,    94,   120,    40,
      41,    42,   121,    57,    58,    59
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      73,    96,    78,    36,   104,   127,   150,   103,     3,    33,
      10,     5,    11,    33,    27,    43,    44,    34,    62,    81,
      82,    34,    98,   136,    35,    86,    87,   165,    35,   118,
      25,   -89,    26,    63,    51,    33,    92,    71,     4,   113,
     114,   115,   116,    34,    52,   118,    53,    52,    71,    72,
      35,   -57,   125,    88,    89,    90,    91,    27,    52,   -31,
      53,     7,    74,   118,     8,    81,    82,    12,    83,    84,
      13,    86,    87,   106,   107,   111,   112,   135,   108,   109,
      14,    22,    18,    27,    31,    49,    60,    32,    65,    68,
      70,    93,    99,   102,   -57,   122,   110,   124,   129,    67,
     140,   128,   132,   138,   135,   143,   144,   148,   154,   147,
     156,   157,   162,   160,   146,   158,    24,   105,   117,   164,
     159,     0,     0,    95,     0,     0,     0,     0,    79
};

static const yytype_int16 yycheck[] =
{
      49,    66,    51,    27,     5,   110,   142,     5,     0,     6,
       5,    14,     7,     6,    11,    32,    33,    14,     8,    17,
      18,    14,    71,   128,    21,    26,    27,   163,    21,    94,
       7,    24,     9,    23,     4,     6,    60,     4,     3,    88,
      89,    90,    91,    14,    14,   110,    16,    14,     4,    16,
      21,     4,   101,    28,    29,    30,    31,    11,    14,    13,
      16,     4,    15,   128,    14,    17,    18,     8,    19,    20,
      14,    26,    27,    81,    82,    86,    87,   126,    83,    84,
      13,    10,    14,    11,    14,     4,     8,    14,    24,    14,
       8,    12,     5,     4,     4,     4,    22,     5,    13,    46,
      14,    25,     8,     8,   153,     5,     7,     5,   147,   138,
       9,     8,    14,     9,   137,   152,    19,    80,    92,    14,
     153,    -1,    -1,    65,    -1,    -1,    -1,    -1,    51
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    36,    37,     0,     3,    14,    38,     4,    14,    68,
       5,     7,     8,    14,    13,    39,    62,    63,    14,    64,
      65,    67,    10,    40,    65,     7,     9,    11,    41,    42,
      69,    14,    14,     6,    14,    21,    71,    86,    88,    89,
      94,    95,    96,    32,    33,    44,    45,    46,    66,     4,
      72,     4,    14,    16,    82,    83,    84,    98,    99,   100,
       8,    70,     8,    23,    91,    24,    43,    44,    14,    47,
       8,     4,    16,    82,    15,    73,    75,    76,    82,    99,
      85,    17,    18,    19,    20,    90,    26,    27,    28,    29,
      30,    31,    71,    12,    92,    98,    69,    48,    82,     5,
      87,    74,     4,     5,     5,    75,    83,    83,    84,    84,
      22,   100,   100,    82,    82,    82,    82,    70,    69,    71,
      93,    97,     4,    50,     5,    82,    77,    93,    25,    13,
      54,    56,     8,    78,    81,    82,    93,    55,     8,    53,
      14,    57,    49,     5,     7,    79,    57,    54,     5,    58,
      41,    59,    60,    80,    53,    51,     9,     8,    62,    78,
       9,    52,    14,    61,    14,    41
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    35,    37,    38,    36,    40,    39,    42,    43,    41,
      41,    44,    44,    45,    46,    46,    48,    49,    47,    51,
      50,    52,    52,    53,    53,    55,    54,    56,    54,    58,
      57,    60,    61,    59,    59,    62,    63,    64,    64,    66,
      65,    67,    67,    68,    68,    69,    70,    70,    70,    72,
      71,    71,    71,    71,    74,    73,    73,    76,    77,    75,
      75,    78,    80,    79,    79,    81,    82,    82,    82,    83,
      83,    83,    84,    85,    84,    84,    87,    86,    86,    88,
      90,    89,    92,    91,    91,    91,    93,    93,    94,    96,
      97,    95,    98,    98,    99,    99,    99,   100,   100,   100,
     100,   100
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,    10,     0,     3,     0,     0,     4,
       1,     2,     0,     2,     1,     1,     0,     0,     7,     0,
       6,     2,     0,     3,     0,     0,     3,     0,     2,     0,
       4,     0,     0,     4,     1,     1,     2,     2,     1,     0,
       5,     3,     1,     3,     1,     4,     3,     1,     0,     0,
       3,     1,     1,     1,     0,     3,     1,     0,     0,     5,
       0,     2,     0,     3,     0,     1,     3,     3,     1,     3,
       3,     1,     3,     0,     3,     1,     0,     5,     4,     2,
       0,     5,     0,     3,     0,     1,     1,     1,     1,     0,
       0,     6,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1
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
#line 401 "compilador.y" /* yacc.c:1646  */
    { 
                nivel_lexico_atual=0;
                num_vars=0;
                desl=0;
                nivel=0;
                geraCodigo (NULL, "INPP",NULL,NULL,NULL); 
             }
#line 1740 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 408 "compilador.y" /* yacc.c:1646  */
    {adiciona_ts(PROG,NULL);}
#line 1746 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 410 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "PARA",NULL,NULL,NULL); 
             }
#line 1754 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 416 "compilador.y" /* yacc.c:1646  */
    { 
                reza();
              }
#line 1762 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 423 "compilador.y" /* yacc.c:1646  */
    {
              adiciona_item_lista();
              geraCodigo (NULL, "DSVS",lr->fim->identificador,NULL,NULL);
            }
#line 1771 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 428 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
            }
#line 1779 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 432 "compilador.y" /* yacc.c:1646  */
    { doDmem(); }
#line 1785 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 434 "compilador.y" /* yacc.c:1646  */
    { doDmem(); }
#line 1791 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 448 "compilador.y" /* yacc.c:1646  */
    {tempCategoria=PROC;}
#line 1797 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 449 "compilador.y" /* yacc.c:1646  */
    {tempCategoria=FUN;}
#line 1803 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 452 "compilador.y" /* yacc.c:1646  */
    {
            nivel_lexico_atual++;
            free(rots);
            rots=(char *) malloc (5* sizeof(char));
            rots=retornaRotulo(); 
            adiciona_ts(tempCategoria,rots);
          }
#line 1815 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 460 "compilador.y" /* yacc.c:1646  */
    {
            char lexico[4];
            sprintf(lexico, "%d", nivel_lexico_atual);
            geraCodigo (rots, "ENPR",lexico,NULL,NULL); 
            adiciona_item_lista();
          }
#line 1826 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 468 "compilador.y" /* yacc.c:1646  */
    {
            remove_item_lista(lr->fim->identificador);
            char lexico[4];
            sprintf(lexico, "%d", nivel_lexico_atual);
            geraCodigo (NULL, "RTPR",lexico,"0",NULL); 
            nivel_lexico_atual--;
          }
#line 1838 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 479 "compilador.y" /* yacc.c:1646  */
    {
                    procedure->param=(lista_params *) malloc(sizeof(lista_params));
                    memcpy(procedure->param,lp,sizeof(lista_params)); //Procedure definido com os par
                    adiciona_deslocamento_param();
                 }
#line 1848 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 488 "compilador.y" /* yacc.c:1646  */
    {
            tipo_variavel tipo= retornaTipo();
            procedure->tipo = tipo;
            lp->inicio=NULL;
            lp->fim=NULL;
          }
#line 1859 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 494 "compilador.y" /* yacc.c:1646  */
    {
            lp->inicio=NULL;
            lp->fim=NULL;
          }
#line 1868 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 504 "compilador.y" /* yacc.c:1646  */
    {passado=REFERENCIA;}
#line 1874 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 506 "compilador.y" /* yacc.c:1646  */
    {passado=VALOR;}
#line 1880 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 511 "compilador.y" /* yacc.c:1646  */
    {
              free(paramName);
              paramName = (char *) malloc (256 * sizeof(char));
              strcpy(paramName,token);
            }
#line 1890 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 517 "compilador.y" /* yacc.c:1646  */
    {adiciona_param_ts(PF,NULL,passado);}
#line 1896 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 521 "compilador.y" /* yacc.c:1646  */
    {desl=0;}
#line 1902 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 521 "compilador.y" /* yacc.c:1646  */
    { reza(); }
#line 1908 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 541 "compilador.y" /* yacc.c:1646  */
    { 
                adiciona_tipo_ts(num_vars);
              }
#line 1916 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 548 "compilador.y" /* yacc.c:1646  */
    { /* insere última vars na tabela de símbolos */ 
               adiciona_ts(VS,NULL);
               num_vars++;
              }
#line 1925 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 552 "compilador.y" /* yacc.c:1646  */
    { /* insere vars na tabela de símbolos */
                adiciona_ts(VS,NULL);
                num_vars++;
              }
#line 1934 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 570 "compilador.y" /* yacc.c:1646  */
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
#line 1950 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 590 "compilador.y" /* yacc.c:1646  */
    {
            if(procedure!=NULL && !no_proc){
              yyerror("Tipo de operação nao permitida");
            }
          }
#line 1960 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 595 "compilador.y" /* yacc.c:1646  */
    { 
            compItem = -1;
            if(tempItemArmz->passagem == REFERENCIA)
              geraCodigo (NULL, "ARMI",param1Aux,param2Aux,NULL);
            else{
              geraCodigo (NULL, "ARMZ",param1Aux,param2Aux,NULL);
            }
          }
#line 1973 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 608 "compilador.y" /* yacc.c:1646  */
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
#line 1988 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 618 "compilador.y" /* yacc.c:1646  */
    {
            n_param_atual=0;
            param_chamada=1;
            temp=(lista_params *) malloc(sizeof(lista_params));
            memcpy(temp,procedure->param,sizeof(lista_params));
          }
#line 1999 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 624 "compilador.y" /* yacc.c:1646  */
    {
            free(temp);
            param_chamada=0;
            verifica_chama_proc();
          }
#line 2009 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 630 "compilador.y" /* yacc.c:1646  */
    { 
            
            if(procedure!=NULL && do_verify){
              n_param_atual=0;
              verifica_chama_proc();
            }
          }
#line 2021 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 644 "compilador.y" /* yacc.c:1646  */
    {
            temp->inicio=temp->inicio->itemProx;
          }
#line 2029 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 651 "compilador.y" /* yacc.c:1646  */
    {n_param_atual++;}
#line 2035 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 655 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "SOMA",NULL,NULL,NULL); 
              printf ("+\n"); }
#line 2043 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 658 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "SUBT",NULL,NULL,NULL); 
              printf ("-"); }
#line 2051 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 665 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "MULT",NULL,NULL,NULL); 
              printf ("*"); }
#line 2059 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 668 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "DIVI",NULL,NULL,NULL); 
              printf ("/"); }
#line 2067 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 675 "compilador.y" /* yacc.c:1646  */
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
#line 2098 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 702 "compilador.y" /* yacc.c:1646  */
    {
                geraCodigo (NULL, "CRCT",token,NULL,NULL); 
              }
#line 2106 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 707 "compilador.y" /* yacc.c:1646  */
    {
                  carrega_valor_imprime();
                }
#line 2114 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 715 "compilador.y" /* yacc.c:1646  */
    { 
               geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                lr->fim=lr->fim->itemAnt;
            }
#line 2123 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 721 "compilador.y" /* yacc.c:1646  */
    {
                adiciona_item_lista();
                geraCodigo (NULL, "DSVF",lr->fim->identificador,NULL,NULL);
              }
#line 2132 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 728 "compilador.y" /* yacc.c:1646  */
    {
                define_else();
              }
#line 2140 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 744 "compilador.y" /* yacc.c:1646  */
    {
                adiciona_item_lista();
                geraCodigo (lr->fim->identificador, "NADA",NULL,NULL,NULL);
              }
#line 2149 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 748 "compilador.y" /* yacc.c:1646  */
    {
                adiciona_item_lista();
                geraCodigo (NULL, "DSVF",lr->fim->identificador,NULL,NULL);
              }
#line 2158 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 751 "compilador.y" /* yacc.c:1646  */
    { geraCodigo (NULL, "DSVS",lr->fim->itemAnt->identificador,NULL,NULL);
                              remove_item_lista(lr->fim->itemAnt->identificador);
                              geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                              if(lr->fim->itemAnt != NULL)
                                lr->fim=lr->fim->itemAnt;
                        }
#line 2169 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 761 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMIG",NULL,NULL,NULL);}
#line 2175 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 762 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMDG",NULL,NULL,NULL);}
#line 2181 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 766 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMMA",NULL,NULL,NULL);}
#line 2187 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 767 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMME",NULL,NULL,NULL);}
#line 2193 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 768 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMAG",NULL,NULL,NULL);}
#line 2199 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 769 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMEG",NULL,NULL,NULL);}
#line 2205 "compilador.tab.c" /* yacc.c:1646  */
    break;


#line 2209 "compilador.tab.c" /* yacc.c:1646  */
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
#line 773 "compilador.y" /* yacc.c:1906  */


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

