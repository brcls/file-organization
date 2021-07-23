/*
    Johnny Silva | 11371350
    Erick Barcelos | 11345562
*/

#include "dados.h"


RegistroCabLinha inicializarCabLinha(){
	RegistroCabLinha linha;
	linha.status = '0';
	linha.byteProxReg = 0;
	linha.nroRegistros = 0;
	linha.nroRegRemovidos = 0;
	strcpy(linha.descreveCodigo, "\0");
	strcpy(linha.descreveCartao, "\0");
	strcpy(linha.descreveNome, "\0");
	strcpy(linha.descreveCor, "\0");
	return linha;
}

RegistroDadosLinha inicializarDadosLinha(){
	RegistroDadosLinha linha;
	linha.removido = '1';
	linha.tamanhoRegistro = 0;
	linha.codLinha = 0;
	linha.aceitaCartao = 'N';
	linha.tamanhoNome = 0;
	linha.nomeLinha = NULL;
	linha.tamanhoCor = 0;
	linha.corLinha = NULL;
	return linha;
}

//Soma o tamanho do registo
int somarTamanhoDoRegLinha(RegistroDadosLinha dados){
	int aux = 13;
	aux += dados.tamanhoNome + dados.tamanhoCor;
	return aux;
}

//Função que lê apenas o cabeçalho do csv da linha
RegistroCabLinha pegarCabCSVLinha(char line[300]){

	RegistroCabLinha aux;
	
	aux.status = '0'; //Começa o status com 0, será mudado posteriormente caso dê tudo certo
	aux.nroRegistros = 0;
	aux.nroRegRemovidos = 0;
	aux.byteProxReg = 0;

	int count = 0;  //é usado para saber qual termo estamos lendo com o strtok

	//for que lê cada parte(de virgula em virgula) da linha até encontrar o \n
	for (char *token = strtok(line, ","); token && *token; token = strtok(NULL, ",\n")){
		switch (count){
			case 0:
				strcpy(aux.descreveCodigo, token);				
				break;
			case 1:
				strcpy(aux.descreveCartao, token);				
				break;
			case 2:
				strcpy(aux.descreveNome, token);
				break;
			case 3:
				strcpy(aux.descreveCor, token);
				break;
		}
		count++;
	}

    return aux; //retorna o cabeçalho lido
}

//Função que lê os dados do csv da linha
RegistroDadosLinha pegarDadosCSVLinha(char line[100], FILE *arquivo, RegistroCabLinha *cabecalho){
 
	RegistroDadosLinha aux;
	
	int count = 0; //é usado para saber qual termo estamos lendo com o strtok

	//for que lê cada parte(de virgula em virgula) da linha até encontrar o \n
	for (char *token = strtok(line, ","); token && *token; token = strtok(NULL, ",\n")){
		switch (count){
		case 0:
			//Primeiro verificamos se está removido para acrescentar a info ao cabeçalho
			verificarRemovidoLinha(token, &aux.removido, cabecalho);
			aux.codLinha = atoi(token); //Codlinha não pode admitir valores nulos
			break;
		case 1:
			if (!(strcmp(token, "NULO"))){
				aux.aceitaCartao = '\0';
				break;
			}
			aux.aceitaCartao = token[0];
			break;
		case 2:
			if(!(strcmp(token, "NULO"))){
				aux.tamanhoNome = 0;
				break;	
			}
			aux.tamanhoNome = strlen(token); //Mede o tamanho do dado para alocar o tamanho certo para o nome
			aux.nomeLinha = (char*)calloc(aux.tamanhoNome+1, sizeof(char));
			memset(aux.nomeLinha, 0, aux.tamanhoNome+1);
			strcpy(aux.nomeLinha, token);
			break;
		case 3:
			if(!(strcmp(token, "NULO"))){
				aux.tamanhoCor = 0;
				break;	
			}
			aux.tamanhoCor = strlen(token); //Mede o tamanho do dado para alocar o tamanho certo para a cor
			aux.corLinha = (char*)calloc(aux.tamanhoCor+1, sizeof(char));
			memset(aux.corLinha, 0, aux.tamanhoCor+1);
			strcpy(aux.corLinha, token);
			break;	
		}
		count++;
	}

	aux.tamanhoRegistro = somarTamanhoDoRegLinha(aux); //Somar o tamanho do registro
    return aux; //retorna os dados lidos
}

//Função que escreve o cabeçalho no arquivo
void escreverCabecalhoLinha(FILE *arquivo, RegistroCabLinha cabecalho){
	fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
	fwrite(&cabecalho.byteProxReg, sizeof(long long int), 1, arquivo);
	fwrite(&cabecalho.nroRegistros, sizeof(int), 1, arquivo);
	fwrite(&cabecalho.nroRegRemovidos, sizeof(int), 1, arquivo);
	fwrite(cabecalho.descreveCodigo, sizeof(char), 15, arquivo);
	fwrite(cabecalho.descreveCartao, sizeof(char), 13, arquivo);
	fwrite(cabecalho.descreveNome, sizeof(char), 13, arquivo);
	fwrite(cabecalho.descreveCor, sizeof(char), 24, arquivo);
}

//Função que escreve os dados no arquivo
void escreverDadosLinha(FILE *arquivo, RegistroDadosLinha dados){
	fwrite(&dados.removido, sizeof(char), 1, arquivo);	
	fwrite(&dados.tamanhoRegistro, sizeof(int), 1, arquivo);	
	fwrite(&dados.codLinha, sizeof(int), 1, arquivo);	
	fwrite(&dados.aceitaCartao, sizeof(char), 1, arquivo);	
	fwrite(&dados.tamanhoNome, sizeof(int), 1, arquivo);
	fwrite(dados.nomeLinha, sizeof(char), dados.tamanhoNome, arquivo);	
	fwrite(&dados.tamanhoCor, sizeof(int), 1, arquivo);
	fwrite(dados.corLinha, sizeof(char), dados.tamanhoCor, arquivo);	
}

//Função para criar a tabela no arquivo
void createTableLinha(){
	RegistroCabLinha cabecalho;
	
	char line[100]; //line sãos as linhas do csv
	int count = 0;
	char nomeCSV[25];
	char nomeBin[25];
	scanf(" %s", nomeCSV);
	scanf(" %s", nomeBin);

	FILE *arquivoCSV = abrirCSVLeitura(nomeCSV);
	FILE *arquivoBin = abrirBinarioEscrita(nomeBin);
	
	fgets(line, 100, arquivoCSV);
	cabecalho = pegarCabCSVLinha(line);
	escreverCabecalhoLinha(arquivoBin, cabecalho);
	
	fseek(arquivoBin, 82, SEEK_SET);

	while (fgets(line, 100, arquivoCSV))
		escreverDadosLinha(arquivoBin, pegarDadosCSVLinha(line, arquivoCSV, &cabecalho));

	cabecalho.byteProxReg = ftell(arquivoBin);
	cabecalho.status = '1';

	fseek(arquivoBin, 0, SEEK_SET);
	escreverCabecalhoLinha(arquivoBin, cabecalho);

	fclose(arquivoCSV);
	fclose(arquivoBin);

	binarioNaTela(nomeBin);
}

//Função para ler o cabeçalho de um arquivo
void lerCabecalhoLinha(FILE *arquivo, RegistroCabLinha *cabecalho){
	fread(&cabecalho->status, sizeof(char), 1, arquivo);
	fread(&cabecalho->byteProxReg, sizeof(long long int), 1, arquivo);
	fread(&cabecalho->nroRegistros, sizeof(int), 1, arquivo);
	fread(&cabecalho->nroRegRemovidos, sizeof(int), 1, arquivo);
	fread(&cabecalho->descreveCodigo, sizeof(char), 15, arquivo);
	fread(&cabecalho->descreveCartao, sizeof(char), 13, arquivo);
	fread(&cabecalho->descreveNome, sizeof(char), 13, arquivo);
	fread(&cabecalho->descreveCor, sizeof(char), 24, arquivo);

	if(cabecalho->status == '0')
	{//Se o arquivo estiver corrompido, informa e para a execução
		printf("Falha no processamento do arquivo.");
		exit(0);
	}

}

//Ler dados do arquivo
int lerDadosLinha(FILE *arquivo, RegistroDadosLinha *dados){
	int final = 0;
	fread(&dados->removido, sizeof(char), 1, arquivo);
	fread(&dados->tamanhoRegistro, sizeof(int), 1, arquivo);	
	fread(&dados->codLinha, sizeof(int), 1, arquivo);	
	fread(&dados->aceitaCartao, sizeof(char), 1, arquivo);

	fread(&dados->tamanhoNome, sizeof(int), 1, arquivo);	
	dados->nomeLinha = (char*)calloc(dados->tamanhoNome+1, sizeof(char));
	fread(dados->nomeLinha, sizeof(char), dados->tamanhoNome, arquivo);

	fread(&dados->tamanhoCor, sizeof(int), 1, arquivo);
	dados->corLinha = (char*)calloc(dados->tamanhoCor+1, sizeof(char));
	final = fread(dados->corLinha, sizeof(char), dados->tamanhoCor, arquivo);
	
	
	return final;
}

//Função para imprimir de forma apropriada os dados
int imprimirSelectLinha(RegistroDadosLinha dados, RegistroCabLinha cabecalho){
	// Verificar primeiro se o registro está removido
	if(dados.removido == '0')
		return 0;

	printf("%.*s: %d\n", 15, cabecalho.descreveCodigo, dados.codLinha);
	if (!dados.tamanhoNome)
		printf("%.*s: %s\n", 13, cabecalho.descreveNome, "campo com valor nulo");	
	else
		printf("%.*s: %.*s\n", 13, cabecalho.descreveNome, dados.tamanhoNome, dados.nomeLinha);
	
	if (!dados.tamanhoCor)
		printf("%.*s: %s\n", 24, cabecalho.descreveCor, "campo com valor nulo");
	else
		printf("%.*s: %.*s\n", 24, cabecalho.descreveCor, dados.tamanhoCor, dados.corLinha);
	
	switch (dados.aceitaCartao) {
	case 'S':
		printf("%.*s: %s\n", 13, cabecalho.descreveCartao, "PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR");
		break;
	case 'N':
		printf("%.*s: %s\n", 13, cabecalho.descreveCartao, "PAGAMENTO EM CARTAO E DINHEIRO");
		break;
	case 'F':
		printf("%.*s: %s\n", 13, cabecalho.descreveCartao, "PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA");
		break;
	default:
		printf("%.*s: %s\n", 13, cabecalho.descreveCartao, "campo com valor nulo");
		break;
	}
				
	printf("\n");
	
	return 1;
}

//Função que para ler todos os dados do arquivo e exibir
void selectLinha(){
	char nomeBin[30];
	scanf(" %s", nomeBin);
	FILE *arquivoBin = abrirBinarioLeitura(nomeBin);
	RegistroCabLinha cabecalho;
	RegistroDadosLinha dados;
	int final = 0, achados = 0;
	lerCabecalhoLinha(arquivoBin, &cabecalho);

	//For infino que só para após lê e exibir tudo
	for(;;){
		final = lerDadosLinha(arquivoBin, &dados);
		if(!final) //Se chegar no final = 0, finaliza o for
			break;
		//Toda vez que achar adiciona aos achados, se no final ele for zero, retorna nenhum registro
		achados += imprimirSelectLinha(dados, cabecalho);
	}

	if (!achados)
		printf("Registro inexistente.\n");
	
}

//Função auxiliar para verificar qual campo foi requisitado para comparação
int igualdadeCamposLinha(RegistroDadosLinha dados, char nomeDoCampo[50], char valor[50]){

	if(!strcmp(nomeDoCampo, "codLinha")){
		int aux = atoi(valor);
		if(dados.codLinha == aux)
			return 1;
	}else if(!strcmp(nomeDoCampo, "aceitaCartao")){
		dados.aceitaCartao = valor[0];
			return 1;
	}else if(!strcmp(nomeDoCampo, "nomeLinha")){
		if(!strncmp(dados.nomeLinha, valor, dados.tamanhoNome))
			return 1;
	}else if(!strcmp(nomeDoCampo, "corLinha")){
		if(!strncmp(dados.corLinha, valor, dados.tamanhoCor))
			return 1;
	}
	return 0;
}

//Função que lê os dados conforme instrução requisitada 
void whereLinha(){
	char nomeBin[30];
	scanf(" %s", nomeBin);
	FILE *arquivoBin = abrirBinarioLeitura(nomeBin);

	RegistroCabLinha cabecalho;
	RegistroDadosLinha dados;
	
	char nomeDoCampo[50];
	char valor[50];
	int final = 0, achados = 0;

	scanf(" %s", nomeDoCampo);
	scan_quote_string(valor);

	lerCabecalhoLinha(arquivoBin, &cabecalho);
	
	for(;;)
	{
		final = lerDadosLinha(arquivoBin, &dados);

		if(!final)
			break;
		
		//Comparamos primeiro para ver se a instrução coincide
		if(igualdadeCamposLinha(dados, nomeDoCampo, valor))
		//Toda vez que achar adiciona aos achados, se no final ele for zero, retorna nenhum registro
			achados += imprimirSelectLinha(dados, cabecalho);
	}

	if (!achados)
		printf("Registro inexistente.\n");
	
}

//Função que adiciona os dados inseridos pelo usuário
RegistroDadosLinha entrarDadosInputLinha(){
	RegistroDadosLinha dados;

	//Alocamos primeiro para podermos ler e copiar, sera dado free posteriormente
	char *auxiliarCodLinha = calloc(10, sizeof(char));
	dados.nomeLinha = calloc(50, sizeof(char));
	dados.corLinha = calloc(50, sizeof(char));
	
	scan_quote_string(auxiliarCodLinha);
	scan_quote_string(&dados.aceitaCartao);
	scan_quote_string(dados.nomeLinha);
	scan_quote_string(dados.corLinha);

	dados.codLinha = atoi(auxiliarCodLinha);

	if(&dados.aceitaCartao == "")
		dados.aceitaCartao = '\0';

	if(!strcmp(dados.nomeLinha, ""))
	{
		dados.tamanhoNome = 0;
		free(dados.nomeLinha);
	}else{
		dados.tamanhoNome = strlen(dados.nomeLinha);
	}

	if(!strcmp(dados.corLinha, ""))
	{
		dados.tamanhoCor = 0;
		free(dados.corLinha);
	}else{
		dados.tamanhoCor = strlen(dados.corLinha);
	}

	dados.tamanhoRegistro = somarTamanhoDoRegLinha(dados);
	dados.removido = '1';
	
	//não disse?
	free(auxiliarCodLinha);

 	return dados; //retorna a struct para ser adicionada
}

//Função que insere no arquivo dados manuais
void insertIntoLinha(){
	RegistroCabLinha cabecalho;
	char nomeBin[30];
	int n; //n é o número de inserções a serem realizadas
	
	scanf(" %s", nomeBin);

	FILE *arquivoBin = abrirBinarioLeitura(nomeBin);

	scanf("%d", &n);

	lerCabecalhoLinha(arquivoBin, &cabecalho);
	fseek(arquivoBin, 0, SEEK_END);

	cabecalho.status = '0';

	for(int i = 0; i < n; i++){
		//Escreveremos os dados capturados pelo entrarDadosInput
		escreverDadosLinha(arquivoBin, entrarDadosInputLinha());
		cabecalho.nroRegistros++;
	}

	cabecalho.byteProxReg = ftell(arquivoBin);
	cabecalho.status = '1'; //Se tudo der certo, alteramos o status

	fseek(arquivoBin, 0, SEEK_SET);
	escreverCabecalhoLinha(arquivoBin, cabecalho);

	fclose(arquivoBin);

	binarioNaTela(nomeBin);
}

//Função que busca registro na arvore b
void buscarRegistroLinha() {
	RegCabBtree cabBtree = inicializarCabecalho(); //Inicializar o cabeçalho da arvore
	RegistroCabLinha cab = inicializarCabLinha(); //Inicializar o cabeçalho do Linha
	RegistroDadosLinha dados = inicializarDadosLinha();
	int chave = 0;
	long long int foundRegRRN = 0; 
	char nomeArqLinha[30], nomeArqArvoreB[30], buffer[30];
	
	scanf(" %s %s %s", nomeArqLinha, nomeArqArvoreB, buffer);
	scanf("%d", &chave);

	FILE *linhaBin = abrirBinarioLeitura(nomeArqLinha);
	FILE *arqBtreeIndex = abrirBinarioLeitura(nomeArqArvoreB);
	
	lerCabecalhoArvoreB(arqBtreeIndex, &cabBtree);
	lerCabecalhoLinha(linhaBin, &cab);

	//Vamos chamar a função de buscar recursivamente e testaremos se o registro foi encontrado
	if(!buscarArvoreB(arqBtreeIndex, cabBtree.noRaiz, chave, &foundRegRRN))
	{//Se não retornar nada, então não temos registros
		printf("Registro inexistente.");
		return; //fecha a função
	}

	fseek(linhaBin, foundRegRRN, SEEK_SET);

	//ler e imprimir dados resultantes
	lerDadosLinha(linhaBin, &dados);
	imprimirSelectLinha(dados, cab);

	//fechando ambos arquivos :D
	fclose(linhaBin);
	fclose(arqBtreeIndex);
}

//Função para criar arquivo de indice
void driverLinhaVazio(){
	RegCabBtree cabBtree = inicializarCabecalho();
	RegistroCabLinha cab = inicializarCabLinha(); 
	char nomeArqLinha[30], nomeArqArvoreB[30];

	scanf(" %s %s", nomeArqLinha, nomeArqArvoreB);

	FILE *linhaBin = abrirBinarioLeitura(nomeArqLinha);
	FILE *arqBtreeIndex = abrirBinarioEscrita(nomeArqArvoreB);;
	
	lerCabecalhoLinha(linhaBin, &cab);

	int n = 0, raiz = 0, chave = 0;
	long long int valor = 0;

	RegistroDadosLinha dados = inicializarDadosLinha();

	for(int i = 0; i < (cab.nroRegRemovidos+cab.nroRegistros); i++){
		valor = ftell(linhaBin);
		lerDadosLinha(linhaBin, &dados);

		if(dados.removido == '0') //Se o dado não for removido, continuaremos
			continue;

		chave = dados.codLinha;
		raiz = cabBtree.noRaiz;

		//driver pra iniciar inserção
		inserirDriver(&cabBtree, arqBtreeIndex, &raiz, valor, chave);
	}

	cabBtree.status = '1';
	cabBtree.noRaiz = raiz;
	
	cab.status = '1'; //Se tudo der certo, alteramos o status
	cab.byteProxReg = ftell(linhaBin);

	fseek(arqBtreeIndex, 0, SEEK_SET);
	fseek(linhaBin, 0, SEEK_SET);

	escreverCabecalhoArvoreB(arqBtreeIndex, cabBtree);
	escreverCabecalhoLinha(linhaBin, cab);

	fclose(linhaBin);
	fclose(arqBtreeIndex);
	
	binarioNaTela(nomeArqArvoreB); //exibindo resultado do binario
}

//Função que insere ao final do arquivo indice com entrada via input
void driverLinhaCheio(){
	RegCabBtree cabBtree = inicializarCabecalho();
	RegistroCabLinha cab = inicializarCabLinha(); 
	char nomeArqLinha[30], nomeArqArvoreB[30];

	scanf(" %s %s", nomeArqLinha, nomeArqArvoreB);

	FILE *linhaBin = abrirBinarioLeitura(nomeArqLinha);
	FILE *arqBtreeIndex = abrirBinarioLeitura(nomeArqArvoreB);
	
	lerCabecalhoLinha(linhaBin, &cab);

	int n = 0, raiz = 0, chave = 0;
	long long int valor = 0;

	RegistroDadosLinha dados = inicializarDadosLinha();
	
	fseek(linhaBin, 0, SEEK_END);

	lerCabecalhoArvoreB(arqBtreeIndex, &cabBtree);
	
	scanf("%d", &n);

	for(int i = 0; i < n; i++){//Escreveremos os dados capturados pelo entrarDadosInputLinha
		
		dados = entrarDadosInputLinha();
		
		valor = ftell(linhaBin);
		chave = dados.codLinha;
		raiz = cabBtree.noRaiz;

		//driver pra iniciar inserção
		inserirDriver(&cabBtree, arqBtreeIndex, &raiz, valor, chave);

		escreverDadosLinha(linhaBin, dados);
		cab.nroRegistros++;
	}

	cabBtree.status = '1';
	cabBtree.noRaiz = raiz;
	
	cab.status = '1'; //Se tudo der certo, alteramos o status
	cab.byteProxReg = ftell(linhaBin);

	fseek(arqBtreeIndex, 0, SEEK_SET);
	fseek(linhaBin, 0, SEEK_SET);

	escreverCabecalhoArvoreB(arqBtreeIndex, cabBtree);
	escreverCabecalhoLinha(linhaBin, cab);

	fclose(linhaBin);
	fclose(arqBtreeIndex);
	
	binarioNaTela(nomeArqArvoreB); //exibindo resultado do binario
}