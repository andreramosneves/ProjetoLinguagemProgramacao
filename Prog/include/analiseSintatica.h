#ifndef ANALISESINTATICA_H
#define ANALISESINTATICA_H

#define IS_OPTINAL 1
#define IS_NOT_OPTINAL 0
#define MAX 45

#define REPLICATED 1
#define NOT_REPLICATED 0

#define TERMINAL 0
#define N_TERMINAL 1

#define MAX_VETOR 1000

#define TRUE 1
#define FALSE 0

typedef struct vet{
    char token[70];
    char classe[40];
} my_vet;

my_vet list_token[MAX_VETOR];



int analiseSintatica(char* ip, char* classe,int indice);
void empilhoTudo(int tam, char **sp);
int is_terminal(char *c);


char **sp;


void init();

/* Estrutura para o vetor das Produções */
typedef struct {
    char *regra;
    char *producao;
    int optional;
    int replicate;
    int one_more;
} Production;

/* Classe da minha analise léxica*/
Production rules[MAX + 1];

#endif // ANALISESINTATICA_H
