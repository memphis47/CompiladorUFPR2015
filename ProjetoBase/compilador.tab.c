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
    if(lr != NULL){
      if(lr->fim == NULL)
        printf("NULL\n");
      else{
        printf("Não esta null\n");
      }
      if(lr->inicio == NULL){
        printf("Tbm NULL");
      }
      else
        printf("%s\n",lr->inicio->identificador);
    }
    lr->fim->itemProx = auxItem;
    auxItem->itemAnt = lr->fim;
    lr->fim = auxItem;
  }
  rotNumber++;
}

void remove_item_lista(char *token){
  itemLista *itemAtual = lr->fim;
  printf("%s\n",lr->fim->identificador );
  while(itemAtual != NULL){
    if(strcmp (token, itemAtual->identificador) == 0){
      if(itemAtual->itemAnt!=NULL)
        itemAtual->itemAnt->itemProx = itemAtual->itemProx;
      else{
        lr->inicio = itemAtual->itemProx;
      }
      if(itemAtual->itemProx!=NULL){
        printf("Entrei Aqui\n");
        itemAtual->itemProx->itemAnt = itemAtual->itemAnt;

      }
      else{
        printf("Entrei Acola\n");
        lr->fim = itemAtual->itemAnt;
      }
    }
    itemAtual = itemAtual->itemAnt;
  }
}


#line 196 "compilador.tab.c" /* yacc.c:339  */

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
    LOWER_THAN_ELSE = 287
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

#line 280 "compilador.tab.c" /* yacc.c:358  */

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
#define YYLAST   94

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  63
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  113

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

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
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   149,   149,   149,   163,   162,   179,   183,   183,   185,
     189,   190,   193,   196,   193,   202,   205,   223,   242,   243,
     248,   251,   254,   254,   257,   257,   272,   273,   274,   279,
     282,   285,   289,   292,   295,   298,   299,   302,   307,   307,
     321,   324,   331,   331,   338,   338,   352,   355,   355,   358,
     360,   364,   360,   373,   375,   376,   377,   379,   380,   381,
     382,   383,   385,   386
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
  "IGUAL", "MAEG", "MAIOR", "MENOR", "MEEG", "LOWER_THAN_ELSE", "$accept",
  "programa", "$@1", "bloco", "$@2", "parte_declara_vars", "var", "$@3",
  "declara_vars", "declara_var", "$@4", "$@5", "tipo", "lista_id_var",
  "lista_idents", "comando_composto", "comandos_total", "comandos",
  "comando", "$@6", "expressao_simples", "termo", "fator", "comando_write",
  "$@7", "cond_if", "if_then", "$@8", "cond_else", "$@9", "internal",
  "comando_repetitivo", "com_while", "$@10", "$@11", "expressao", "prior2",
  "prior1", "final", YY_NULLPTR
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
     285,   286,   287
};
# endif

#define YYPACT_NINF -79

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-79)))

#define YYTABLE_NINF -51

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -79,     7,     9,   -79,    25,    11,    28,   -79,     1,    24,
      44,    15,   -79,    54,   -79,   -79,    52,   -79,    55,   -79,
      22,   -79,    53,   -79,    56,    64,   -79,    65,    59,    -3,
     -79,   -79,   -79,    49,   -79,   -79,    50,   -79,   -79,    26,
      38,    58,    39,   -79,   -79,   -79,   -79,   -79,    65,    61,
      62,   -79,    72,     0,   -79,   -79,    -4,    16,   -79,    57,
      -1,   -79,   -79,   -79,   -79,    73,    74,     0,   -79,   -79,
      23,    18,   -79,   -79,    39,    39,    39,    39,    39,    39,
      -1,    22,   -79,   -79,    60,    75,    76,   -79,    12,   -79,
       0,     0,     0,     0,    16,    16,   -79,   -79,   -79,   -79,
     -79,    13,    -1,   -79,   -79,   -79,    18,    18,   -79,   -79,
      78,   -79,   -79
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     0,    19,     0,     0,
       0,     7,    18,     0,     4,     6,     0,     3,     0,    12,
      50,     5,     8,    11,     0,     0,    24,     0,     0,    21,
      23,    26,    27,    46,    28,    49,     0,    10,    17,     0,
       0,     0,     0,    42,    20,    22,    44,    41,     0,     0,
       0,    38,     0,     0,    62,    63,     0,    56,    61,     0,
      50,    51,    16,    15,    13,     0,     0,     0,    36,    37,
       0,    31,    34,    53,     0,     0,     0,     0,     0,     0,
      50,    50,    47,    45,     0,     0,     0,    40,     0,    25,
       0,     0,     0,     0,    55,    54,    59,    57,    58,    60,
      43,    50,    50,    14,    39,    35,    29,    30,    32,    33,
       0,    52,    48
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,    66,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,     6,   -20,   -79,
      27,   -34,   -33,   -79,   -79,   -79,   -79,   -79,   -79,   -79,
     -78,   -79,   -79,   -79,   -79,    32,   -79,   -12,   -28
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    13,    18,    14,    15,    16,    22,    23,
      24,    85,    64,    39,     8,    21,    28,    29,    82,    41,
      70,    71,    72,    31,    65,    32,    33,    59,    47,    60,
      83,    34,    35,    36,    84,    43,    56,    57,    58
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      30,    73,   100,    25,    67,    25,     9,     3,    10,    45,
      81,    26,     4,    26,    68,     6,    69,   105,    27,    25,
      27,   -50,    74,    75,   111,   110,    -9,    26,    25,    90,
      91,    89,    11,    49,    27,    50,    26,    92,    93,     5,
      90,    91,     7,    27,    76,    77,    78,    79,    96,    97,
      98,    99,    51,    54,    52,    55,   106,   107,    12,   108,
     109,    30,    94,    95,    17,    19,    20,   -12,    40,    42,
      38,    44,    46,    53,    48,    62,    63,    66,    86,    80,
      61,    45,    87,   103,   104,   102,   112,   101,    37,     0,
       0,     0,     0,     0,    88
};

static const yytype_int8 yycheck[] =
{
      20,     5,    80,     6,     4,     6,     5,     0,     7,    29,
      11,    14,     3,    14,    14,     4,    16,     5,    21,     6,
      21,    24,    26,    27,   102,    12,    11,    14,     6,    17,
      18,     8,     8,     7,    21,     9,    14,    19,    20,    14,
      17,    18,    14,    21,    28,    29,    30,    31,    76,    77,
      78,    79,    14,    14,    16,    16,    90,    91,    14,    92,
      93,    81,    74,    75,    10,    13,    11,    14,     4,     4,
      14,    12,    23,    15,    24,    14,    14,     5,     5,    22,
      48,   101,     8,     8,     8,    25,     8,    81,    22,    -1,
      -1,    -1,    -1,    -1,    67
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    34,    35,     0,     3,    14,     4,    14,    47,     5,
       7,     8,    14,    36,    38,    39,    40,    10,    37,    13,
      11,    48,    41,    42,    43,     6,    14,    21,    49,    50,
      51,    56,    58,    59,    64,    65,    66,    42,    14,    46,
       4,    52,     4,    68,    12,    51,    23,    61,    24,     7,
       9,    14,    16,    15,    14,    16,    69,    70,    71,    60,
      62,    68,    14,    14,    45,    57,     5,     4,    14,    16,
      53,    54,    55,     5,    26,    27,    28,    29,    30,    31,
      22,    11,    51,    63,    67,    44,     5,     8,    53,     8,
      17,    18,    19,    20,    70,    70,    71,    71,    71,    71,
      63,    50,    25,     8,     8,     5,    54,    54,    55,    55,
      12,    63,     8
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    35,    34,    37,    36,    38,    40,    39,    39,
      41,    41,    43,    44,    42,    45,    46,    46,    47,    47,
      48,    49,    50,    50,    52,    51,    51,    51,    51,    53,
      53,    53,    54,    54,    54,    55,    55,    55,    57,    56,
      56,    58,    60,    59,    62,    61,    61,    63,    63,    64,
      66,    67,    65,    68,    69,    69,    69,    70,    70,    70,
      70,    70,    71,    71
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     9,     0,     3,     1,     0,     3,     0,
       2,     1,     0,     0,     6,     1,     3,     1,     3,     1,
       3,     1,     2,     1,     0,     5,     1,     1,     1,     3,
       3,     1,     3,     3,     1,     3,     1,     1,     0,     6,
       5,     2,     0,     5,     0,     3,     0,     1,     4,     1,
       0,     0,     6,     3,     3,     3,     1,     3,     3,     3,
       3,     1,     1,     1
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
#line 149 "compilador.y" /* yacc.c:1646  */
    { 
                num_vars=0;
                desl=0;
                geraCodigo (NULL, "INPP",NULL,NULL,NULL); 
             }
#line 1440 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 156 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "PARA",NULL,NULL,NULL); 
             }
#line 1448 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 163 "compilador.y" /* yacc.c:1646  */
    { 
                free(param1);
                free(param2);
                free(param3);
                param1 = (char *) malloc (4 * sizeof(char));
                sprintf(param1, "%d", num_vars);
                num_vars=0;
                geraCodigo (NULL, "AMEM",param1,NULL,NULL); 
              }
#line 1462 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 183 "compilador.y" /* yacc.c:1646  */
    { }
#line 1468 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 183 "compilador.y" /* yacc.c:1646  */
    {
                }
#line 1475 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 193 "compilador.y" /* yacc.c:1646  */
    { }
#line 1481 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 196 "compilador.y" /* yacc.c:1646  */
    { /* AMEM */
               
              }
#line 1489 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 206 "compilador.y" /* yacc.c:1646  */
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
#line 1511 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 223 "compilador.y" /* yacc.c:1646  */
    { /* insere vars na tabela de símbolos */
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
#line 1533 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 257 "compilador.y" /* yacc.c:1646  */
    {
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
          }
#line 1551 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 269 "compilador.y" /* yacc.c:1646  */
    {
            geraCodigo (NULL, "ARMZ",param1Aux,param2Aux,NULL);
          }
#line 1559 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 279 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "SOMA",NULL,NULL,NULL); 
              printf ("+\n"); }
#line 1567 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 282 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "SUBT",NULL,NULL,NULL); 
              printf ("-"); }
#line 1575 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 289 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "MULT",NULL,NULL,NULL); 
              printf ("*"); }
#line 1583 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 292 "compilador.y" /* yacc.c:1646  */
    {
              geraCodigo (NULL, "DIVI",NULL,NULL,NULL); 
              printf ("/"); }
#line 1591 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 299 "compilador.y" /* yacc.c:1646  */
    {
                gera_comando_crvl(token);
              }
#line 1599 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 302 "compilador.y" /* yacc.c:1646  */
    {
                geraCodigo (NULL, "CRCT",token,NULL,NULL); 
              }
#line 1607 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 307 "compilador.y" /* yacc.c:1646  */
    {
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
                }
#line 1626 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 325 "compilador.y" /* yacc.c:1646  */
    { 
               geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                lr->fim=lr->fim->itemAnt;
            }
#line 1635 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 331 "compilador.y" /* yacc.c:1646  */
    {
                adiciona_item_lista();
                geraCodigo (NULL, "DSVF",lr->fim->identificador,NULL,NULL);
              }
#line 1644 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 338 "compilador.y" /* yacc.c:1646  */
    {
                char rtn[4];
                sprintf(rtn, "%d", rotNumber);
                char rot[]="R";
                strcat(rot, rtn);
                char *strRot = (char *) malloc (256 * sizeof(char));
                strcpy(strRot,rot);
                
                geraCodigo (NULL, "DSVS",strRot,NULL,NULL);
                geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                if(lr->fim->itemAnt != NULL)
                  lr->fim=lr->fim->itemAnt;
                adiciona_item_lista();
              }
#line 1663 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 360 "compilador.y" /* yacc.c:1646  */
    {
                adiciona_item_lista();
                geraCodigo (lr->fim->identificador, "NADA",NULL,NULL,NULL);
              }
#line 1672 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 364 "compilador.y" /* yacc.c:1646  */
    {
                adiciona_item_lista();
                geraCodigo (NULL, "DSVF",lr->fim->identificador,NULL,NULL);
              }
#line 1681 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 367 "compilador.y" /* yacc.c:1646  */
    { geraCodigo (NULL, "DSVS",lr->fim->itemAnt->identificador,NULL,NULL);
                              remove_item_lista(lr->fim->itemAnt->identificador);
                              geraCodigo (lr->fim->identificador, "NADA",NULL,NULL);
                                if(lr->fim->itemAnt != NULL)
                              lr->fim=lr->fim->itemAnt;}
#line 1691 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 375 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMIG",NULL,NULL,NULL);}
#line 1697 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 376 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMDG",NULL,NULL,NULL);}
#line 1703 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 379 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMMA",NULL,NULL,NULL);}
#line 1709 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 380 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMME",NULL,NULL,NULL);}
#line 1715 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 381 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMAG",NULL,NULL,NULL);}
#line 1721 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 382 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CMEG",NULL,NULL,NULL);}
#line 1727 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 385 "compilador.y" /* yacc.c:1646  */
    {gera_comando_crvl(token);}
#line 1733 "compilador.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 386 "compilador.y" /* yacc.c:1646  */
    {geraCodigo (NULL, "CRCT",token,NULL,NULL);}
#line 1739 "compilador.tab.c" /* yacc.c:1646  */
    break;


#line 1743 "compilador.tab.c" /* yacc.c:1646  */
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
#line 388 "compilador.y" /* yacc.c:1906  */


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

