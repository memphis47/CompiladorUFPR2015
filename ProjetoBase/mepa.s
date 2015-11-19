
# *****************************************************************
# INICIO macros
# *****************************************************************


# -----------------------------------------------------------------
#  INPP
# -----------------------------------------------------------------

.macro INPP
   movq %rsp, %rax
   movq $0, %rdi
   movq %rax, D(,%rdi,4)
.endm

# -----------------------------------------------------------------
#  PARA
# -----------------------------------------------------------------

.macro PARA
   movq $FIM_PGMA, %rax
   int  $SYSCALL
.endm

# -----------------------------------------------------------------
#  AMEM
# -----------------------------------------------------------------

.macro AMEM mem
   movq $\mem, %rax
   movq $4, %rbx
   imul %rbx, %rax
   sub %rax, %rsp
.endm

# -----------------------------------------------------------------
#  DMEM
# -----------------------------------------------------------------

.macro DMEM mem
   movq $\mem, %rax
   movq $4, %rbx
   imul %rbx, %rax
   add %rax, %rsp
.endm


# -----------------------------------------------------------------
#  CRCT
# -----------------------------------------------------------------

.macro CRCT k
   push $\k
.endm



# -----------------------------------------------------------------
#  CRVL
# -----------------------------------------------------------------

.macro CRVL m n
   movq $\m, %rdi
   movq D(,%rdi,4), %rax
   movq $\n, %rbx
   imul $4, %rbx
   sub %rbx, %rax
   movq (%rax), %rax
   push %rax
.endm

# -----------------------------------------------------------------
#  ARMZ
# -----------------------------------------------------------------

.macro ARMZ m n
   pop %rcx
   movq $\m, %rdi
   movq D(,%rdi,4), %rax
   movq $\n, %rbx
   imul $4, %rbx
   sub %rbx, %rax
   movq %rcx, (%rax)

.endm


# -----------------------------------------------------------------
#  CREN
# -----------------------------------------------------------------

.macro CREN m n
   movq $\m, %rdi
   movq D(,%rdi,4), %rax
   movq $\n, %rbx
   imul $4, %rbx
   sub %rbx, %rax
   push %rax
.endm

# -----------------------------------------------------------------
#  CRVI
# -----------------------------------------------------------------

.macro CRVI m n
   movq $\m, %rdi
   movq D(,%rdi,4), %rax
   movq $\n, %rbx
   imul $4, %rbx
   sub %rbx, %rax
   movq (%rax), %rax
   movq (%rax), %rax
   push %rax
.endm

# -----------------------------------------------------------------
#  ARMI
# -----------------------------------------------------------------

.macro ARMI m n
   pop %rcx
   movq $\m, %rdi
   movq D(,%rdi,4), %rax
   movq $\n, %rbx
   imul $4, %rbx
   sub %rbx, %rax
   movq (%rax), %rax
   movq %rcx, (%rax)
.endm

# -----------------------------------------------------------------
#  ENRT
# -----------------------------------------------------------------

.macro ENRT j n
   movq $\n, %rax
   sub $1, %rax
   imul $4, %rax
   movq $\j, %rdi
   movq D(,%rdi,4), %rbx
   sub %rbx, %rax
   movq %rax, %rsp
.endm

# -----------------------------------------------------------------
#  NADA
# -----------------------------------------------------------------

.macro NADA 
   nop
.endm

# -----------------------------------------------------------------
#  DSVS
# -----------------------------------------------------------------

.macro DSVS rot
   jmp \rot
.endm

# -----------------------------------------------------------------
#  DSVF
#  Se topo da pilha == 0, entao desvia para rot,
#                          senao segue
#  Implementação complicada.
#  - chama _dsvf com a pilha na seguinte situaçao:
#      valor booleano (%rcx)
#      endereco de retorno se topo=0 (%ebc)
#      endereco de retorno se topo=1 (%rax)
#  - basta empilhar [%rax, %rbx] de acordo com %rcx e "ret"
#
# -----------------------------------------------------------------

.macro DSVF rot
   push $\rot
   call _dsvf
.endm

_dsvf:   
   pop %rax  
   pop %rbx  
   pop %rcx
   cmp $0, %rcx
   je  _dsvf_falso
   push %rax   
   ret
_dsvf_falso:
   push %rbx 
   ret
   
# -----------------------------------------------------------------
#  DSVR - Desvia para rótulo
#
# -----------------------------------------------------------------

.macro DSVR rot j k
   push $\j
   push $\k
   call _dsvr
   jmp \rot
.endm

 _dsvr:
    pop %rax # k
    pop %rbx # j

    push %rax
    push %rax
    ret


# -----------------------------------------------------------------
#  IMPR
# -----------------------------------------------------------------

.macro IMPR
   push $strNumOut
   call printf
   add $8, %rsp
.endm

# -----------------------------------------------------------------
#  LEIT
# -----------------------------------------------------------------

.macro LEIT
   push $entr
   push $strNumIn
   call scanf
   add $8, %rsp
   push entr
.endm

# -----------------------------------------------------------------
#  SOMA
# -----------------------------------------------------------------

.macro SOMA
   popq %rax
   popq %rbx
   addq %rax, %rbx
   pushq %rbx
.endm

# -----------------------------------------------------------------
#  SUBT
# -----------------------------------------------------------------

.macro SUBT
   pop %rax
   pop %rbx
   sub %rax, %rbx
   push %rbx
.endm

# -----------------------------------------------------------------
#  MULT
# -----------------------------------------------------------------

.macro MULT
   pop %rax
   pop %rbx
   imul %rax, %rbx
   push %rbx
.endm
      
# -----------------------------------------------------------------
#  DIVI
# A divisão no intel é esquisita. O comando divl não usa dois
# operandos, mas sim um. A instrução assume que a divisão é do par
# %rdx:%rax (64 # bits) pelo parâmetro. O quociente vai em %rax e o
# resto vai # para %rdx.
# -----------------------------------------------------------------

.macro DIVI
   pop %rdi     # divisor
   pop %rax     # dividendo
   movq $0, %rdx # não pode esquecer de zerar %rdx quando não o usar.
   idiv %rdi     #  faz %rdx:%rax / %rdi
   push %rax     # empilha o resultado
.endm
      
# -----------------------------------------------------------------
#  INVR
# -----------------------------------------------------------------

.macro INVR
   pop %rax
   imul $-1, %rax
   push %rax
.endm
      
# -----------------------------------------------------------------
#  CONJ (E)
# -----------------------------------------------------------------

.macro CONJ
   pop %rax
   pop %rbx
   and  %rax, %rbx
   push %rbx
.endm
      
# -----------------------------------------------------------------
#  DISJ (OU)
# -----------------------------------------------------------------

.macro DISJ
   pop %rax
   pop %rbx
   or   %rax, %rbx
   push %rbx
.endm
      
# -----------------------------------------------------------------
#  NEGA (not)
# -----------------------------------------------------------------

.macro NEGA
   pop %rax
   movq $1, %rbx
   sub %rax, %rbx
   movq %rbx, %rax
   push %rax
.endm

# -----------------------------------------------------------------
#  CMME
# -----------------------------------------------------------------

.macro CMME
   pop %rax
   pop %rbx
   call _cmme
   push %rcx
.endm

_cmme:
   cmp %rax,  %rbx   
   jl _cmme_true
   movq $0, %rcx
   ret
_cmme_true:
   movq $1, %rcx
   ret


# -----------------------------------------------------------------
#  CMMA
# -----------------------------------------------------------------

.macro CMMA
   pop %rax
   pop %rbx
   call _cmma
   push %rcx
.endm

_cmma:
   cmp %rax,  %rbx   
   jg _cmma_true
   movq $0, %rcx
   ret
_cmma_true:
   movq $1, %rcx
   ret

   
# -----------------------------------------------------------------
#  CMIG
# -----------------------------------------------------------------

.macro CMIG
   pop %rax
   pop %rbx
   call _cmig
   push %rcx
.endm

_cmig:
   cmp %rax,  %rbx   
   je _cmig_true
   movq $0, %rcx
   ret
_cmig_true:
   movq $1, %rcx
   ret

# -----------------------------------------------------------------
#  CMDG
# -----------------------------------------------------------------

.macro CMDG
   pop %rax
   pop %rbx
   call _cmdg
   push %rcx
.endm

_cmdg:
   cmp %rax,  %rbx   
   jne _cmdg_true
   movq $0, %rcx
   ret
_cmdg_true:
   movq $1, %rcx
   ret

# -----------------------------------------------------------------
#  CMEG
# -----------------------------------------------------------------

.macro CMEG
   pop %rax
   pop %rbx
   call _cmeg
   push %rcx
.endm

_cmeg:
   cmp %rax,  %rbx   
   jle _cmle_true
   movq $0, %rcx
   ret
_cmle_true:
   movq $1, %rcx
   ret


# -----------------------------------------------------------------
#  CMAG
# -----------------------------------------------------------------

.macro CMAG
   pop %rax
   pop %rbx
   call _cmag
   push %rcx
.endm

_cmag:
   cmp %rax,  %rbx   
   jge _cmge_true
   movq $0, %rcx
   ret
_cmge_true:
   movq $1, %rcx
   ret

   

# -----------------------------------------------------------------
# CHPR p,m { M[s+1]:=i+1; M[s+2]:=m; s:= s+2;  i:=p}
#
# Alterado para: CHPR p,m { M[s+1]:=m; M[s+2]:=i+1; s:= s+2;  i:=p}
# 
# CHPR - A implementação de chamadas de procrdimento é diferente da
# proposta original do livro. O problema é como guardar o ER e depois
# disso guardar k. É possível fazer, porém fica muito complicado (até
# na volta do procrdimento). Por isso, optei por fazer uma
# implementação diferente. Primeiro vai "k" e depois "ER". Isso
# implica em alterações na implementação de ENPR, RTPR e DSVR - mas
# não no nível de geração de comandos. Os mesmos comandos MEPA
# funcionam aqui igual ao que funcionariam na idéia original (exceto
# pela inverção de k e ER, evidentemente).
# -----------------------------------------------------------------

.macro CHPR rot k
   push $\k
   call \rot
.endm


# -----------------------------------------------------------------
# 
# ENPR k { s++; M[s]:=D[k]; D[k]:=s+1 }
#
# -----------------------------------------------------------------

.macro ENPR k
   movq $\k, %rdi
   movq D(,%rdi,4), %rax
   push %rax
   movq %rsp, %rax
   sub $4, %rax
   movq %rax, D(,%rdi,4)
.endm

# -----------------------------------------------------------------
# original: RTPR k,n { D[k]:=M[s]; i:=M[s-2];  s:=s-(n+3) }
# adaptado: RTPR k,n { D[k]:=pop;  i:=pop; lixo:=pop; s:=s-n }
# -----------------------------------------------------------------

.macro RTPR k n
   pop %rax # D[k] salvo
   pop %rbx # ER. Tem que salvar enquanto libera o resto da pilha
   pop %rcx # k do chamador (a ser jogado fora)

   movq $\k, %rdi
   movq %rax, D(,%rdi,4)

   movq $\n, %rax
   imul $4, %rax
   add %rax, %rsp # rsp <- rsp - rax
   push %rbx      # restaura ER para poder fazer "i=M[s-1]"="ret"
   ret
.endm


# -----------------------------------------------------------------
#  Macros para depuração
# -----------------------------------------------------------------

# -----------------------------------------------------------------
# Imprime tracos para indicar passagem
# -----------------------------------------------------------------

.macro IMPRQQ
  push $strTR
  call printf
  add $4, %rsp
.endm
  


# -----------------------------------------------------------------
#  impime_RA
#       k = nivel lexico do ra
#       n = numero de parametros
#       v = numero de vars simples
# -----------------------------------------------------------------
 .macro imprime_RA k,n,v
RT:       push $\k
    push $\n
    push $\v
    call _imprime_RA
 .endm
 
 _imprime_RA:
   pop %rbx  # ER
   pop %rcx  # v
   pop %rdx  # n
   pop %rdi  # k
   movq D(,%rdi,4), %rax
   push $strIniRA
   call printf
   add $4, %rsp
   
_impr_vars_locais:   
   cmp $0, %rcx
   jge _fim_vars_locais
   push (%rax)
   push $strHEX
   call printf
   add $8, %rsp
_fim_vars_locais: 
   push %rbx
   ret



# *****************************************************************
# FIM macros
# *****************************************************************



.section .data
.equ TAM_D, 10
.lcomm D TAM_D
   

entr: .int 0
strNumOut: .string "%d\n"
strNumIn: .string "%d"
strIniRA: .string "----- strIniRA  --------\n"
strTR: .string "-----\n"
strHEX:   .string "%X\n"


.section .text
.equ FIM_PGMA, 1
.equ SYSCALL, 0x80 

.globl _start
_start:

.include "MEPA"


   

