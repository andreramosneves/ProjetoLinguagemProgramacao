#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/analiseLexica.h"
#include "include/analiseSintatica.h"
#include "include/Pilha.h"
#include "assert.h"

#define COM_COMENTARIO 1
#define SEM_COMENTARIO 0

/*
Feito por :

André Ramos Neves
Vinicius Santos Nunes
André Augusto Miguel
*/
int fl_comentario = 0;
int tam_vetor = 0;


int main (int args, char **argc) {

    printf("Lendo arquivo de entrada %s\n",argc[1]);
    /* Arquivo que irá ler os tokens*/
    FILE *arq_i;
    /* Arquivo de saída que irá escrever as palavras reconhecidas */
    FILE *arq_o;

    char *arq_p;



    /* Variável que irá armazenar o que está dentro do arquivo de texto*/
	char c[4200];

	int result = 0;
	//K refere-se ao tamanho do token
	int k,inicio;
	inicio = 0;
	k = 0;

	/* Carrego todas as minhas regras e producoções */
	init();

	/* Inicializo a minha pilha com a primeira regra */
    FPVazia(&pilha);


    /* Configuração inicial da minha primeira regra */
    topo.Chave = 1;
    topo.regra = "P";
    topo.producao = "program I ; B";
    topo.term = N_TERMINAL;

    Empilha(topo, &pilha);

    char token[100];

    // Abre um arquivo TEXTO para LEITURA
    arq_i = fopen(argc[1], "rt");
    arq_o = fopen("analisadorlexico.txt", "w");

    while (!feof(arq_i))
    {
        arq_p = fgets(c, sizeof(c), arq_i);
        if (arq_p != NULL){

            while(c[k] != '\0' && c[k] != '\n'){

                 /* Se forem diferentes de meus delimitadores e no caso ele tem que ser de tamanho 1 */
                if(c[k] == ',' ||  c[k] == '.'
                         ||  c[k] ==  '(' ||  c[k] ==  ')'
                ||  (c[k] ==  ':' && c[k + 1] != '=') || c[k] ==  ';')     {
                   token[inicio] = c[k];
                   inicio += 1;
                }else
                /* Verifico qual é minha palavra */
                while( c[k] != ' ' &&  c[k] != '\n' && c[k] != '\t' && c[k] != '\0'  && c[k] != ',' &&  c[k] != '.'
                         &&  c[k] !=  '(' &&  c[k] !=  ')'
                         &&  c[k] !=  ';'){
                   token[inicio] = c[k];
                   k++;
                   inicio += 1;

                    /* Caso ache algum delimitador, retorno a posição de cada palavra */
                    if(c[k] == ',' ||  c[k] == '.'
                             ||  c[k] ==  '(' ||  c[k] ==  ')'
                    ||  c[k] ==  ':' ||  c[k] ==  ';' || (c[k - 1] == '=' && (c[k] != ' ' || c[k] != '\n' )))   {
                        k--;
                        break;
                    }

                }

                token[inicio] = '\0';


                /* É realizado a análise Léxica */
                if (strlen(token) != 0) {
                    if(fl_comentario == SEM_COMENTARIO){
                        result = analiseLexica (token);
                    }
                    else
                        result = 0;
                    /* Se for = 1 , então escrevo no meu novo arquivo de texto */
                    if(arq_o == NULL)
                        printf("Erro ao escrever o arquivo! \n");
                    else{
                        if(result == 0){
                            fputs(classe, arq_o);
                            fputs(" ", arq_o);
                            fputs(token, arq_o);
                            /* Se abri meu comentário altero minha flag, e todas as minhas classes de palavras geradas serão de comentário */
                            if(strcmp(classe,"comentario") == 0 && strcmp(token,"/*") == 0)
                                fl_comentario = COM_COMENTARIO;
                            /* Se encontrar o token de fechamento, então mudo meu flag */
                            if(strcmp(classe,"comentario") == 0 && strcmp(token,"*/") == 0 && fl_comentario == COM_COMENTARIO)
                                fl_comentario = SEM_COMENTARIO;

                            fputc('\n', arq_o);
                        }else if(result == 1){
                            printf("Erro do Token = %s\n", token);
                            printf("Tamanho do Token = %d\n", strlen(token));
                            printf("Comentario = %d\n", fl_comentario);
                            fputs("ERRO LEXICO\n", arq_o);
                            fputc('\n', arq_o);
                            exit(1);
                        }
                    }
                }
                /* Reseto as váriaveis de controle do meu token */
                k++;
                inicio = 0;

            }
        }
            inicio = 0;
            k = 0;
    }
    /* Escrevo em meu arquivo de saído o token valido */

    fclose(arq_o);
    fclose(arq_i);

    printf("\nArquivo gerado como analisadorlexico.txt\n");

    arq_i = fopen("analisadorlexico.txt", "rt");

    /* Esse Loop é para atribuir a um vetor todas os tokens da análise léxica */
    while (!feof(arq_i))
    {
        arq_p = fgets(c, sizeof(c), arq_i);
        inicio = 0;
        k = 0;
        if (arq_p != NULL){

                inicio = 0;
                /* Pego o valor da classe na análise lexica */
                while( c[k] != ' '){
                    list_token[tam_vetor].classe[inicio] = c[k];
                    classe[inicio] = c[k];
                    k++;
                    inicio += 1;
                }
                classe[inicio] = '\0';
                list_token[tam_vetor].classe[inicio] = '\0';
                inicio = 0;

                /* Ignoro o espaço*/
                k++;

                /* Pego o valor do Token na análise lexica*/
                while( c[k] != '\n'){
                    token[inicio] = c[k];
                    list_token[tam_vetor].token[inicio] = c[k];
                    k++;
                    inicio += 1;
                }
                token[inicio] = '\0';
                list_token[tam_vetor].token[inicio] = '\0';
                /* É realizado a análise Léxica*/

        }
                tam_vetor++;
    }

                arq_o = fopen("analisadorsintatico.txt", "w");
                int j;
                /* Reseto meu flag de comentário */
                fl_comentario = SEM_COMENTARIO;
                for(j = 0; j < tam_vetor - 1  ; j++){
                    if(strcmp(list_token[j].classe,"comentario") != 0){
                        result = analiseSintatica(list_token[j].token,list_token[j].classe,j);
                    /* Se for = 1 ERRO, caso contrário escrevo no meu novo arquivo de texto */
                        if(result == 0){
                            fputs(list_token[j].token, arq_o);
                            fputc('\n', arq_o);

                        }else if(result == 1){
                            fputs("ERRO SINTATICO", arq_o);
                            fputc('\n', arq_o);
                        }
					} else{
                            if(strcmp(list_token[j].classe,"comentario") == 0 && strcmp(list_token[j].token,"/*") == 0){
                                fl_comentario = COM_COMENTARIO;
                            }

                            if(strcmp(list_token[j].classe,"comentario") == 0 && strcmp(list_token[j].token,"*/") == 0){
                                /* Fechou o comentário sem ter aberto ele. */
                                if( fl_comentario == SEM_COMENTARIO){
                                    fputs("ERRO SINTATICO",arq_o);
                                    fputc('\n', arq_o);
                                }
                                fl_comentario = SEM_COMENTARIO;

                            }

					}
                }

                if(! Vazia(pilha)){
                    Desempilha(&pilha,&topo);
                    fputs("ERRO SINTATICO, Expected: ",arq_o);
                    fputs(topo.regra,arq_o);
                    fputc('\n', arq_o);
                }
                fclose(arq_o);


    // Iniciando a analise semantica
    criaTabelaSimbolos(list_token, tam_vetor, "tabelasimbolos.txt");
    printf(" Arquivo gerado tabelasimbolos.txt\n");
    analiseSemantica(list_token, tam_vetor, "analiseSemantica.txt");
    printf(" Arquivo gerado analiseSemantica.txt\n");
	//system("pause");

	return 0;

}


