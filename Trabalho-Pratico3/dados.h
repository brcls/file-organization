/*
    Johnny Silva | 11371350
    Erick Barcelos | 11345562
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 5
#define LIMIT 1000000

// Registros da árvore B
// 77 bytes
typedef struct regCabArvoreB{
    char status;
    int noRaiz; //ponteiro
    int RRNproxNo; //?
} RegCabBtree;

//77 bytes
typedef struct regDadosArvoreB{
    char folha; //indica se o nó é um nó folha ou não 
    int nroChavesIndexadas; //armazena o número de chaves indexadas no nó
    int RRNdoNo; //armazena o número do RRN referente ao nó
    int ponteiro[MAX]; //ponteiro
    int chave[MAX-1]; //chave (prefixo ou cod)
    long long int ponteiroReg[MAX-1]; //RRN no arquivo binario
} DadosBtree;

typedef struct workingPage{
    char folha; //indica se o nó é um nó folha ou não 
    int nroChavesIndexadas; //armazena o número de chaves indexadas no nó
    int RRNdoNo; //armazena o número do RRN referente ao nó
    int ponteiro[MAX+1]; //ponteiro
    int chave[MAX]; //chave (prefixo ou cod)
    long long int ponteiroReg[MAX]; //RRN no arquivo binario
} Wpage;

//Registros da linhas
//82 bytes
typedef struct regCabLinha
{
    char status;
    long long int byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descreveCodigo[15];
    char descreveCartao[13];
    char descreveNome[13];
    char descreveCor[24];
}RegistroCabLinha;

//Tamanho váriavel
typedef struct regDadosLinha
{
    char removido;
    int tamanhoRegistro;
    int codLinha; //não aceita valores nulos
    char aceitaCartao;
    int tamanhoNome;
    char *nomeLinha;
    int tamanhoCor;
    char *corLinha;   
}RegistroDadosLinha;

// Funções principais do veículo

void createTableLinha();
void selectLinha();
void whereLinha();
void insertIntoLinha();

// Funções auxiliares do linha

int somarTamanhoDoRegLinha(RegistroDadosLinha dados);
RegistroCabLinha pegarCabCSVLinha(char line[300]);
RegistroDadosLinha pegarDadosCSVLinha(char line[100], FILE *arquivo, RegistroCabLinha *cabecalho);
void escreverCabecalhoLinha(FILE *arquivo, RegistroCabLinha cabecalho);
void escreverDadosLinha(FILE *arquivo, RegistroDadosLinha dados);
void lerCabecalhoLinha(FILE *arquivo, RegistroCabLinha *cabecalho);
int lerDadosLinha(FILE *arquivo, RegistroDadosLinha *dados);
int imprimirSelectLinha(RegistroDadosLinha dados, RegistroCabLinha cabecalho);
int igualdadeCamposLinha(RegistroDadosLinha dados, char nomeDoCampo[50], char valor[50]);
RegistroDadosLinha entrarDadosInputLinha();
RegistroCabLinha inicializarCabLinha();
RegistroDadosLinha inicializarDadosLinha();

//Registros do veículo
//175 bytes
typedef struct regCabVeiculo
{
    char status;
    long long int byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descrevePrefixo[18];
    char descreveData[35];
    char descreveLugares[42];
    char descreveLinha[26];
    char descreveModelo[17];
    char descreveCategoria[20];

}RegistroCabVeiculo;

//Tamanho váriavel
typedef struct regDadosVeiculo
{
    char removido;
    int tamanhoRegistro;
    char prefixo[5];
    char data[10];
    int quantidadeLugares;
    int codLinha;
    int tamanhoModelo;
    char *modelo;
    int tamanhoCategoria;
    char *categoria;

}RegistroDadosVeiculo;

// Funções principais do veículo

void createTableVeiculo();
void selectVeiculo();
void whereVeiculo();
void insertIntoVeiculo();

// Funções auxiliares do veículo

int somarTamanhoRegVeiculo(RegistroDadosVeiculo dados);
RegistroCabVeiculo pegarCabCSVVeiculo(char line[300]);
RegistroDadosVeiculo pegarDadosCSVVeiculo(char line[300], FILE *arquivo, RegistroCabVeiculo *cabecalho);
void escreverCabecalhoVeiculo(FILE *arquivo, RegistroCabVeiculo cabecalho);
void escreverDadosVeiculo(FILE *arquivo, RegistroDadosVeiculo dados);
void lerCabecalhoVeiculo(FILE *arquivo, RegistroCabVeiculo *cabecalho);
int lerDadosVeiculo(FILE *arquivo, RegistroDadosVeiculo *dados);
int imprimirSelectVeiculo(RegistroDadosVeiculo dados, RegistroCabVeiculo cabecalho);
int igualdadeCamposVeiculo(RegistroDadosVeiculo dados, char nomeDoCampo[50], char valor[50]);
RegistroDadosVeiculo entrarDadosInputVeiculo();
RegistroCabVeiculo inicializarCabVeiculo();
RegistroDadosVeiculo inicializarDadosVeiculo();

//Funções do auxiliares.c

void SeletorFuncao(int x);
int verificarRemovidoVeiculo(char *token, char *troca, RegistroCabVeiculo *cabecalho);
int verificarRemovidoLinha(char *token, char *troca, RegistroCabLinha *cabecalho);
FILE* abrirCSVLeitura(char nomeArquivo[20]);
FILE* abrirBinarioLeitura(char nomeArquivo[20]);
FILE* abrirBinarioEscrita(char nomeArquivo[20]);
void binarioNaTela(char *nomeArquivoBinario);
void formataData(char data[11], char novaData[30]);
void scan_quote_string(char *str);
int convertePrefixo(char* str);

// Funções da árvore B

void lerCabecalhoArvoreB(FILE *arquivo, RegCabBtree *cabecalho);
void lerPaginaArvoreB(FILE *arquivo, DadosBtree *dados);
void escreverCabecalhoArvoreB(FILE *arquivo, RegCabBtree cabecalho);
void escreverPaginaArvoreB(FILE *arquivo, DadosBtree dados);
int ocorrenciaDaChave(DadosBtree pagina, int chave, int *pos);
int ocorrenciaDaChaveWorkPage(Wpage pagina, int chave, int *pos);
int numeroDeChaves(DadosBtree pagina);
int andarNoArquivo(int num);
int espacoPagina(DadosBtree pagina);
RegCabBtree inicializarCabecalho();
DadosBtree inicializarPagina();
Wpage inicializarWorkPagina();
void shiftVetorPagina(DadosBtree *pagina, int inicio, int final);
void shiftVetorWorkPagina(Wpage *pagina, int inicio, int final);
int buscarArvoreB(FILE *arquivo, int RRN, int chave, long long int *foundRegRRN);
void inserirDriver(RegCabBtree *cabBtree, FILE *arqBtreeIndex, int *raiz, int valor, int chave);
int inserirArvoreB(RegCabBtree *cabBtree, FILE *arquivo, int RRN, int regRRN, int chave, int *chavePromovida, int *filhoChavePromovida, long long int *regRRNPromo);
void split(RegCabBtree *cabBtree, FILE *arquivo, int novaChave, long long int regRRN, int filhoDireitaRRN, DadosBtree *pagina, int *chavePromovida, long long int *regRRNPromo, int *filhoChavePromovida, DadosBtree *novaPagina);

void buscarRegistroVeiculo();
void driverVeiculoVazio();
void driverVeiculoCheio();

void buscarRegistroLinha();
void driverLinhaVazio();
void driverLinhaCheio();

// Funções de junção e ordenação

void printarVeiculoLinha(RegistroCabLinha cabLinha, RegistroDadosLinha linha, RegistroCabVeiculo cabVeiculo, RegistroDadosVeiculo veiculo);
void juncaoDeLoopAninhado();
void juncaoDeLoopUnico();
int compararCodlinhaV(const void *x, const void *y);
int compararCodlinhaL(const void *x, const void *y); 
void quickSortVeiculo(int numReg, FILE *desordenado, RegistroDadosVeiculo vetor[LIMIT]);
void escreverVetVeiculo(int *numRegInseridos, int numReg, FILE *ordenado, RegistroDadosVeiculo vetor[LIMIT]);
void quickSortLinha(int numReg, FILE *desordenado, RegistroDadosLinha vetor[LIMIT]);
void escreverVetLinha(int *numRegInseridos, int numReg, FILE *ordenado, RegistroDadosLinha vetor[LIMIT]);
void ordenarVeiculo();
void ordenarLinha();
void juncaoOrdenacaoIntercalacao();