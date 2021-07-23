/*
    Johnny Silva | 11371350
    Erick Barcelos | 11345562
*/
#include "dados.h"

// Função que printa veiculo e linha na ordem desejada
void printarVeiculoLinha(RegistroCabLinha cabLinha, RegistroDadosLinha linha, RegistroCabVeiculo cabVeiculo, RegistroDadosVeiculo veiculo){
    imprimirSelectVeiculo(veiculo, cabVeiculo);
    imprimirSelectLinha(linha, cabLinha);
}

// função de junção loop aninhado
void juncaoDeLoopAninhado(){
    RegistroCabLinha cabLinha = inicializarCabLinha();
    RegistroCabVeiculo cabVeiculo = inicializarCabVeiculo();
    RegistroDadosLinha linha = inicializarDadosLinha();
    RegistroDadosVeiculo veiculo = inicializarDadosVeiculo();
    
    char linhaNome[30];
    char veiculoNome[30];
    char buffer[30];

    scanf(" %s %s %s %s", veiculoNome, linhaNome, buffer, buffer);

    FILE *arqVeiculo = abrirBinarioLeitura(veiculoNome);
    FILE *arqLinha = abrirBinarioLeitura(linhaNome);

    lerCabecalhoVeiculo(arqVeiculo, &cabVeiculo);
    lerCabecalhoLinha(arqLinha, &cabLinha);
    
    int existeIgualdade = 0;

    for(int i = 0; i < (cabVeiculo.nroRegistros+cabVeiculo.nroRegRemovidos); i++){ // laço para o arquivo veiculo
        lerDadosVeiculo(arqVeiculo, &veiculo);
        
        if(veiculo.removido == '0') //Se o dado não for removido, continuaremos 
			continue;
        
        fseek(arqLinha, 82, SEEK_SET);

        for(int j = 0; j < (cabLinha.nroRegistros+cabLinha.nroRegRemovidos); j++){ // laço pra o arquivo linha
            lerDadosLinha(arqLinha, &linha);

            if(linha.removido == '0') //Se o dado não for removido, continuaremos
			    continue;

            if(linha.codLinha == veiculo.codLinha) // se existir igualdade
                printarVeiculoLinha(cabLinha, linha, cabVeiculo, veiculo), existeIgualdade = 1;
            
        }
    }

    if(!existeIgualdade)
        printf("Registro inexistente.\n");

    fclose(arqVeiculo);
    fclose(arqLinha);

}

// função de loop unico
void juncaoDeLoopUnico(){
    RegistroCabLinha cabLinha = inicializarCabLinha();
    RegistroCabVeiculo cabVeiculo = inicializarCabVeiculo();
    RegistroDadosLinha linha = inicializarDadosLinha();
    RegistroDadosVeiculo veiculo = inicializarDadosVeiculo();
    RegCabBtree cabBtree = inicializarCabecalho();
    
    char linhaNome[30];
    char veiculoNome[30];
    char arvoreBnome[30];
    char buffer[30];
    long long int foundRegRRN = 0;

    scanf(" %s %s %s %s %s", veiculoNome, linhaNome, buffer, buffer, arvoreBnome);

    FILE *arqVeiculo = abrirBinarioLeitura(veiculoNome);
    FILE *arqLinha = abrirBinarioLeitura(linhaNome);
    FILE *arvoreB = abrirBinarioLeitura(arvoreBnome);

    lerCabecalhoVeiculo(arqVeiculo, &cabVeiculo);
    lerCabecalhoLinha(arqLinha, &cabLinha);
    lerCabecalhoArvoreB(arvoreB, &cabBtree);

    int existeIgualdade = 0;

    for(int i = 0; i < (cabVeiculo.nroRegistros+cabVeiculo.nroRegRemovidos); i++){ // laço do arquivo veiculo
        lerDadosVeiculo(arqVeiculo, &veiculo);
        
        if(veiculo.removido == '0') //Se o dado não for removido, continuaremos
			continue;
        
        if(buscarArvoreB(arvoreB, cabBtree.noRaiz, veiculo.codLinha, &foundRegRRN)){ // busca na arvore-B
            fseek(arqLinha, foundRegRRN, SEEK_SET);
            lerDadosLinha(arqLinha, &linha);
            printarVeiculoLinha(cabLinha, linha, cabVeiculo, veiculo);
            existeIgualdade = 1;
        }
            
    }

    if(!existeIgualdade)
        printf("Registro inexistente.\n");

    fclose(arqVeiculo);
    fclose(arqLinha);
    fclose(arvoreB);
}

// função de comparação de codLinha do veiculo para o qsort
int compararCodlinhaV(const void *x, const void *y) {
    int pri = ((RegistroDadosVeiculo *)x)->codLinha;
    int seg = ((RegistroDadosVeiculo *)y)->codLinha;
    return (pri - seg);
}

// função de comparação de codLinha da linha para o qsort
int compararCodlinhaL(const void *x, const void *y) {
    int pri = ((RegistroDadosLinha *)x)->codLinha;
    int seg = ((RegistroDadosLinha *)y)->codLinha;
    return (pri - seg);
}

// ordena vetor registro de dados do veiculo
void quickSortVeiculo(int numReg, FILE *desordenado, RegistroDadosVeiculo vetor[LIMIT]){
    for(int i = 0; i < numReg; i++){
        vetor[i] = inicializarDadosVeiculo();
        lerDadosVeiculo(desordenado, &vetor[i]);
    }

    qsort(vetor, numReg, sizeof(RegistroDadosVeiculo), compararCodlinhaV);
}

//escreve vetor de registros em um arquivo
void escreverVetVeiculo(int *numRegInseridos, int numReg, FILE *ordenado, RegistroDadosVeiculo vetor[LIMIT]){
    for(int i = 0; i < numReg; i++){
        if(vetor[i].removido == '1'){
            escreverDadosVeiculo(ordenado, vetor[i]);
            (*numRegInseridos)++;
        }
    }
}

// ordena vetor registro de dados da linha
void quickSortLinha(int numReg, FILE *desordenado, RegistroDadosLinha vetor[LIMIT]){
    for(int i = 0; i < numReg; i++){
        vetor[i] = inicializarDadosLinha();
        lerDadosLinha(desordenado, &vetor[i]);
    }

    qsort(vetor, numReg, sizeof(RegistroDadosLinha), compararCodlinhaL);
}

//escreve vetor de registros em um arquivo
void escreverVetLinha(int *numRegInseridos, int numReg, FILE *ordenado, RegistroDadosLinha vetor[LIMIT]){
    for(int i = 0; i < numReg; i++){
        if(vetor[i].removido == '1'){
            escreverDadosLinha(ordenado, vetor[i]);
            (*numRegInseridos)++;
        }
    }
}

// cria outro arquivo veiculo ordenado com qsort
void ordenarVeiculo(){
    RegistroCabVeiculo cabVeiculo = inicializarCabVeiculo();
    RegistroDadosVeiculo *vetor = malloc(LIMIT*sizeof(RegistroDadosVeiculo));

    char veiculoNomeAntes[30];
    char veiculoNomeDepois[30];
    char buffer[30];

    scanf(" %s %s %s", veiculoNomeAntes, veiculoNomeDepois, buffer);

    FILE *veiculoDesordenado = abrirBinarioLeitura(veiculoNomeAntes);
    FILE *veiculoOrdenado = abrirBinarioEscrita(veiculoNomeDepois);

    lerCabecalhoVeiculo(veiculoDesordenado, &cabVeiculo);
    
    fseek(veiculoOrdenado, 175, SEEK_SET);
    
    int numRegInseridos = 0;
    // ordena e escreve no outro arquivo
    quickSortVeiculo(cabVeiculo.nroRegistros+cabVeiculo.nroRegRemovidos, veiculoDesordenado, vetor);
    escreverVetVeiculo(&numRegInseridos, cabVeiculo.nroRegistros+cabVeiculo.nroRegRemovidos, veiculoOrdenado, vetor);

    cabVeiculo.nroRegistros = numRegInseridos;
    cabVeiculo.nroRegRemovidos = 0;
    cabVeiculo.byteProxReg = ftell(veiculoOrdenado);

    fseek(veiculoOrdenado, 0, SEEK_SET);
    escreverCabecalhoVeiculo(veiculoOrdenado, cabVeiculo);

    fclose(veiculoDesordenado);
    fclose(veiculoOrdenado);
    free(vetor);
    binarioNaTela(veiculoNomeDepois);

}
// cria outro arquivo veiculo ordenado com qsort
void ordenarLinha(){
    RegistroCabLinha cabLinha = inicializarCabLinha();
    RegistroDadosLinha *vetor = malloc(LIMIT*sizeof(RegistroDadosLinha));

    char linhaNomeAntes[30];
    char linhaNomeDepois[30];
    char buffer[30];
    
    scanf(" %s %s %s", linhaNomeAntes, linhaNomeDepois, buffer);

    FILE *linhaDesordenado = abrirBinarioLeitura(linhaNomeAntes);
    FILE *linhaOrdenado = abrirBinarioEscrita(linhaNomeDepois);

    lerCabecalhoLinha(linhaDesordenado, &cabLinha);
    
    fseek(linhaOrdenado, 82, SEEK_SET);
    
    int numRegInseridos = 0;
    // ordena e escreve no outro arquivo
    quickSortLinha(cabLinha.nroRegistros+cabLinha.nroRegRemovidos, linhaDesordenado, vetor);
    escreverVetLinha(&numRegInseridos, cabLinha.nroRegistros+cabLinha.nroRegRemovidos, linhaOrdenado, vetor);
    // atualiza o cabeçalho
    cabLinha.nroRegistros = numRegInseridos;
    cabLinha.nroRegRemovidos = 0;
    cabLinha.byteProxReg = ftell(linhaOrdenado);

    fseek(linhaOrdenado, 0, SEEK_SET);
    escreverCabecalhoLinha(linhaOrdenado, cabLinha);

    fclose(linhaDesordenado);
    fclose(linhaOrdenado);
    free(vetor);
    binarioNaTela(linhaNomeDepois);
}

// função ordena intercalação
void juncaoOrdenacaoIntercalacao(){
    RegistroCabLinha cabLinha = inicializarCabLinha();
    RegistroCabVeiculo cabVeiculo = inicializarCabVeiculo();
    RegistroDadosLinha *vertL = malloc(LIMIT*sizeof(RegistroDadosLinha));
    RegistroDadosVeiculo *vertV = malloc(LIMIT*sizeof(RegistroDadosVeiculo));

    char linhaNome[30];
    char veiculoNome[30];
    char buffer[30];

    scanf(" %s %s %s %s", veiculoNome, linhaNome, buffer, buffer);

    FILE *arqVeiculo = abrirBinarioLeitura(veiculoNome);
    FILE *arqLinha = abrirBinarioLeitura(linhaNome);
    
    lerCabecalhoVeiculo(arqVeiculo, &cabVeiculo);
    lerCabecalhoLinha(arqLinha, &cabLinha);

    fseek(arqVeiculo, 175, SEEK_SET);
    fseek(arqLinha, 82, SEEK_SET);
    // ordena em dois vetores
    quickSortVeiculo(cabVeiculo.nroRegistros+cabVeiculo.nroRegRemovidos, arqVeiculo, vertV);
    quickSortLinha(cabLinha.nroRegistros+cabLinha.nroRegRemovidos, arqLinha, vertL);
    // fecha arquivo
    fclose(arqVeiculo);
    fclose(arqLinha);
    // abre para leitura
    arqVeiculo = abrirBinarioEscrita(veiculoNome);
    arqLinha = abrirBinarioEscrita(linhaNome);
    // pula para a posição do primeiro registro
    fseek(arqVeiculo, 175, SEEK_SET);
    fseek(arqLinha, 82, SEEK_SET);

    int numRegInsertV = 0, numRegInsertL = 0;
    // escreve os vetores
    escreverVetVeiculo(&numRegInsertV, cabVeiculo.nroRegistros+cabVeiculo.nroRegRemovidos, arqVeiculo, vertV);
    escreverVetLinha(&numRegInsertL, cabLinha.nroRegistros+cabLinha.nroRegRemovidos, arqLinha, vertL);
    // atualiza o cabeçalho
    cabVeiculo.nroRegistros = numRegInsertV;
    cabVeiculo.nroRegRemovidos = 0;
    cabVeiculo.byteProxReg = ftell(arqVeiculo);

    cabLinha.nroRegistros = numRegInsertL;
    cabLinha.nroRegRemovidos = 0;
    cabLinha.byteProxReg = ftell(arqLinha);

    fseek(arqVeiculo, 0, SEEK_SET);
    escreverCabecalhoVeiculo(arqVeiculo, cabVeiculo);
    
    fseek(arqLinha, 0, SEEK_SET);
    escreverCabecalhoLinha(arqLinha, cabLinha);

    fseek(arqVeiculo, 175, SEEK_SET);
    fseek(arqLinha, 82, SEEK_SET);

    int existeIgualdade = 0;
    int tamVeiculo = cabVeiculo.nroRegistros + cabVeiculo.nroRegRemovidos;
    int tamLinha = cabLinha.nroRegistros + cabLinha.nroRegRemovidos;

    RegistroDadosLinha linha = inicializarDadosLinha();
    RegistroDadosVeiculo veiculo = inicializarDadosVeiculo();
    // verifica se os arquivos não estão vazios
    if(tamLinha > 0 && tamVeiculo > 0){
        lerDadosVeiculo(arqVeiculo, &veiculo);
        lerDadosLinha(arqLinha, &linha);
    }else{
        printf("Registro inexistente.\n");
        return;
    }

    int l = 0, v = 0;
    // enquanto não terminar nenhum dos arquivos, roda
    while(v != tamVeiculo && l != tamLinha){
    
        if(veiculo.codLinha == linha.codLinha){ // se possuirem registros com codlinha iguais, imprime
            imprimirSelectVeiculo(veiculo, cabVeiculo);
            imprimirSelectLinha(linha, cabLinha);

            lerDadosVeiculo(arqVeiculo, &veiculo);
    
            existeIgualdade = 1;
            v++; // avança veiculo
        }else if(veiculo.codLinha > linha.codLinha){ // codlinha de linha menor, avança linha
            lerDadosLinha(arqLinha, &linha);
            l++;
        }else if(veiculo.codLinha < linha.codLinha){ // codlinha de veiculo menor, avança veiculo
            lerDadosVeiculo(arqVeiculo, &veiculo);
            v++;
        }

    }

    if(!existeIgualdade)
        printf("Registro inexistente.\n");

    fclose(arqVeiculo);
    fclose(arqLinha);
    free(vertV);
    free(vertL);
}