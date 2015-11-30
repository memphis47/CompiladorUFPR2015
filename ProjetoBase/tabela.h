typedef struct item item;

struct item
{
  char *identificador;
  categorias categoria;
  int  nivel_lexico;
  int  deslocamento;
  char *passagem;
  char *rotulo;
  char *tipo; // mudar depois para enum
  item *itemAnt;
  item *itemProx;
};

typedef struct tabela_simbolos
{
  int n_itens;
  item *topo_pilha;
  item *fim_pilha;
  item *itens;

} tabela_simbolos;