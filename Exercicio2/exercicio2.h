// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estruturas
typedef struct no{
    char nome[50];
    int idade;
    int id;
    struct no *prox;
}No;

typedef struct lista{
    No *ini, *fim;
}Lista;

//Funçoes
Lista *criaLista();
void destroi(Lista **l);
void insereFinal(Lista *l, int id, int idade, char nome[50]);
void imprime(Lista *l);
void lerDados(FILE *arq, Lista *list);