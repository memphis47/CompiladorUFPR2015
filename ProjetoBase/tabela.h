typedef struct item item;
typedef struct lista_params lista_params;

struct item
{
  char *identificador;
  char *rotulo;
  int  nivel_lexico;
  int  deslocamento;
  int  pendente;
  int  decl;
  categorias categoria;
  tipo_parametro passagem;
  tipo_variavel tipo; // mudar depois para enum
  lista_params *param;
  item *itemAnt;
  item *itemProx;
};

struct lista_params{
  int n_param;
  item *fim;
  item *inicio;
  item *paramItem;  
};

typedef struct tabela_simbolos
{
  int n_itens;
  item *topo_pilha;
  item *fim_pilha;
  item *itens;

} tabela_simbolos;