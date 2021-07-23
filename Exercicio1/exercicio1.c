#include "exercicio1.h"

Lista *criaLista(){
    Lista *l = malloc(sizeof(Lista));
    l->ini = NULL;
    l->fim = NULL;
    return l;
}

void destroi(Lista **l) {
    // apaga elementos
    No *aux;
    while ((*l)->ini != NULL) {
        aux = (*l)->ini;
        (*l)->ini = (*l)->ini->prox;
        free(aux);
    }
    // apaga lista
    free(*l);
    *l = NULL;
    return;
}

void insereFinal(Lista *l, int id, int idade, char nome[50]) {
    No *aux = malloc(sizeof(No));
    
    aux->idade = idade;
    aux->id = id;
    strcpy(aux->nome, nome);   
    
    aux->prox = NULL;
    // atualiza ponteiros
    if (l->fim != NULL)
        l->fim->prox = aux;
    l->fim = aux;
    
    if (l->ini == NULL)
        l->ini = aux;

}

void imprime(Lista *l) {
    No *aux = l->ini;

    while (aux != NULL) {
        if(aux->idade >= 18){
            if(!strcmp(aux->nome, ""))
                printf("O usuario de identificacao %d eh de maior.\n", aux->id);
            else
                printf("O usuario %s eh de maior.\n", aux->nome);
        }
        aux = aux->prox;
    }
}

void abrirCSV(FILE *arquivo, Lista *list){
     
    int flag = 0;
    char *linha = malloc(300*sizeof(char)); // pega cada linha do arquivo
    
    while (fgets(linha, 1024, arquivo)){// pega linha até o final do arquivo
        if(flag){
            char* tmp = strdup(linha);
            char *token, *nome;
            int id = 0, idade = 0, count = 0;
            
            while((token = strsep(&tmp, ","))) {
                if(count == 0)
                    id = atof(token);
                else if(count == 1)
                    nome = token;
                else if(count == 2)
                    idade = atof(token);
                
                count++;
            }
            
            insereFinal(list, id, idade, nome);

            count = 0;
        }
        flag = 1;
    }
    free(linha);
}   