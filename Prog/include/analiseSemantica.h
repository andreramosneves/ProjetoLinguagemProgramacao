#ifndef ANALISESEMANTICA_H
#define ANALISESEMANTICA_H


typedef struct sem{
    char id[70];
    char tipo[32];
    char classe[40];
} my_sem;

my_sem list_semantica[MAX_VETOR];
int tam_list_semantica = 0;

void criaTabelaSimbolos(my_vet *vetToken, int vetTamanho, char *s_arq_o);

int analiseSemantica(my_vet *vetToken, int vetTamanho, char *s_arq_o);

#endif // ANALISESEMANTICA_H
