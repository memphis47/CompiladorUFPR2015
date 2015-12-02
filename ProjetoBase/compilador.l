%{


#include<stdio.h>
#include<string.h>

#include "compilador.h"
#include "compilador.tab.h"

//#define IMPRIME(STR) {};
#define IMPRIME(STR) printf("%s    # %s \n", STR, token);

int nl = 1;  

%}

pulo_linha [\n]

maior \>

maior_igual \>\=

menor \<

menor_igual \<\=

igual \=

diferente \<\>

descarte [\t ]+

ident [a-zAZ][a-zAZ1-9]*

numero [0-9]+

comentario "(*".*"*)"

%%

{pulo_linha} {nl++;} 

{descarte}

{comentario}

program { simbolo  = simb_program;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("program ");
          return PROGRAM;
 }

var      { simbolo = simb_var;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("var  ");
          return VAR;
 }

begin { simbolo = simb_begin;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("begin ");
          return T_BEGIN;
 }

end { simbolo = simb_end;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("end  ");
          return T_END;
 }

:= { simbolo = simb_atribuicao;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" atribuicao  ");
          return ATRIBUICAO;  
}

write {
     simbolo = simb_write;
     strncpy (token, yytext, TAM_TOKEN);
     IMPRIME(" write  ");
     return WRITE;  
}

while {
     simbolo = simb_write;
     strncpy (token, yytext, TAM_TOKEN);
     IMPRIME(" while  ");
     return WHILE;  
}

do {
     simbolo = simb_write;
     strncpy (token, yytext, TAM_TOKEN);
     IMPRIME(" do  ");
     return DO;  
}

procedure { simbolo  = simb_procedure;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("procedure ");
          return PROCEDURE;
}


\; { simbolo = simb_ponto_e_virgula;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" pt_virgula  ");
          return PONTO_E_VIRGULA;
 }

\: { simbolo = simb_dois_pontos;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" dois pontos ");
          return DOIS_PONTOS;
 }

\, { simbolo = simb_virgula;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" virgula  ");
          return VIRGULA;
 }


\. { simbolo = simb_ponto;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ponto  ");
          return PONTO; 
    }

\(  { simbolo = simb_abre_parenteses;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" abre_parenteses  ");
          return ABRE_PARENTESES;
    }

\)  { simbolo = simb_fecha_parenteses;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" fecha_parenteses ");
          return FECHA_PARENTESES;
 }


if   {   simbolo = simb_if;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" if ");
        return IF;
}

then   { simbolo = simb_then;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" then ");
        return THEN;

}

else   {  simbolo = simb_else;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" else ");
        return ELSE;

}

{maior}   {  simbolo = simb_maior;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" > ");
        return MAIOR;

}

{maior_igual}  {  
          simbolo = simb_maior_igual;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" >= ");
          return MAEG;

}

{menor}   {  simbolo = simb_menor;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" < ");
        return MENOR;

}

{menor_igual}  {  simbolo = simb_menor_igual;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" <= ");
        return MEEG;

}

{igual}  {  simbolo = simb_igual;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" == ");
        return IGUAL;

}

{diferente}  {  simbolo = simb_diferente;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" != ");
        return DIFE;

}

{ident} { simbolo = simb_identificador; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ident  ");
          return IDENT;
        }

{numero} { simbolo = simb_numero; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" numero  ");
          return NUMERO;
        }

\+    { simbolo = simb_soma;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" soma ");
        return SOMA; }

\-    { simbolo = simb_sub;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" subtracao ");
        return SUB; }

\*    { simbolo = simb_multiplicacao;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" multiplicacao ");
        return MUL;  }

\/    { simbolo = simb_div;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" divisao ");
        return DIV;  }



%%

