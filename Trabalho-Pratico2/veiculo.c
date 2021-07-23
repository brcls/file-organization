/*
    Johnny Silva | 11371350
    Erick Barcelos | 11345562
*/
#include "dados.h"

RegistroCabVeiculo inicializarCabVeiculo(){
	RegistroCabVeiculo veic;
	veic.status = '0';
	veic.byteProxReg = 0;
	veic.nroRegistros = 0;
	veic.nroRegRemovidos = 0;
	strcpy(veic.descrevePrefixo, "\0");
	strcpy(veic.descreveData, "\0");
	strcpy(veic.descreveLugares, "\0");
	strcpy(veic.descreveLinha, "\0");
	strcpy(veic.descreveModelo, "\0");
	strcpy(veic.descreveCategoria, "\0");
	return veic;
}

RegistroDadosVeiculo inicializarDadosVeiculo(){
	RegistroDadosVeiculo veic;
	veic.removido = '1';
	veic.tamanhoRegistro = 0;
	strcpy(veic.prefixo, "\0");
	strcpy(veic.data, "\0");
	veic.quantidadeLugares = 0;
	veic.codLinha = 0;
	veic.tamanhoModelo = 0;
	veic.modelo = NULL;
	veic.tamanhoCategoria = 0;
	veic.categoria = NULL;
	return veic;
}

//Soma o tamanho do registo
int somarTamanhoRegVeiculo(RegistroDadosVeiculo dados){
	int aux = 31;
	aux += dados.tamanhoModelo + dados.tamanhoCategoria;
	return aux;
}

//Função que lê apenas o cabeçalho do csv do veiculo
RegistroCabVeiculo pegarCabCSVVeiculo(char line[300]){
	RegistroCabVeiculo aux;
	
	aux.status = '0'; //Começa o status com 0, será mudado posteriormente caso dê tudo certo
	aux.nroRegistros = 0;
	aux.nroRegRemovidos = 0;
	aux.byteProxReg = 0;

	int count = 0; //é usado para saber qual termo estamos lendo com o strtok

	//for que lê cada parte(de virgula em virgula) da linha até encontrar o \n
	for (char *token = strtok(line, ","); token && *token; token = strtok(NULL, ",\n")){
		switch (count){
			case 0:
				strcpy(aux.descrevePrefixo, token);				
				break;
			case 1:
				strcpy(aux.descreveData, token);				
				break;
			case 2:
				strcpy(aux.descreveLugares, token);
				break;
			case 3:
				strcpy(aux.descreveLinha, token);
				break;
			case 4:
				strcpy(aux.descreveModelo, token);
				break;
			case 5:
				strcpy(aux.descreveCategoria, token);
				break;	
		}
		count++;
	}

    return aux; //retorna o cabeçalho lido
}

//Função que lê os dados do csv do veiculo
RegistroDadosVeiculo pegarDadosCSVVeiculo(char line[300], FILE *arquivo, RegistroCabVeiculo *cabecalho){
 
	RegistroDadosVeiculo aux;
	
	int count = 0; //é usado para saber qual termo estamos lendo com o strtok

	//for que lê cada parte(de virgula em virgula) da linha até encontrar o \n
	for (char *token = strtok(line, ","); token && *token; token = strtok(NULL, ",\n")){
		switch (count){
		case 0:
			//Primeiro verificamos se está removido para acrescentar a info ao cabeçalho
			verificarRemovidoVeiculo(token, &aux.removido, cabecalho);
			strcpy(aux.prefixo, token); //Prefixo não pode admitir valores nulos
			break;
		case 1:
			if (!(strcmp(token, "NULO"))){
				strcpy(aux.data, "@@@@@@@@@@");
				aux.data[0] = '\0';
				break;
			}
			strcpy(aux.data, token);
			break;
		case 2:
			if (!(strcmp(token, "NULO"))){
				aux.quantidadeLugares = -1;	
				break;
			}
			aux.quantidadeLugares = atoi(token);
			break;
		case 3:
			if (!(strcmp(token, "NULO"))){
				aux.codLinha = -1;	
				break;
			}
			aux.codLinha = atoi(token);
			break;
		case 4:
			if(!(strcmp(token, "NULO"))){
				aux.tamanhoModelo = 0;
				break;	
			}
			aux.tamanhoModelo = strlen(token); //Mede o tamanho do dado para alocar o tamanho certo para o modelo
			aux.modelo = (char*)calloc(aux.tamanhoModelo+1, sizeof(char));
			memset(aux.modelo, 0, aux.tamanhoModelo+1);
			strcpy(aux.modelo, token);
			break;
		case 5:
			if(!(strcmp(token, "NULO"))){
				aux.tamanhoCategoria = 0;
				break;	
			}
			aux.tamanhoCategoria = strlen(token); //Mede o tamanho do dado para alocar o tamanho certo para a categoria
			aux.categoria = (char*)calloc(aux.tamanhoCategoria+1, sizeof(char));
			memset(aux.categoria, 0, aux.tamanhoCategoria+1);
			strcpy(aux.categoria, token);
			break;	
		}
		count++;
	}

	aux.tamanhoRegistro = somarTamanhoRegVeiculo(aux); //Somar o tamanho do registro
    return aux; //retorna os dados lidos
}


//Função que escreve o cabeçalho no arquivo
void escreverCabecalhoVeiculo(FILE *arquivo, RegistroCabVeiculo cabecalho){
	fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
	fwrite(&cabecalho.byteProxReg, sizeof(long long int), 1, arquivo);
	fwrite(&cabecalho.nroRegistros, sizeof(int), 1, arquivo);
	fwrite(&cabecalho.nroRegRemovidos, sizeof(int), 1, arquivo);
	fwrite(cabecalho.descrevePrefixo, sizeof(char), 18, arquivo);
	fwrite(cabecalho.descreveData, sizeof(char), 35, arquivo);
	fwrite(cabecalho.descreveLugares, sizeof(char), 42, arquivo);
	fwrite(cabecalho.descreveLinha, sizeof(char), 26, arquivo);
	fwrite(cabecalho.descreveModelo, sizeof(char), 17, arquivo);
	fwrite(cabecalho.descreveCategoria, sizeof(char), 20, arquivo);
}

//Função que escreve os dados no arquivo
void escreverDadosVeiculo(FILE *arquivo, RegistroDadosVeiculo dados){
	fwrite(&dados.removido, sizeof(char), 1, arquivo);	
	fwrite(&dados.tamanhoRegistro, sizeof(int), 1, arquivo);	
	fwrite(dados.prefixo, sizeof(char), 5, arquivo);	
	fwrite(dados.data, sizeof(char), 10, arquivo);	
	fwrite(&dados.quantidadeLugares, sizeof(int), 1, arquivo);	
	fwrite(&dados.codLinha, sizeof(int), 1, arquivo);	
	fwrite(&dados.tamanhoModelo, sizeof(int), 1, arquivo);
	fwrite(dados.modelo, sizeof(char), dados.tamanhoModelo, arquivo);	
	fwrite(&dados.tamanhoCategoria, sizeof(int), 1, arquivo);
	fwrite(dados.categoria, sizeof(char), dados.tamanhoCategoria, arquivo);	
}

//Função para criar a tabela no arquivo
void createTableVeiculo(){
	RegistroCabVeiculo cabecalho;
	
	char line[300]; //line sãos as linhas do csv
	char nomeCSV[25];
	char nomeBin[25];
	scanf(" %s", nomeCSV);
	scanf(" %s", nomeBin);

	FILE *arquivoCSV = abrirCSVLeitura(nomeCSV);
	FILE *arquivoBin = abrirBinarioEscrita(nomeBin);
	
	fgets(line, 300, arquivoCSV);
	cabecalho = pegarCabCSVVeiculo(line);
	escreverCabecalhoVeiculo(arquivoBin, cabecalho);

	fseek(arquivoBin, 175, SEEK_SET);

	while (fgets(line, 300, arquivoCSV))
		escreverDadosVeiculo(arquivoBin, pegarDadosCSVVeiculo(line, arquivoCSV, &cabecalho));

	cabecalho.byteProxReg = ftell(arquivoBin);
	cabecalho.status = '1';

	fseek(arquivoBin, 0, SEEK_SET);
	escreverCabecalhoVeiculo(arquivoBin, cabecalho);

	fclose(arquivoCSV);
	fclose(arquivoBin);

	binarioNaTela(nomeBin);
}

//Função para ler o cabeçalho de um arquivo
void lerCabecalhoVeiculo(FILE *arquivo, RegistroCabVeiculo *cabecalho){
	fread(&cabecalho->status, sizeof(char), 1, arquivo);
	fread(&cabecalho->byteProxReg, sizeof(long long int), 1, arquivo);
	fread(&cabecalho->nroRegistros, sizeof(int), 1, arquivo);
	fread(&cabecalho->nroRegRemovidos, sizeof(int), 1, arquivo);
	fread(&cabecalho->descrevePrefixo, sizeof(char), 18, arquivo);
	fread(&cabecalho->descreveData, sizeof(char), 35, arquivo);
	fread(&cabecalho->descreveLugares, sizeof(char), 42, arquivo);
	fread(&cabecalho->descreveLinha, sizeof(char), 26, arquivo);
	fread(&cabecalho->descreveModelo, sizeof(char), 17, arquivo);
	fread(&cabecalho->descreveCategoria, sizeof(char), 20, arquivo);

	if(cabecalho->status == '0')
	{ //Se o arquivo estiver corrompido, informa e para a execução
		printf("Falha no processamento do arquivo.");
		exit(0);
	}
}

//Ler dados do arquivo
int lerDadosVeiculo(FILE *arquivo, RegistroDadosVeiculo *dados){
	int final = 0;
	fread(&dados->removido, sizeof(char), 1, arquivo);
	fread(&dados->tamanhoRegistro, sizeof(int), 1, arquivo);	
	fread(&dados->prefixo, sizeof(char), 5, arquivo);	
	fread(&dados->data, sizeof(char), 10, arquivo);	
	fread(&dados->quantidadeLugares, sizeof(int), 1, arquivo);	
	fread(&dados->codLinha, sizeof(int), 1, arquivo);	

	fread(&dados->tamanhoModelo, sizeof(int), 1, arquivo);	
	dados->modelo = (char*)calloc(dados->tamanhoModelo+1, sizeof(char));
	fread(dados->modelo, sizeof(char), dados->tamanhoModelo, arquivo);

	fread(&dados->tamanhoCategoria, sizeof(int), 1, arquivo);
	dados->categoria = (char*)calloc(dados->tamanhoCategoria+1, sizeof(char));
	final = fread(dados->categoria, sizeof(char), dados->tamanhoCategoria, arquivo);
	
	return final;
}

//Função para imprimir de forma apropriada os dados
int imprimirSelectVeiculo(RegistroDadosVeiculo dados, RegistroCabVeiculo cabecalho){
	// Verificar primeiro se o registro está removido
	if(dados.removido == '0')
		return 0;

	printf("%.*s: %.*s\n", 18, cabecalho.descrevePrefixo, 5, dados.prefixo);
	if (!dados.tamanhoModelo)
		printf("%.*s: %s\n", 17, cabecalho.descreveModelo, "campo com valor nulo");	
	else
		printf("%.*s: %.*s\n", 17, cabecalho.descreveModelo, dados.tamanhoModelo, dados.modelo);
	
	if (!dados.tamanhoCategoria)
		printf("%.*s: %s\n", 20, cabecalho.descreveCategoria, "campo com valor nulo");
	else
		printf("%.*s: %.*s\n", 20, cabecalho.descreveCategoria, dados.tamanhoCategoria, dados.categoria);
	
	if(!strlen(dados.data))
		printf("%.*s: %s\n", 35, cabecalho.descreveData, "campo com valor nulo");
	else
	{
		char *dataFormatada = calloc(30, sizeof(char));
		formataData(dados.data, dataFormatada); //Data formatada para o formato corrido
		printf("%.*s: %s\n", 35, cabecalho.descreveData, dataFormatada);
	}
		
	
	if(dados.quantidadeLugares == -1)
		printf("%.*s: %s\n", 42, cabecalho.descreveLugares, "campo com valor nulo");
	else
		printf("%.*s: %d\n", 42, cabecalho.descreveLugares, dados.quantidadeLugares);			
	printf("\n");
	
	return 1;
}

//Função que para ler todos os dados do arquivo e exibir
void selectVeiculo(){
	char nomeBin[30];
	scanf(" %s", nomeBin);
	FILE *arquivoBin = abrirBinarioLeitura(nomeBin);

	RegistroCabVeiculo cabecalho;
	RegistroDadosVeiculo dados;
	int final = 0, achados = 0;

	lerCabecalhoVeiculo(arquivoBin, &cabecalho);
	
	//For infino que só para após lê e exibir tudo
	for(;;){
		final = lerDadosVeiculo(arquivoBin, &dados);
		if(!final) //Se chegar no final = 0, finaliza o for
			break;
		//Toda vez que achar adiciona aos achados, se no final ele for zero, retorna nenhum registro
		achados += imprimirSelectVeiculo(dados, cabecalho);
	}

	if (!achados)
		printf("Registro inexistente.\n");
	
}

//Função auxiliar para verificar qual campo foi requisitado para comparação
int igualdadeCamposVeiculo(RegistroDadosVeiculo dados, char nomeDoCampo[50], char valor[50]){

	if(!strcmp(nomeDoCampo, "prefixo")){
		if(!strncmp(dados.prefixo, valor, 5))
			return 1;
	}else if(!strcmp(nomeDoCampo, "data")){
		if(!strncmp(dados.data, valor, 10))
			return 1;
	}else if(!strcmp(nomeDoCampo, "quantidadeLugares")){
		int aux = atoi(valor);
		if(dados.quantidadeLugares == aux)
			return 1;
	}else if(!strcmp(nomeDoCampo, "codLinha")){
		int aux = atoi(valor);
		if(dados.codLinha == aux)
			return 1;
	}else if(!strcmp(nomeDoCampo, "modelo")){
		if(!strncmp(dados.modelo, valor, dados.tamanhoModelo))
			return 1;
	}else if(!strcmp(nomeDoCampo, "categoria")){
		if(!strncmp(dados.categoria, valor, dados.tamanhoCategoria))
			return 1;
	}
	return 0;
}

//Função que lê os dados conforme instrução requisitada 
void whereVeiculo(){
	char nomeBin[30];
	scanf(" %s", nomeBin);
	FILE *arquivoBin = abrirBinarioLeitura(nomeBin);

	RegistroCabVeiculo cabecalho;
	RegistroDadosVeiculo dados;
	
	char nomeDoCampo[50];
	char valor[50];
	int final = 0, achados = 0;

	scanf(" %s", nomeDoCampo);
	scan_quote_string(valor);

	lerCabecalhoVeiculo(arquivoBin, &cabecalho);
	
	for(;;)
	{
		final = lerDadosVeiculo(arquivoBin, &dados);

		if(!final)
			break;
		
		//Comparamos primeiro para ver se a instrução coincide
		if(igualdadeCamposVeiculo(dados, nomeDoCampo, valor))
			//Toda vez que achar adiciona aos achados, se no final ele for zero, retorna nenhum registro
			achados += imprimirSelectVeiculo(dados, cabecalho);
	}

	if (!achados)
		printf("Registro inexistente.\n");
	
}

//Função que adiciona os dados inseridos pelo usuário
RegistroDadosVeiculo entrarDadosInputVeiculo(){
	RegistroDadosVeiculo dados;

	//Alocamos primeiro para podermos ler e copiar, sera dado free posteriormente
	dados.modelo = calloc(50, sizeof(char));
	dados.categoria = calloc(50, sizeof(char));
	char *auxiliarQntLug = calloc(10, sizeof(char));
	char *auxiliarCodLinha = calloc(10, sizeof(char));
	
	scan_quote_string(dados.prefixo);
	scan_quote_string(dados.data);
	scan_quote_string(auxiliarQntLug);
	scan_quote_string(auxiliarCodLinha);
	scan_quote_string(dados.modelo);
	scan_quote_string(dados.categoria);

	if(!strcmp(dados.data, "")){
		strcpy(dados.data, "@@@@@@@@@@");
		dados.data[0] = '\0';
	}
	
	if(strcmp(auxiliarQntLug, ""))
		dados.quantidadeLugares = atoi(auxiliarQntLug);
	else
		dados.quantidadeLugares = -1;
	
	if(strcmp(auxiliarCodLinha, ""))
		dados.codLinha = atoi(auxiliarCodLinha);
	else
		dados.codLinha = -1;

	if(!strcmp(dados.modelo, ""))
	{
		dados.tamanhoModelo = 0;
		free(dados.modelo);
	}else{
		dados.tamanhoModelo = strlen(dados.modelo);
	}

	if(!strcmp(dados.categoria, ""))
	{
		dados.tamanhoCategoria = 0;
		free(dados.categoria);
	}else{
		dados.tamanhoCategoria = strlen(dados.categoria);
	}

	dados.tamanhoRegistro = somarTamanhoRegVeiculo(dados);
	dados.removido = '1';
	
	//não disse?
	free(auxiliarQntLug);
	free(auxiliarCodLinha);

	return dados; //retorna a struct para ser adicionada
}

//Função que insere no arquivo dados manuais
void insertIntoVeiculo(){
	RegistroCabVeiculo cabecalho;
	char nomeBin[30];
	int n; //n é o número de inserções a serem realizadas
	
	scanf(" %s", nomeBin);

	FILE *arquivoBin = abrirBinarioLeitura(nomeBin);

	scanf("%d", &n);

	lerCabecalhoVeiculo(arquivoBin, &cabecalho);
	fseek(arquivoBin, 0, SEEK_END);

	cabecalho.status = '0';

	for(int i = 0; i < n; i++){
		//Escreveremos os dados capturados pelo entrarDadosInputVeiculo
		escreverDadosVeiculo(arquivoBin, entrarDadosInputVeiculo());
		cabecalho.nroRegistros++;
	}

	cabecalho.byteProxReg = ftell(arquivoBin);
	cabecalho.status = '1'; //Se tudo der certo, alteramos o status

	fseek(arquivoBin, 0, SEEK_SET);
	escreverCabecalhoVeiculo(arquivoBin, cabecalho);

	fclose(arquivoBin);

	binarioNaTela(nomeBin);
}

//Função que busca registro na arvore b
void buscarRegistroVeiculo() {
	RegCabBtree cabBtree = inicializarCabecalho(); //Inicializar o cabeçalho da arvore
	RegistroCabVeiculo cab = inicializarCabVeiculo(); //Inicializar o cabeçalho do veiculo
	RegistroDadosVeiculo dados = inicializarDadosVeiculo();
	int chave = 0, foundRegRRN = 0;
	char prefixo[20]; //prefixo do veiculo
	char nomeArqVeiculo[30], nomeArqArvoreB[30], buffer[30]; //nome do arquivo bin e indices da arvore, buffer só descartamos por enquanto
	
	scanf(" %s %s %s", nomeArqVeiculo, nomeArqArvoreB, buffer);
	scan_quote_string(prefixo);
	
	chave = convertePrefixo(prefixo);

	FILE *veiculoBin = abrirBinarioLeitura(nomeArqVeiculo);
	FILE *arqBtreeIndex = abrirBinarioLeitura(nomeArqArvoreB);
	
	lerCabecalhoArvoreB(arqBtreeIndex, &cabBtree);
	lerCabecalhoVeiculo(veiculoBin, &cab);

	//Vamos chamar a função de buscar recursivamente e testaremos se o registro foi encontrado
	if(!buscarArvoreB(arqBtreeIndex, cabBtree.noRaiz, chave, &foundRegRRN)) 
	{//Se não retornar nada, então não temos registros
		printf("Registro inexistente.");
		return; //fecha a função
	}

	fseek(veiculoBin, foundRegRRN, SEEK_SET); //setamos para o registro achado

	//ler e imprimir dados resultantes
	lerDadosVeiculo(veiculoBin, &dados);
	imprimirSelectVeiculo(dados, cab);

	//fechando ambos arquivos :D
	fclose(veiculoBin); 
	fclose(arqBtreeIndex);
}

//Função para criar arquivo de indice
void driverVeiculoVazio(){
	RegCabBtree cabBtree = inicializarCabecalho();
	RegistroCabVeiculo cab = inicializarCabVeiculo(); 
	char nomeArqVeiculo[30], nomeArqArvoreB[30];
	
	scanf(" %s %s", nomeArqVeiculo, nomeArqArvoreB);
	
	FILE *veiculoBin = abrirBinarioLeitura(nomeArqVeiculo);
	FILE *arqBtreeIndex = abrirBinarioEscrita(nomeArqArvoreB);
	
	lerCabecalhoVeiculo(veiculoBin, &cab);

	int n = 0, raiz = 0, chave = 0;
	long long int valor = 0;

	RegistroDadosVeiculo dados = inicializarDadosVeiculo();

	for(int i = 0; i < (cab.nroRegRemovidos+cab.nroRegistros); i++){ //for com a quantidade de registros total
		valor = ftell(veiculoBin);
		lerDadosVeiculo(veiculoBin, &dados);

		if(dados.removido == '0') //Se o dado não for removido, continuaremos
			continue;

		chave = convertePrefixo(dados.prefixo);
		raiz = cabBtree.noRaiz;

		//driver pra iniciar inserção
		inserirDriver(&cabBtree, arqBtreeIndex, &raiz, valor, chave);
	}

	cabBtree.status = '1';
	cabBtree.noRaiz = raiz;
	
	cab.status = '1'; //Se tudo der certo, alteramos o status
	cab.byteProxReg = ftell(veiculoBin);
	
	fseek(arqBtreeIndex, 0, SEEK_SET);
	fseek(veiculoBin, 0, SEEK_SET);

	escreverCabecalhoArvoreB(arqBtreeIndex, cabBtree);
	escreverCabecalhoVeiculo(veiculoBin, cab);

	fclose(veiculoBin);
	fclose(arqBtreeIndex);
	
	binarioNaTela(nomeArqArvoreB); //exibindo resultado do binario
}

//Função que insere ao final do arquivo indice com entrada via input
void driverVeiculoCheio(){
	RegCabBtree cabBtree = inicializarCabecalho();
	RegistroCabVeiculo cab = inicializarCabVeiculo(); 
	char nomeArqVeiculo[30], nomeArqArvoreB[30];
	
	scanf(" %s %s", nomeArqVeiculo, nomeArqArvoreB);

	FILE *veiculoBin = abrirBinarioLeitura(nomeArqVeiculo);
	FILE *arqBtreeIndex = abrirBinarioLeitura(nomeArqArvoreB);
	
	lerCabecalhoVeiculo(veiculoBin, &cab);

	int n = 0, raiz = 0, chave = 0;
	long long int valor = 0;

	RegistroDadosVeiculo dados = inicializarDadosVeiculo();
	
	fseek(veiculoBin, 0, SEEK_END);
	
	lerCabecalhoArvoreB(arqBtreeIndex, &cabBtree);
	
	scanf("%d", &n);

	for(int i = 0; i < n; i++){//Escreveremos os dados capturados pelo entrarDadosInputVeiculo
		
		dados = entrarDadosInputVeiculo();
		
		valor = ftell(veiculoBin);
		chave = convertePrefixo(dados.prefixo);
		raiz = cabBtree.noRaiz;

		//driver pra iniciar inserção
		inserirDriver(&cabBtree, arqBtreeIndex, &raiz, valor, chave);

		escreverDadosVeiculo(veiculoBin, dados);
		cab.nroRegistros++;
	}

	cabBtree.status = '1';
	cabBtree.noRaiz = raiz;
	
	cab.status = '1'; //Se tudo der certo, alteramos o status
	cab.byteProxReg = ftell(veiculoBin);

	fseek(arqBtreeIndex, 0, SEEK_SET);
	fseek(veiculoBin, 0, SEEK_SET);

	escreverCabecalhoArvoreB(arqBtreeIndex, cabBtree);
	escreverCabecalhoVeiculo(veiculoBin, cab);

	fclose(veiculoBin);
	fclose(arqBtreeIndex);
	
	binarioNaTela(nomeArqArvoreB); //exibindo resultado do binario
}