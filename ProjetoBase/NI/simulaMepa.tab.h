/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

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

#ifndef YY_YY_SIMULAMEPA_TAB_H_INCLUDED
# define YY_YY_SIMULAMEPA_TAB_H_INCLUDED
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
    DOIS_PONTOS = 258,
    ROTULO = 259,
    INTEIRO = 260,
    VIRGULA = 261,
    INPP = 262,
    PARA = 263,
    SOMA = 264,
    SUBT = 265,
    MULT = 266,
    DIVI = 267,
    INVR = 268,
    CONJ = 269,
    DISJ = 270,
    NEGA = 271,
    CMME = 272,
    CMMA = 273,
    CMIG = 274,
    CMDG = 275,
    CMEG = 276,
    CMAG = 277,
    NADA = 278,
    LEIT = 279,
    IMPR = 280,
    CRCT = 281,
    AMEM = 282,
    DMEM = 283,
    ENPR = 284,
    ENRT = 285,
    DSVS = 286,
    DSVF = 287,
    CRVL = 288,
    ARMZ = 289,
    CRVI = 290,
    ARMI = 291,
    CREN = 292,
    CHPR = 293,
    RTPR = 294,
    DSVR = 295
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

#endif /* !YY_YY_SIMULAMEPA_TAB_H_INCLUDED  */
