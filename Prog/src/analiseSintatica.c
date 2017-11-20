#include "../include/analiseSintatica.h"
#include "../include/Pilha.h"
#include "string.h"



int analiseSintatica(char* ip, char* classe,int indice)
{
    int k = 0;
    int tam = 0;
    int l = 0;
    /* Acessamos o topo da nossa pilha com a sua configuração inicial */
    Desempilha(&pilha,&topo);
    Empilha(topo,&pilha);
    while(! Vazia(pilha)){

        /* Se casou com o input, desimpilhamos e vamos para o próximo input */
       if(strcmp (ip ,topo.regra) == 0 || (topo.term == TERMINAL && strcmp(classe, topo.regra) == 0)){
            /* Pop and next input */
            Desempilha(&pilha,&topo);

            return 0;
       }else if( topo.term == TERMINAL ){
            /* Return error */
            return 1;
       }else if(find_index_rule(topo.regra) != -1){
                /* Get Production */
                Desempilha(&pilha,&topo);

                char *producoes;
                k = find_index_rule(topo.regra);
                producoes = rules[k].producao;
                /* Yk,Yk-1, Y1*/

                tam = split(producoes, " ");
                    /*
                    Esse trecho verifica na ocasião que se for obrigatório o preenchimento de pelo menos umas das producoões
                     da regra, se for nulo, então retornará erro  */
                    char temp[20] = "";
                    if(rules[find_index_rule(topo.regra)].one_more == TRUE ){
                        for(l = 0; l < tam  ; l++){
                                /* Pego a primeira regra */
                                first_rule(temp,rules[find_index_rule(sp[l])].producao);
                            /* Empilho somente uma das regras */
                            if(strcmp (ip ,rules[find_index_rule(sp[l])].producao) == 0
                                || strcmp(classe, rules[find_index_rule(sp[l])].producao) == 0
                                || strcmp(ip, temp ) == 0){

                                producoes = rules[find_index_rule(sp[l])].regra;
                                tam = split(producoes, " ");
                                break;

                            }else if( l == tam - 1 && (strcmp (ip ,rules[find_index_rule(sp[l])].producao) != 0
                                && strcmp(classe, rules[find_index_rule(sp[l])].producao) != 0)){
                                    /* Retorna erro sintático */
                                    return 1;
                            }
                        }
                    }
                    /* Se o valor que desimpilhei for opcional
                        , então vejo o primeiro valor do split e compara aou meu Input*/
                    if(rules[find_index_rule(topo.regra)].optional == IS_OPTINAL ){
                        /* Se aceita o valor de Vazio em seu token*/
                        if(strcmp (ip ,sp[0]) == 0 || strcmp(classe, sp[0]) == 0) {
                            empilhoTudo(tam,sp);
                        } else {
                            /* Se é opcional e Nulo, então,desempilh e atualizo meu topo com valor desempilhado */
                            Desempilha(&pilha,&topo);
                            Empilha(topo,&pilha);

                        }

                    }else{
                        /* Caso encontre algum caso específico para a regra que se auto replique */
                        if(rules[find_index_rule(topo.regra)].replicate == REPLICATED){
                                /* Verifico se que é dependente pode se replicar, caso não encontre, não gero a última regra */
                            if(strcmp(ip, "*") != 0 && strcmp(ip, "div") != 0 &&
                               strcmp(ip, "+") != 0 && strcmp(ip, "-") != 0 )
                                /* Não empilho a regra que se replica*/
                                if(strcmp(topo.regra,"C") == 0){
                                    /* Aqui eu vejo o Follow e as replicações do Comando que são bem importantes */
                                    if(strcmp(classe,"id") == 0 && strcmp(list_token[indice + 1].token,":=") == 0){
                                        producoes = "A C";
                                        tam = split(producoes, " ");
                                    }else if(strcmp(ip,"if") == 0){
                                        producoes = "CN C";
                                        tam = split(producoes, " ");
                                    }else if(strcmp(classe,"id") == 0 && strcmp(list_token[indice + 1].token,"(") == 0){
                                        producoes = "PC C";
                                        tam = split(producoes, " ");
                                    }else if(strcmp(ip,"while") == 0){
                                        producoes = "CR C";
                                        tam = split(producoes, " ");
                                    }else if(strcmp(ip,"write") == 0){
                                        producoes = "CWR C";
                                        tam = split(producoes, " ");
                                    }else if(strcmp(ip,"begin") == 0){
                                        producoes = "begin C end ;";
                                        tam = split(producoes, " ");
                                    }
                                    else
                                        tam--;
                                }
                                else
                                    tam--;

                        }
                            empilhoTudo(tam,sp);

                    }

       }else{
            return 1;
       }

    }
    return 1;

}

/* Função para empilhar o meu array de caracteres */

void empilhoTudo(int tam, char **sp)
{
    int i = 0;
    int k = 0;

    for(i = tam - 1 ; i >= 0; i--){
        k = find_index_rule(sp[i]);

        if(k >= 0){
            topo.regra = rules[k].regra;
            topo.producao = rules[k].producao;
        }else{
            topo.regra = sp[i];
            topo.producao = "";
        }
        topo.term = is_terminal(sp[i]);
        Empilha(topo,&pilha);

    }

}

/* Função que verifica se for letra maiuscula, então é considerada que produziu algo não terminal*/
int is_terminal(char *c){

if (c[0] == 'A'|| c[0] == 'B'|| c[0] == 'C'|| c[0] == 'D'||
     c[0] == 'E'|| c[0] == 'F'||  c[0] == 'G'|| c[0] == 'H'||
      c[0] == 'I'||c[0] == 'J'|| c[0] == 'K'|| c[0] == 'L'||
       c[0] == 'M'|| c[0] == 'N'|| c[0] == 'O'||  c[0] == 'P'||
        c[0] == 'Q'|| c[0] == 'R'||c[0] == 'S'|| c[0] == 'T'|| c[0] == 'U'||
     c[0] == 'V'|| c[0] == 'W'|| c[0] == 'Y'||  c[0] == 'X'|| c[0] == 'Z' || c[0] == '[')
    return N_TERMINAL;

return TERMINAL;

}

/* Funçao que retorna um array de caracteres a minha variável sp, declarada no header da análiseSintatica.h */
int split (const char *str, char *sepa)
{
    char *p;
    size_t  items = 0, q;

    p = str;
    for (;;)
    {
            p += strspn(p, sepa);
            if (!(q = strcspn(p, sepa)))
                    break;
            if (q)
            {
                    sp           = realloc(sp, (items+1) * sizeof(char *));
                    sp[items]    = malloc(q+1);
                    strncpy(sp[items], p, q);
                    sp[items][q] = 0;
                    items++;
                    p += q;
            }
    }

    sp           = realloc(sp, (items+1) * sizeof(char *));
    sp[items]    = NULL;
    return items;

}



/* Para cada espaço na produção ele vai produzir várias outras regras

    Palavras e simbólos com letra minúscula são consideradas terminais.
    O único símbolo que é considerado não terminal é a "["
*/
void init(){
    int i = 0;
    /* Configuração Inicial*/
    rules[i].regra    = "P";
    rules[i].producao = "program I ; B";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* identificador */
    rules[i].regra    = "I";
    rules[i].producao = "U";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* Letra ou Digito// Classe = ID */
    rules[i].regra    = "U";
    rules[i].producao = "id";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;
    /* Block */
    rules[i].regra    = "B";
    rules[i].producao = "[D] [S] begin C end .";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* Exemplo de uma regra que se auto replique sem ser obrigatório o preenchimento do var,
        elá só vai empilhar se encontrar o terminal var, caso encontre, então irá replicar a produção [D] */
    /* Declaration */
    rules[i].regra    = "[D]";
    rules[i].producao = "var DV ; [D]";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;

    i += 1;

    /* Declaration Variable */
    rules[i].regra    = "DV";
    rules[i].producao = "LI : tipo";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

   /* Identifier List*/
    rules[i].regra    = "LI";
    rules[i].producao = "id NV";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* New Variable */
    rules[i].regra    = "NV";
    rules[i].producao = ", LI";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* No mesmo caso que pode ter várias vezes a regra [D], a regra [S] é igual */
    rules[i].regra    = "[S]";
    rules[i].producao = "procedure id ( LP ) ; [D] CC [S]";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* List Parameter */
    rules[i].regra    = "LP";
    rules[i].producao = "var PAR";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* Parameter Procedure */
    rules[i].regra    = "PAR";
    rules[i].producao = "id : tipo NP";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* New Parameter Procedure*/
    rules[i].regra    = "NP";
    rules[i].producao = ", var PAR";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;


    /* Command Composite */
    rules[i].regra    = "CC";
    rules[i].producao = "begin C end ;";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* Command */
    /* Essa regra a produção é marcado para o flag se replicar de acordo com a produção,
        por enquanto é necessário informar a produção que seja opcional e que gere ele mesmo e seja auto replicável,
        porém deve ser tratado no trecho da análise sintática os tokens que a regra irá produzir sua auto replicação
        Pós linha 77 são tratadas todas as regras que se auto replicam
    */
    rules[i].regra    = "C";
    rules[i].producao = "A C";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = REPLICATED;

    i += 1;

    /* Atribution */

    rules[i].regra    = "A";
    rules[i].producao = "id := ES ;";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* Procedure Call*/
    rules[i].regra    = "PC";
    rules[i].producao = "id ( CLP ) ;";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* LP = Lista de Parametros */
    rules[i].regra    = "CLP";
    rules[i].producao = "JA JB JC";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    rules[i].regra    = "JA";
    rules[i].producao = "id NPC";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    rules[i].regra    = "JB";
    rules[i].producao = "bool NPC";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* New Parameter Call */
    rules[i].regra    = "NPC";
    rules[i].producao = ", CLP";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    rules[i].regra    = "JC";
    rules[i].producao = "numero NV";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* CR =  Comando Repetitivo*/
    rules[i].regra    = "CR";
    rules[i].producao = "while ( E ) do C";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* CN =  Condicional */
    rules[i].regra    = "CN";
    rules[i].producao = "if ( E ) then C EL";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* EL = Else */
    rules[i].regra    = "EL";
    rules[i].producao = "else C";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* E = Expressão */
    rules[i].regra    = "E";
    rules[i].producao = "ES RL";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;


    /* ES = Expressão Simples*/
    rules[i].regra    = "ES";
    rules[i].producao = "SM MN TR";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* Sinal de Soma */
    rules[i].regra    = "SM";
    rules[i].producao = "+";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* Sinal de Menos */
    rules[i].regra    = "MN";
    rules[i].producao = "-";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* TR = Termo */
    rules[i].regra    = "TR";
    rules[i].producao = "FA MP";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    rules[i].regra    = "MP";
    rules[i].producao = "ML DVI SM MN TR";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = REPLICATED;
    i += 1;

    /* Multiplicacao */
    rules[i].regra    = "ML";
    rules[i].producao = "*";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /* Divisor */
    rules[i].regra    = "DVI";
    rules[i].producao = "div";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /*

        A flag one_more indica que as regras informadas deverão produzir pelo menos 1 uma delas
        É tratado isso na linha 45

        FA = Fator
    */
    rules[i].regra    = "FA";
    rules[i].producao = "KA KC G KB";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    rules[i].one_more = TRUE;
    i += 1;

    rules[i].regra    = "KA";
    rules[i].producao = "id";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    rules[i].regra    = "KB";
    rules[i].producao = "numero";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    rules[i].regra    = "KC";
    rules[i].producao = "bool";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    rules[i].regra    = "G";
    rules[i].producao = "( ES )";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;


    /* RL = Relação */
    rules[i].regra    = "RL";
    rules[i].producao = "EQ DF MEN MEQ MAQ MAI";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /*  EQ = Igual     */
    rules[i].regra    = "EQ";
    rules[i].producao = "= ES";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /*  DF = Diferença     */
    rules[i].regra    = "DF";
    rules[i].producao = "<> ES";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /*  MEN = Menor     */
    rules[i].regra    = "MEN";
    rules[i].producao = "< ES";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    /*  MAI = Maior    */
    rules[i].regra    = "MAI";
    rules[i].producao = "> ES";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    rules[i].regra    = "MEQ";
    rules[i].producao = "<= ES";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;

    rules[i].regra    = "MAQ";
    rules[i].producao = ">= ES";
    rules[i].optional = IS_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;


    /* Comando write */
    rules[i].regra    = "CWR";
    rules[i].producao = "write ( id ) ;";
    rules[i].optional = IS_NOT_OPTINAL;
    rules[i].replicate = NOT_REPLICATED;
    i += 1;


}

/* Função que retorna o indice da regra de acordo com o parâmetro passado,
    caso não encontre retornará 1*/
int find_index_rule (char *c){

    int i = 0;
    for(i = 0; i < MAX; i++){
        if(strcmp(rules[i].regra, c) == 0){
            return i;
        }
    }
    return -1;
}

/* Pego a primeira regra  de acordo com a produção passada atribuo a minha variavel temporaria */
void first_rule ( char *temp, char* production){
    int i = 0;

    for( i = 0 ; i < strlen(production); i++){
        if ( production[i] == ' '){
         temp[i] = '\0';
         break;
        }
        temp[i] = production[i];
    }

}

