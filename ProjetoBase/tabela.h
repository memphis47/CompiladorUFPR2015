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
  lista_params *param;
  item *itemAnt;
  item *itemProx;
};

typedef struct params{
  int n_param;
  item *fim;
  item *inicio;
  item *paramItem;  
}lista_params;

typedef struct tabela_simbolos
{
  int n_itens;
  item *topo_pilha;
  item *fim_pilha;
  item *itens;

} tabela_simbolos;