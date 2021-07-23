/*
    Johnny Silva | 11371350
    Erick Barcelos | 11345562
*/
#include "dados.h"

//Inicializar o cabeçalho
RegCabBtree inicializarCabecalho(){
	RegCabBtree ab;
	ab.status = '0';
	ab.RRNproxNo = 0;
	ab.noRaiz = -1;
	return ab;
}

//Inicializar a página
DadosBtree inicializarPagina(){
	DadosBtree ab;
	ab.folha = '0';
	ab.nroChavesIndexadas = 0;
	ab.RRNdoNo = 0;
	
	for(int i = 0; i < MAX; i++){
		if(i < MAX-1){
			ab.chave[i] = -1;
			ab.ponteiroReg[i] = -1;
		}
		ab.ponteiro[i] = -1;
	}

	return ab;
}

//Inicializar a página de trabalho (com tamanhos maiores)
Wpage inicializarWorkPagina(){
	Wpage ab;
	ab.folha = '0';
	ab.nroChavesIndexadas = 0;
	ab.RRNdoNo = 0;
	
	for(int i = 0; i <= MAX; i++){
		if(i < MAX){
			ab.chave[i] = -1;
			ab.ponteiroReg[i] = -1;
		}
		ab.ponteiro[i] = -1;
	}

	return ab;
}

//Função para ler o cabeçalho de um arquivo
void lerCabecalhoArvoreB(FILE *arquivo, RegCabBtree *cabecalho){
	fread(&cabecalho->status, sizeof(char), 1, arquivo);
	fread(&cabecalho->noRaiz, sizeof(int), 1, arquivo);
	fread(&cabecalho->RRNproxNo, sizeof(int), 1, arquivo);
	
	if(cabecalho->status == '0')
	{ //Se o arquivo estiver corrompido, informa e para a execução
		printf("Falha no processamento do arquivo.");
		exit(0);
	}
}

//Ler dados do arquivo
void lerPaginaArvoreB(FILE *arquivo, DadosBtree *dados){
	fread(&dados->folha, sizeof(char), 1, arquivo);
	fread(&dados->nroChavesIndexadas, sizeof(int), 1, arquivo);	
	fread(&dados->RRNdoNo, sizeof(int), 1, arquivo);	
	
    //ler e inserir no dados
	for(int i = 0; i < MAX; i++){
		fread(&dados->ponteiro[i], sizeof(int), 1, arquivo);
		if(i < MAX-1){
			fread(&dados->chave[i], sizeof(int), 1, arquivo);	
			fread(&dados->ponteiroReg[i], sizeof(long long int), 1, arquivo);
		}
	}
}

//Função que escreve o cabeçalho no arquivo
void escreverCabecalhoArvoreB(FILE *arquivo, RegCabBtree cabecalho){
	fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
	fwrite(&cabecalho.noRaiz, sizeof(int), 1, arquivo);
	fwrite(&cabecalho.RRNproxNo, sizeof(int), 1, arquivo);
	
	char lixo = '@';
    //ainda não entendi porque tanto lixo, mas ai
	for(int i = 0; i < 68; i++)
		fwrite(&lixo, sizeof(char), 1, arquivo);
}

//Função que escreve os dados no arquivo
void escreverPaginaArvoreB(FILE *arquivo, DadosBtree dados){

	dados.nroChavesIndexadas = numeroDeChaves(dados);

	fwrite(&dados.folha, sizeof(char), 1, arquivo);	
	fwrite(&dados.nroChavesIndexadas, sizeof(int), 1, arquivo);	
	fwrite(&dados.RRNdoNo, sizeof(int), 1, arquivo);	
	
    //Aqui escrevemos ao invés de resgatar 
	for(int i = 0; i < MAX; i++){
		fwrite(&dados.ponteiro[i], sizeof(int), 1, arquivo);
		if(i < MAX-1){
			fwrite(&dados.chave[i], sizeof(int), 1, arquivo);	
			fwrite(&dados.ponteiroReg[i], sizeof(long long int), 1, arquivo);	
		}
	}
}

//Função para verificar se a chave existe ou onde deveria estar
int ocorrenciaDaChave(DadosBtree pagina, int chave, int *pos){
	for(int i = 0; i < MAX-1; i++){
		*pos = i;
		if(pagina.chave[i] == -1)
			return 0;
		if(pagina.chave[i] == chave)
			return 1;
		if(chave > pagina.chave[i])
			*pos = i+1;
		else if(chave < pagina.chave[i])
			return 0;
	}
	return 0;
}

//Função para verificar se a chave existe ou onde deveria estar da workpage
int ocorrenciaDaChaveWorkPage(Wpage pagina, int chave, int *pos){
	for(int i = 0; i < MAX; i++){
		*pos = i;
		if(pagina.chave[i] == -1)
			return 0;
		if(pagina.chave[i] == chave)
			return 1;
		if(chave > pagina.chave[i])
			*pos = i+1;
		else if(chave < pagina.chave[i])
			return 0;
	}
	return 0;
}

//Função para contar quantidade de chaves
int numeroDeChaves(DadosBtree pagina){
	int num = 0;

	for(int i = 0; i < MAX-1; i++){
		if(pagina.chave[i] == -1)
			return num;
		num++;
	}

	return num;
}

//Função para andar no arquivo pela quantidade de bytes na pagina
int andarNoArquivo(int num){
	return (num+1)*77;
}

//Função para retornar se há espaço na página
int espacoPagina(DadosBtree pagina){
	return (pagina.nroChavesIndexadas >= 0 && pagina.nroChavesIndexadas < MAX-1);
}

//Função para mover uma posição a direito na pagina
void shiftVetorPagina(DadosBtree *pagina, int inicio, int final){
	for (int i = inicio; i > final; i--){
		pagina->chave[i] = pagina->chave[i-1];
		pagina->ponteiroReg[i] = pagina->ponteiroReg[i-1];
		pagina->ponteiro[i+1] = pagina->ponteiro[i];
	}
}

//Função para mover uma posição a direito na pagina de trabalho
void shiftVetorWorkPagina(Wpage *pagina, int inicio, int final){
	for (int i = inicio-1; i > final; i--){
		pagina->chave[i] = pagina->chave[i-1];
		pagina->ponteiroReg[i] = pagina->ponteiroReg[i-1];
		pagina->ponteiro[i+1] = pagina->ponteiro[i];
	}
}

//Função recursiva que busca registro na arvore b
int buscarArvoreB(FILE *arquivo, int RRN, int chave, long long int *foundRegRRN){	
	int pos = 0;
	
	if(RRN == -1)
		return 0; // nao achou
	
	DadosBtree pagina = inicializarPagina();

	fseek(arquivo, andarNoArquivo(RRN), SEEK_SET);
	lerPaginaArvoreB(arquivo, &pagina);

	if(ocorrenciaDaChave(pagina, chave, &pos)){
		*foundRegRRN = pagina.ponteiroReg[pos];
		return 1;
	}else 
		return buscarArvoreB(arquivo, pagina.ponteiro[pos], chave, foundRegRRN);
	
	return 0;
}

//Função driver que insere na arvore b e realiza split se houver promoap
void inserirDriver(RegCabBtree *cabBtree, FILE *arqBtreeIndex, int *raiz, int valor, int chave){
	int chavePromo = 0, filhoPromo = 0;
	long long int regRRNPromo = 0;
	
	if(inserirArvoreB(cabBtree, arqBtreeIndex, *raiz, valor, chave, &chavePromo, &filhoPromo, &regRRNPromo)){ //Se houver promoção
		DadosBtree novaPagina = inicializarPagina();
		novaPagina.chave[0] = chavePromo;
		novaPagina.ponteiroReg[0] = regRRNPromo;
		novaPagina.ponteiro[0] = *raiz;
		novaPagina.ponteiro[1] = filhoPromo;
		if (*raiz == -1) //se raiz vazia então é folha
			novaPagina.folha = '1';
		
		novaPagina.RRNdoNo = cabBtree->RRNproxNo;
		*raiz = cabBtree->RRNproxNo++;
		cabBtree->noRaiz = *raiz;
		
		fseek(arqBtreeIndex, andarNoArquivo(novaPagina.RRNdoNo), SEEK_SET);
		escreverPaginaArvoreB(arqBtreeIndex, novaPagina);
	}
}

//Função recursivo que de fato insere na arvore b
int inserirArvoreB(RegCabBtree *cabBtree, FILE *arquivo, int RRN, int regRRN, int chave, int *chavePromovida, int *filhoChavePromovida, long long int *regRRNPromo){
	
	if (RRN == -1){ //Verificar se o nó é folha
		*chavePromovida = chave;
		*regRRNPromo = regRRN;
		*filhoChavePromovida = -1;
		return 1;
	}

	DadosBtree pagina = inicializarPagina(); // página de disco correntemente examinada pela função
	DadosBtree novaPagina = inicializarPagina(); //página de disco nova resultante do particionamento
	int pos = 0; //posição na página na qual a chave ocorre ou deveria ocorrer
	int pbChave = 0; //chave promovida do nível inferior para ser inserida em PAGE
	int pbRRN = 0; //RRN promovido do nível inferior para ser inserido em PAGE
	long long int pbRegRRN = 0; //RRN promovido do nível inferior para ser inserido em PAGE

	fseek(arquivo, andarNoArquivo(RRN), SEEK_SET);
	lerPaginaArvoreB(arquivo, &pagina);

	if(ocorrenciaDaChave(pagina, chave, &pos))
		return 0;

	int retValor = inserirArvoreB(cabBtree, arquivo, pagina.ponteiro[pos], regRRN, chave, &pbChave, &pbRRN, &pbRegRRN);

	if (!retValor) //sem promoção ou sem erro
		return retValor;
	else if (espacoPagina(pagina)){

		fseek(arquivo, andarNoArquivo(RRN), SEEK_SET);
		
		shiftVetorPagina(&pagina, pagina.nroChavesIndexadas, pos);

		pagina.chave[pos] = pbChave;
		pagina.ponteiroReg[pos] = pbRegRRN;
		pagina.ponteiro[pos+1] = pbRRN;
		
		escreverPaginaArvoreB(arquivo, pagina);
		return 0; //SEM PROMOÇÃO
	} else if (pagina.nroChavesIndexadas == 4) {
        split(cabBtree, arquivo, pbChave, pbRegRRN, pbRRN, &pagina, chavePromovida, regRRNPromo, filhoChavePromovida, &novaPagina);

		fseek(arquivo, andarNoArquivo(RRN), SEEK_SET);
		escreverPaginaArvoreB(arquivo, pagina);
		
		fseek(arquivo, andarNoArquivo(*filhoChavePromovida), SEEK_SET);
		escreverPaginaArvoreB(arquivo, novaPagina);
		
		cabBtree->RRNproxNo++;
		return 1; //PROMOÇÃO
	}
}

//Função de tratamento do overflow causado pela inserção de uma chave
void split(RegCabBtree *cabBtree, FILE *arquivo, int novaChave, long long int regRRN, int filhoDireitaRRN, DadosBtree *pagina, int *chavePromovida, long long int *regRRNPromo, int *filhoChavePromovida, DadosBtree *novaPagina) {
    //Vamos criar uma pagina com espaços a mais para posteriormente dividir e conquistar
	Wpage workPagina = inicializarWorkPagina(); 
	int middle = MAX/2;
	int pos = 0;

	for(int i = 0; i < MAX; i++){	
		if(i < MAX-1){
			workPagina.chave[i] = pagina->chave[i];
			workPagina.ponteiroReg[i] = pagina->ponteiroReg[i];
		}
		workPagina.ponteiro[i] = pagina->ponteiro[i];
	}

	if(ocorrenciaDaChaveWorkPage(workPagina, novaChave, &pos))
		return;
	
    //Moveremos os dados para a direita onde a chave deverá entrar
	shiftVetorWorkPagina(&workPagina, MAX, pos);
		
	workPagina.chave[pos] = novaChave;
	workPagina.ponteiroReg[pos] = regRRN;
	workPagina.ponteiro[pos+1] = filhoDireitaRRN;

	fseek(arquivo, 0, SEEK_END);

	if(pagina->folha == '1')
		novaPagina->folha = '1';
	else
		novaPagina->folha = '0';

	int teste = pagina->RRNdoNo;

	novaPagina->RRNdoNo = cabBtree->RRNproxNo;
	novaPagina->nroChavesIndexadas = 2;

	*chavePromovida = workPagina.chave[middle];
	*regRRNPromo = workPagina.ponteiroReg[middle];
	*filhoChavePromovida = novaPagina->RRNdoNo;

    //Nova página resultante
	*pagina = inicializarPagina();
	pagina->nroChavesIndexadas = 2;
	pagina->folha = novaPagina->folha;
	pagina->RRNdoNo = teste;

	int j = 0;

	for(int i = 0; i <= middle; i++){
		if(i < middle){
			pagina->chave[j] = workPagina.chave[i];
			pagina->ponteiroReg[j] = workPagina.ponteiroReg[i];
		}
		pagina->ponteiro[j] = workPagina.ponteiro[i];
		j++;
	}

	j = 0;

	for(int i = middle+1; i <= MAX; i++){
		if(i < MAX){
			novaPagina->chave[j] = workPagina.chave[i];
			novaPagina->ponteiroReg[j] = workPagina.ponteiroReg[i];
		}
		novaPagina->ponteiro[j] = workPagina.ponteiro[i];
		j++;
	}
}