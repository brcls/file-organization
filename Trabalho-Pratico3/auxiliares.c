/*
    Johnny Silva | 11371350
    Erick Barcelos | 11345562
*/

#include "dados.h"

//Seleciona qual operação o programa deve realizar
void SeletorFuncao(int x)
{
    switch (x)
    {
    case 1:
        createTableVeiculo();
        break;
    case 2:
        createTableLinha();
        break;
    case 3:
        selectVeiculo();
        break;
    case 4:
        selectLinha();
        break;
    case 5:
        whereVeiculo();
        break;
    case 6:
        whereLinha();
        break;
    case 7:
        insertIntoVeiculo();   
        break;
    case 8:
        insertIntoLinha();
        break;
    case 9:
        driverVeiculoVazio();
        break;
    case 10:
        driverLinhaVazio();
        break;
    case 11:
        buscarRegistroVeiculo();
        break;
    case 12:
        buscarRegistroLinha();
        break;
    case 13:
        driverVeiculoCheio();
        break;
    case 14:
        driverLinhaCheio();
        break; 
    case 15:
        juncaoDeLoopAninhado();
        break; 
    case 16:
        juncaoDeLoopUnico();
        break; 
    case 17:
        ordenarVeiculo();
        break; 
    case 18:
        ordenarLinha();
        break; 
    case 19:
        juncaoOrdenacaoIntercalacao();
        break; 
    }
}

//Verifica se o dado do veiculo está removido, caso não adiciona aos normais ao invés dos removidos
int verificarRemovidoVeiculo(char *token, char *troca, RegistroCabVeiculo *cabecalho){
	if(token[0] != '*'){
        *troca = '1';
        cabecalho->nroRegistros++;
    }else{
        *troca = '0';
        memmove(token, token+1, strlen(token));
        cabecalho->nroRegRemovidos++;
    }
}

//Verifica se o dado da linha está removido, caso não adiciona aos normais ao invés dos removidos
int verificarRemovidoLinha(char *token, char *troca, RegistroCabLinha *cabecalho){
	if(token[0] != '*'){
        *troca = '1';
        cabecalho->nroRegistros++;
    }else{
        *troca = '0';
        memmove(token, token+1, strlen(token));
        cabecalho->nroRegRemovidos++;
    }
}

//Abrir o arquivo na função apenas para ler
FILE* abrirCSVLeitura(char nomeArquivo[20])
{
    FILE *fb;

    if ((fb = fopen(nomeArquivo, "r")) == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        // Sair do programa caso retornar NULL
        exit(0);
    }

    return fb;
}

//Abrir o arquivo em binario na função focada em ler
FILE* abrirBinarioLeitura(char nomeArquivo[20]){
    FILE *fb;

    if ((fb = fopen(nomeArquivo, "rb+")) == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        // Sair do programa caso retornar NULL
        exit(0);
    }

    return fb;
}

//Abrir o arquivo em binario na função focada em escrever
FILE* abrirBinarioEscrita(char nomeArquivo[20]){
    FILE *fb;

    if ((fb = fopen(nomeArquivo, "wb+")) == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        // Sair do programa caso retornar NULL
        exit(0);
    }

    return fb;
}

//Função dada
void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs; 
	unsigned char *mb;
	size_t fl;
	FILE *fs;
    
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

//Formata data para o formato corrido
void formataData(char data[11], char novaData[30])
{
    char *dia = calloc(3, sizeof(char));
    char *mes = calloc(3, sizeof(char));
    char *ano = calloc(5, sizeof(char));
    int mesInt = 0;
    char *nomeMes = calloc(20, sizeof(char));
    
    strcpy(novaData, data);
    char *token = strtok(novaData, "-");
    strcpy(ano, token);
    token = strtok(NULL, "-");
    strcpy(mes, token);
    token = strtok(NULL, "-");
    strcpy(dia, token);

    mesInt = atoi(mes);

    switch (mesInt)
    {
    case 1:
        strcpy(nomeMes, "janeiro");
        break;
    case 2:
        strcpy(nomeMes, "fevereiro");
        break;
    case 3:
        strcpy(nomeMes, "março");
        break;
    case 4:
        strcpy(nomeMes, "abril");
        break;
    case 5:
        strcpy(nomeMes, "maio");
        break;
    case 6:
        strcpy(nomeMes, "junho");
        break;
    case 7:
        strcpy(nomeMes, "julho");
        break;
    case 8:
        strcpy(nomeMes, "agosto");
        break;
    case 9:
        strcpy(nomeMes, "setembro");
        break;
    case 10:
        strcpy(nomeMes, "outubro");
        break;
    case 11:
        strcpy(nomeMes, "novembro");
        break;
    case 12:
        strcpy(nomeMes, "dezembro");
        break;
    }

    strcpy(novaData, dia);
    strcat(novaData, " de ");
    strcat(novaData, nomeMes);
    strcat(novaData, " de ");
    strcat(novaData, ano);

    free(dia);
    free(mes);
    free(ano);
    free(nomeMes);

}

//Função dada
void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

int convertePrefixo(char* str) {

    /* O registro que tem essa string como chave foi removido */
    if(str[0] == '*')
        return -1;

    /* Começamos com o primeiro digito na ordem de 36^0 = 1 */
    int power = 1;

    /* Faz a conversão char por char para chegar ao resultado */
    int result = 0;
    for(int i = 0; i < 5; i++) {

        /* 
            Interpreta o char atual como se fosse um digito
            em base 36. Os digitos da base 36 são:
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D,
            E, F, G, H, I, J, K, L, M, N, O, P, Q, R,
            S, T, U, V, W, X, Y, Z
        */
        int cur_digit;
        /* Checa pelos digitos normais e os converte para números */
        if(str[i] >= '0' && str[i] <= '9')
            cur_digit = str[i] - '0';
        /* Checa pelas letras e as converte para números */
        else if(str[i] >= 'A' && str[i] <= 'Z')
            cur_digit = 10 + str[i] - 'A';

        /*
            Multiplica o digito atual pelo ordem da posição atual
            e adiciona no resultado
            Primeira posição:   36^0 = 1
            Segunda posição:    36^1 = 36
            Terceira posição:   36^2 = 1.296
            Quarta posição:     36^3 = 46.656
            Quinta posição:     36^4 = 1.679.616
        */
        result += cur_digit * power;

        /* Aumenta a ordem atual */
        power *= 36;

    }

    return result;

}