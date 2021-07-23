#include "exercicio2.h"
// Funçao para criar a lista
Lista *criaLista(){
    Lista *l = malloc(sizeof(Lista));
    l->ini = NULL;
    l->fim = NULL;
    return l;
}
// Funçao para destruir a lista
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
// Funçao para inserir no final da lista
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
// Para imprimir os dados
void imprime(Lista *l) {
    No *aux = l->ini;

    while (aux != NULL) {
        if(aux->idade >= 18){ // Tem que ser maior de idade
            if(!strcmp(aux->nome, "")) // Caso nao possua nome
                printf("O usuario de identificacao %d eh de maior.\n", aux->id);
            else
                printf("O usuario %s eh de maior.\n", aux->nome);
        }
        aux = aux->prox; 
    }
}
// Funçao que atribui os dados do arquivo binario para a lista
void lerDados(FILE *arq, Lista *list){
    
    char nome[50];
    int id = 0, idade = 0, final = 0; // variavel "final" para caso chegue no final do arquivo
    
    for(;;){
        fread(&id, sizeof(int), 1, arq);
        fread(&nome, sizeof(char), 50, arq);
        final = fread(&idade, sizeof(int), 1, arq); // se retornar 0, esta no final do arquivo

        if(!final) // sai do laço
            break;

        insereFinal(list, id, idade, nome);
    }
}
