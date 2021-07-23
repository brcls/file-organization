#include "exercicio3.h"

int main(int argc, char *argv[]){
    
    char arqNome[50];

    scanf(" %s", arqNome);

    FILE *arq = fopen(arqNome, "wb"); // Abre pra escrever

    if (arq == NULL) // Caso esteja vazio
        exit(-1);

    escreverDados(arq);

    fclose(arq);

    binarioNaTela(arqNome);

    return 0;
}