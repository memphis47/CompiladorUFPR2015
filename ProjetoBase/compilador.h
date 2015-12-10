/* -------------------------------------------------------------------
 *            Arquivo: compilaodr.h
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [15/03/2012, 08h:22m]
 *
 * -------------------------------------------------------------------
 *
 * Tipos, prot�tipos e vai�veis globais do compilador
 *
 * ------------------------------------------------------------------- */

#define TAM_TOKEN 16

typedef enum simbolos { 
  simb_program, simb_var, simb_begin, simb_end, 
  simb_identificador, simb_numero, simb_write,
  simb_ponto, simb_virgula, simb_ponto_e_virgula, simb_dois_pontos,
  simb_atribuicao, simb_abre_parenteses, simb_fecha_parenteses,
  simb_soma, simb_sub, simb_multiplicacao, simb_div,
  simb_if, simb_then, simb_else,
  simb_maior, simb_maior_igual, simb_menor,
  simb_menor_igual, simb_igual, simb_diferente,
  simb_while, simb_do,
  simb_procedure

} simbolos;

typedef enum categorias {
  VS, // Variavel Simples 0
  PF, // Parametro formal 1
  FUN, // Funcao 2
  PROC, // Procedure 3
  PROG, // Program 4
  ROT, // Rotulo 5
} categorias;

typedef enum tipo_parametro {
  REFERENCIA, SIMPLES, VALOR
} tipo_parametro;

typedef enum tipo_variavel {
  INTEGER, BOOLEAN, CHAR
} tipo_variavel;

/* -------------------------------------------------------------------
 * vari�veis globais
 * ------------------------------------------------------------------- */

extern simbolos simbolo, relacao;
extern char token[TAM_TOKEN];
extern int nivel_lexico;
extern int desloc;
extern int nl;


simbolos simbolo, relacao;
char token[TAM_TOKEN];



