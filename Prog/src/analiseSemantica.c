#include "../include/analiseSintatica.h"
#include "../include/analiseSemantica.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void criaTabelaSimbolos(my_vet *vetToken, int vetTamanho, char *s_arq_o){
    FILE *arq_o;
    arq_o = fopen(s_arq_o, "w");
    char s_line[1024];
    char c[4200];

    int flag = 0;
    int flag_var = 0;

    int i = 0;
    int k = 0;
    for(i; i < vetTamanho; i++){
        if(flag == 1){
            sprintf(list_semantica[tam_list_semantica].id, "%s", vetToken[i].token);
            sprintf(list_semantica[tam_list_semantica].tipo, "%s", "");
            sprintf(list_semantica[tam_list_semantica].classe, "%s", vetToken[i-1].token);
            fputs(list_semantica[tam_list_semantica].id, arq_o);
            fputs("\t", arq_o);
            fputs(list_semantica[tam_list_semantica].tipo, arq_o);
            fputs("\t", arq_o);
            fputs(list_semantica[tam_list_semantica].classe, arq_o);
            fputs("\n", arq_o);
            flag = 0;
            tam_list_semantica++;

        }
        if(flag == 2){
            if(flag_var == 0){
                k = tam_list_semantica;
                flag_var = -1;
            }
            if(strcmp("id",vetToken[i].classe) == 0){
                sprintf(list_semantica[tam_list_semantica].id, "%s", vetToken[i].token);
                sprintf(list_semantica[tam_list_semantica].tipo, "%s", "");
                sprintf(list_semantica[tam_list_semantica].classe, "%s", "var");
                tam_list_semantica++;
            }
            if(strcmp(vetToken[i].token, ";") == 0 || strcmp(vetToken[i].token, ")") == 0
                || (strcmp(vetToken[i].token, ",") == 0 && strcmp(vetToken[i - 1].classe, "tipo") == 0 ) ){
                for(  ; k < tam_list_semantica ; k++ ){
                    sprintf(list_semantica[k].tipo, "%s", vetToken[i - 1].token);
                    fputs(list_semantica[k].id, arq_o);
                    fputs("\t", arq_o);
                    fputs(list_semantica[k].tipo, arq_o);
                    fputs("\t", arq_o);
                    fputs(list_semantica[k].classe, arq_o);
                    fputs("\n", arq_o);

                }
                flag_var = 0;
                flag = 0;
            }
        }

        if(strcmp(vetToken[i].classe, "reservada") == 0){
            if(strcmp(vetToken[i].token, "program") == 0)
                flag = 1;
            if(strcmp(vetToken[i].token, "procedure") == 0)
                flag = 1;
            if(strcmp(vetToken[i].token, "var") == 0)
                flag = 2;
        }
    }

    fclose(arq_o);
}

int analiseSemantica(my_vet *vetToken, int vetTamanho, char *s_arq_o){

    int i = 0;
    int temp = 0;

    int fl_integer = 0;
    int fl_boolean = 0;
    int fl_erro = 0;

    int fl_if = 0;

    FILE *arq_o;
    arq_o = fopen(s_arq_o, "w");
    int index;

    for(; i < vetTamanho; i++){
        if((strcmp(vetToken[i].token,"=") == 0 || strcmp(vetToken[i].token,"<>") == 0)
           && fl_boolean == 1 && fl_if == 1){
            if(strcmp(vetToken[i + 1].classe, "bool") == 0)
                fl_boolean = 0;
            else if(strcmp(vetToken[i + 1].classe, "id") == 0){
                index = busca(list_semantica, vetToken[i + 1].token);
                if (index !=  -1){
                    if (strcmp(list_semantica[index].tipo,"boolean") == 0)
                        fl_boolean = 1;
                    else
                        fl_erro = 1;
                }
            }else if(strcmp(vetToken[i + 1].classe, "numero") == 0){
                fl_erro = 1;
            }

        }

        if ( fl_integer == 1 && fl_if == 1 ){
            if(strcmp(vetToken[i].token, ")") == 0){
                fputs("ERRO SEMANTICO, ESPERADO INTEGER NO BOOLEANO\n",arq_o);
                fl_integer = 0;
                fl_if = 0;
            }

        }

        if ( fl_boolean == 1 && fl_if == 1 ){

            if(strcmp(vetToken[i].token, ")") == 0 && fl_erro == 1){
                fputs("ERRO SEMANTICO, ESPERADO BOOLEANO\n",arq_o);
                fl_boolean = 0;
                fl_if = 0;
                fl_erro = 0;
            }

        }
        if((strcmp(vetToken[i].token,">") == 0 || strcmp(vetToken[i].token,"<") == 0 || strcmp(vetToken[i].token,"<=") == 0
            || strcmp(vetToken[i].token,">=") == 0 || strcmp(vetToken[i].token,"=") == 0 || strcmp(vetToken[i].token,"<>") == 0)
           && fl_if == 1){
            if(strcmp(vetToken[i + 1].classe, "numero") == 0)
                fl_integer = 0;
            else if(strcmp(vetToken[i + 1].classe, "id") == 0){
                index = busca(list_semantica, vetToken[i + 1].token);
                if (index !=  -1){
                    if (strcmp(list_semantica[index].tipo,"boolean") == 0)
                        fl_integer = 1;
                }
            }
            else if(strcmp(vetToken[i + 1].classe, "bool") == 0)
                fl_integer = 1;
        }
//        else if (fl_if == 1)
//            printf("Passou aqui %s\n",vetToken[i].token);


        if(strcmp(vetToken[i].token, ")") == 0 && fl_if == 1)
            fl_if = 0;

        if(strcmp(vetToken[i].token,"if") == 0 || strcmp(vetToken[i].token,"while") == 0){
            fl_if = 1;
            temp = i;
            i += 2;
            fputs(vetToken[i - 2].token,arq_o);
            fputs("\n",arq_o);
            fputs(vetToken[i - 1].token,arq_o);
            fputs("\n",arq_o);
            fputs(vetToken[i].token,arq_o);
            fputs("\n",arq_o);

            index = busca(list_semantica, vetToken[i].token);
            if (index !=  -1){
                if (strcmp(list_semantica[index].tipo,"integer") == 0)
                    fl_integer = 1;
                else if (strcmp(list_semantica[index].tipo,"boolean") == 0){
                    fl_boolean = 1;
                    fl_erro = 0;
                }
            }

        }else{
            fputs(vetToken[i].token,arq_o);
            fputs("\n",arq_o);
        }

    }
    fclose(arq_o);

}

int busca (my_sem *list_semantica, char *id){
    int j = 0;
    for( j ; j < tam_list_semantica; j++){
        if ( strcmp( list_semantica[j].id, id) == 0){
            return j;
        }
    }
    return -1;
}


