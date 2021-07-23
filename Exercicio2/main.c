#include "exercicio2.h"

int main(int argc, char *argv[]){
    
    char arqNome[50];

    scanf(" %s", arqNome);

    FILE *arq = fopen(arqNome, "rb"); // Abre pra ler
    
    Lista *list = criaLista(); // Lista que vai ter os dados

    if (arq == NULL) // Caso esteja vazio
        exit(-1);

    lerDados(arq, list);
    imprime(list);

    destroi(&list);
    fclose(arq);

    return 0;
}