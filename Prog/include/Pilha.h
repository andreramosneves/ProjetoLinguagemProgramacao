#ifndef PILHA_H
#define PILHA_H


/* Estrutura da minha pilha */
typedef struct {
  int Chave;
  /* Se for terminar é 0, caso contrário é 1*/
  int term;
  /* outros componentes */
  char *regra;
  char *producao;
} TipoItem;

typedef struct TipoCelula *TipoApontador;

typedef struct TipoCelula {
  TipoItem Item;
  TipoApontador Prox;
} TipoCelula;

typedef struct {
  TipoApontador Fundo, Topo;
  int Tamanho;
} TipoPilha;


void FPVazia(TipoPilha *Pilha);

TipoPilha pilha;
TipoItem topo;


#endif // PILHA_H
