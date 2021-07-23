#include "exercicio1.h"

int main(int argc, char *argv[]){
    
    char *arqName = malloc(25*sizeof(char));

    scanf(" %s", arqName);

    Lista *list = criaLista();
    FILE *arquivo = fopen(arqName,"r");
    
    if (arquivo == NULL)
        exit(-1);

    abrirCSV(arquivo, list);

    imprime(list);

    destroi(&list);
    free(arqName);
    fclose(arquivo);

    return 0;
}