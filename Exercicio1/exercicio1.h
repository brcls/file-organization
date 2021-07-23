#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no{
    char nome[50];
    int idade;
    int id;
    struct no *prox;
}No;

typedef struct lista{
    No *ini, *fim;
}Lista;

Lista *criaLista();
void destroi(Lista **l);
void insereFinal(Lista *l, int id, int idade, char nome[50]);
void imprime(Lista *l);
void abrirCSV(FILE *arquivo, Lista *list);