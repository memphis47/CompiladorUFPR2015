
%{
#include <stdio.h>
%}

%token IDENTA IDENTB MAIS MENOS AND OR ASTERISCO DIV ABRE_PARENTESES FECHA_PARENTESES

%%

expr       : expr MAIS termo {printf ("+"); } |
             expr MENOS termo {printf ("-"); } | 
             termo
;

termo      : termo ASTERISCO fator  {printf ("*"); }| 
             termo AND fator  {printf ("&&"); }| 
             termo OR fator  {printf ("||"); }| 
             termo DIV fator  {printf ("/"); }|
             fator
;

fator      : IDENTA {printf ("A"); } |
             IDENTB {printf ("B"); }   
;

%%

main (int argc, char** argv) {
   yyparse();
   printf("\n");
}

