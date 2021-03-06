/*
 * Trabalho 02 de Introdução à Computação II - UNISANTOS - 2018
 * Autores:
 * Alexandre Saura   | C.C.
 * Fábio Thomaz      | C.C.
 * Ricardo Oliveira  | C.C.
 * Professor: Ciro Cirne Trindade
*/

/*UPDATE - RICARDO OLIVERIA:
 *AINDA ESTOU ARRUMANDO ALGUNS DETALHES DE EXIBIÇÃO*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FPD "produtos.dat" //FILE_PRODUTOS
#define FLJ "lojas.dat"    //FILE_LOJAS
#define FPC "precos.dat"   //FILE_PRECOS

typedef struct {
	int cod;
	char descricao[51];
} tproduto;

typedef struct {
	int cod;
	char nome[41];
	char site[81];
} tloja;

typedef struct {
	int cod_produto;
	int cod_loja;
	float preco;
} tpreco;

void cadastrar_prod(void);								//FINALIZADO
int pegar_cod(const char *, int);						//FINALIZADO
void listar_prod(void);									//FINALIZADO
void ordenar(int, int, tproduto *, tloja *, tpreco *);  //FINALIZADO
void alterar_prod(void);								//FINALIZADO
void cadastrar_loja(void);								//FINALIZADO
void listar_loja(void);									//FINALIZADO
void cadastrar_preco(void);								//FINALIZADO
void listar_preco(void);								//FINALIZADO
void consultar_preco(void);								//FINALIZADO
void printar_linha(int, int);							//FINALIZADO

int main()
{
	/*Menu principal. Mantém um looping enquanto o usuário quiser continuar*/
	int op;
	do{
		printf("/ Digite o numero de uma das opcoes\\\n");
		printf("*----------------------------------*\n");
		printf("| <1> Cadastrar produtos;          |\n");
		printf("| <2> Alterar produto;             |\n");
		printf("| <3> Listar produtos cadastrados; |\n");
		printf("| <4> Cadastrar lojas;             |\n");
		printf("| <5> Listar lojas cadastradas;    |\n");
		printf("| <6> Cadastrar precos;            |\n");
		printf("| <7> Listar precos cadastros;     |\n");
		printf("| <8> Consultar preco de produto;  |\n");
		printf("| <0> Sair.                        |\n");
		printf("*----------------------------------*\n>>> ");
		scanf("%d", &op);

		switch(op){
			case 0:
				break;
			case 1:
				cadastrar_prod();
				break;
			case 2:
				alterar_prod();
				break;
			case 3:
				listar_prod();
				break;
			case 4:
				cadastrar_loja();
				break;
			case 5:
				listar_loja();
				break;
			case 6:
				cadastrar_preco();
				break;
			case 7:
				listar_preco();
				break;
			case 8:
				consultar_preco();
				break;
			default:
				fprintf(stderr, "\n\aOpcao invalida!\n");
		}
		printf("\n");
	}while(op != 0);
	printf("Fim de execucao\n");
	
	return 0;
}

void cadastrar_prod()
{
	/*Este procedimento cadastra novos produtos*/
	FILE * pin;
	int codigo = pegar_cod(FPD, 1);

	printf("************************************\n");
	printf("*        CADASTRO DE PRODUTOS      *\n");
	printf("************************************\n");
	printf("* Para sair, digite 'sair' na des- *\n");
	printf("* -cricao                          *\n");
	if((pin = fopen(FPD, "ab")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\n", FPD);
		exit(1);
	}
	tproduto produto;
	char descricao[51];
	int i = 1;

	do{
		printf("************************************\n");
		printf("Produto %03d\n", i++);
		printf("Codigo: %d\n", ++codigo);
		printf("Descricao: ");
		scanf(" %49[^\n]", descricao);
		while(getchar() != '\n');
		if(!strcmp(descricao, "sair"))
			break;

		produto.cod = codigo;
		strcpy(produto.descricao, descricao);
		fwrite(&produto, sizeof(tproduto), 1, pin);
	}while(strcmp(descricao, "sair"));
	fclose(pin);
	printf("\n");
}

int pegar_cod(const char * arquivo, int num)
{
	/*Esta função retorna o código do último produto cadastrado
	 *Parametro 1: uma string constante. É o tipo de arquivo que
	 			   será manipulado.
	 *Parametro 2: um inteiro. Indica qual será o tipo de arquivo a
	 			   ser manipulado.
	 			   Se 1: é do tipo 'tproduto';
	 			   Se 2: é do tipo 'tloja';
	 			   Se 3: é do tipo 'tpreco'.*/
	FILE * pout;
	int codigo;

	if((pout = fopen(arquivo, "a+b")) == NULL){
		fprintf(stderr, "\n\aErro ao ler o arquivo '%s'!\n", arquivo);
		exit(1);
	}
	fseek(pout, 0, SEEK_END);
	if(!ftell(pout)){
        fclose(pout);
        return 0;
	}
	switch(num){
        	case 1:
				codigo = ftell(pout) / sizeof(tproduto);
				break;
        	case 2:
           		codigo = ftell(pout) / sizeof(tloja);
           		break;
        	case 3:
           		codigo = ftell(pout) / sizeof(tpreco);
           		break;
        	default:
            		fprintf(stderr, "\n\aAlgo deu errado!\n");
	}
	fclose(pout);

	return codigo;
}

void listar_prod()
{
	/*Este procedimento mostra as informações sobre os produtos; baseia-se em ordem
	alfabética*/
	FILE * pout;

	printf("************************************\n");
	printf("*       LISTAGEM DE PRODUTOS       *\n");
	printf("************************************\n");

	if((pout = fopen(FPD, "rb")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\nTalvez ele esteja vazio!\n", FPD);
		return;
	}

	fseek(pout, 0, SEEK_END);
	int i, tam = ftell(pout)/sizeof(tproduto);
	tproduto produtos[tam];
	rewind(pout);
	fread(produtos, sizeof(tproduto), tam, pout);
	ordenar(1, tam, produtos, NULL, NULL);
	fclose(pout);

	register int maior = 11;//strlen(" Descricao") + 1
	for(i = 0; i < tam; i++){
		if(strlen(produtos[i].descricao) > maior){
			maior = strlen(produtos[i].descricao) + 1;
		}
	}
	printf("+-----------");
	printar_linha(maior, 1);
	printf("|Codigo     | Descricao");
	printar_linha(maior, strlen("Descricao"));
	printf("+-----------");
	printar_linha(maior, 1);
	for(i = 0; i < tam; i++){
        printf("+-----------");
		printar_linha(maior, 1);
        printf("|%010d | %s", produtos[i].cod, produtos[i].descricao);
		printar_linha(maior, strlen(produtos[i].descricao));
	}
	printf("+-----------");
	printar_linha(maior, 1);
}

void printar_linha(int maior, int tipo){
	/*Este procedimento printa ou uma linha ou o caractere '|', este indica o fim de uma linha.
	 *Argumentos: int maior - é o strlen() da maior palavra encontrada
	 			  int tipo - é o tipo de linha a ser printada (linha comum ou fim de linha)
	 *Tipos de linha: 1 - indica uma linha comum;
	 				  outro valor: imprime o indicador de fim de linha, '|'
	*/
	register int j = 0;
	if(tipo == 1){
		putchar('+');
		while(j < maior){
			putchar('-');
			j++;
		}
		puts("+");
		return;
	}
	int dif = maior - tipo - 1;
	while(j < dif){
		putchar(' ');
		j++;
	}
	puts("|");
	return;
	
}

void ordenar(int num, int max, tproduto * produtos, tloja * lojas, tpreco * precos)
{
	/*Ordenação por ordem alfabética usando Insertion Sort
	 *Parametro 1: um inteiro. Indica qual tipo de estrutura será ordenada.
	 		Se 1: é do tipo 'tproduto';
	 		Se 2: é do tipo'tloja';
	 		Se 3: é do tipo'tpreco'.
	 *Parametro 2: um inteiro. É o tamanho da estrutura a ser ordenada
	 *Parametro 3: uma estrutura do tipo 'tproduto'. Caso 'num' seja 1,
	 		então 'tloja' e 'tpreco' serão igual a 'NULL'. É a estrutura
	 		a ser ordenada.
	 *Parametro 4: uma estrutura do tipo 'tloja'. Caso 'num' seja 2,
	 		então 'tproduto' e 'tpreco' serão igual a 'NULL'. É a estrutura
	 		a ser ordenada.
	 *Parametro 5: uma estrutura do tipo 'tpreco'. Caso 'num' seja 3,
	 		então 'tproduto' será igual a 'NULL', 'tloja' receberá uma
			estrutura do tipo 'tloja' É a estrutura a ser ordenada.*/

	if(num == 1){
		register int i, j;
		tproduto key;
		for(i = 1; i < max; i++){
			key = produtos[i];
			j = i-1;
			while(j >= 0 && strcasecmp(produtos[j].descricao, key.descricao) > 0){
				produtos[j+1] = produtos[j];
				j--;
			}
			produtos[j+1] = key;
		}
		return;
	}

	if(num == 2){
		register int i, j;
		tloja key;
		for(i = 1; i < max; i++){
			key = lojas[i];
			j = i-1;
			while(j >= 0 && strcasecmp(lojas[j].nome, key.nome) > 0){
				lojas[j+1] = lojas[j];
				j--;
			}
			lojas[j+1] = key;
		}
		return;
	}
	if(num == 3){
		int tam_loja = pegar_cod(FLJ, 2);
		int indice;
		register int i, j;
		tpreco key;
		for(indice = 0; indice < tam_loja; indice++) {
			for(i = 1; i < max; i++){
				key = precos[i];
				j = i-1;
				while(j >= 0 && precos[j].cod_loja == lojas[indice].cod && precos[j].preco < key.preco){
					precos[j+1] = precos[j];
					j--;
				}
				precos[j+1] = key;
			}
		}
		return;
	}
}

void alterar_prod()
{
	/*Este procedimento altera a descrição de um produto*/
	FILE * pin;
	int codigo;

	printf("************************************\n");
	printf("*       ALTERACAO DE PRODUTO       *\n");
	printf("************************************\n");
	printf("* Digite 'sair' na nova descricao  *\n");
	printf("* para cancelar a acao             *\n");
	printf("************************************\n");
	printf("Codigo do produto: ");
	scanf("%d", &codigo);

	if((pin = fopen(FPD, "r+b")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\nTalvez ele esteja vazio!\n", FPD);
		return;
	}
	tproduto produto;
	fseek(pin, (codigo-1) * sizeof(tproduto), SEEK_SET);
	fread(&produto, sizeof(tproduto), 1, pin);

	if(produto.cod == codigo){
		printf("\nDescricao: %s\n", produto.descricao);
	}else{
		printf("\nProduto nao encontrado!\n");
		return;
	}

	char descricao[51];
	printf("Nova descricao: ");
	scanf(" %50[^\n]", descricao);
	while(getchar() != '\n');
	if(!strcmp(descricao, "sair")){
		fclose(pin);
		return;
	}
	produto.cod = codigo;
	strcpy(produto.descricao, descricao);
	fseek(pin, -sizeof(tproduto), SEEK_CUR);
	fwrite(&produto, sizeof(tproduto), 1, pin);
	fclose(pin);
}


void cadastrar_loja()
{
	/*Este procedimento cadastra novas lojas*/
	FILE * pin;
	int codigo = pegar_cod(FLJ, 2);

	printf("************************************\n");
	printf("*         CADASTRO DE LOJAS        *\n");
	printf("************************************\n");
	printf("* Para sair, digite 'sair' no nome *\n");
	printf("* ou no site                       *\n");
	if((pin = fopen(FLJ, "ab")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\n", FLJ);
		exit(1);
	}
	tloja loja;
	char nome[41], site[81];
	int i = 1;

	do{
		printf("************************************\n");
		printf("Loja %03d\n", i++);
		printf("Codigo: %d\n", ++codigo);
		printf("Nome: ");
		scanf(" %40[^\n]", nome);
		while(getchar() != '\n');
		if(!strcmp(nome, "sair"))
			break;
		printf("Site: ");
		scanf(" %80[^\n]", site);
		while(getchar() != '\n');
		if(!strcmp(site, "sair"))
			break;

		loja.cod = codigo;
		strcpy(loja.nome, nome);
		strcpy(loja.site, site);
		fwrite(&loja, sizeof(tloja), 1, pin);
	}while(strcmp(nome, "sair") && strcmp(site, "sair"));
	fclose(pin);
	printf("\n");
}

void listar_loja()
{
	/*Este procedimento mostra as informações sobre as lojas; baseia-se em ordem
	alfabética dos nomes*/
	FILE * pout;

	printf("************************************\n");
	printf("*        LISTAGEM DE LOJAS         *\n");
	printf("************************************\n");

	if((pout = fopen(FLJ, "rb")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\nTalvez ele esteja vazio!\n", FLJ);
		return;
	}

	fseek(pout, 0, SEEK_END);
	int i, tam = ftell(pout)/sizeof(tloja);
	tloja lojas[tam];
	rewind(pout);
	fread(lojas, sizeof(tloja), tam, pout);
	ordenar(2, tam, NULL, lojas, NULL);
	fclose(pout);
	char white[] = "                                        ";

	register int maior = 5;//strlen("Site") + 1
	for(i = 0; i < tam; i++){
		if(strlen(lojas[i].site) > maior){
			maior = strlen(lojas[i].site) + 1;
		}
	}
	printf("+-----------+-----------------------------------------");	
	printar_linha(maior, 1);
	printf("|Codigo     |Nome                                     | Site");
	printar_linha(maior, strlen("site"));
	printf("|-----------+-----------------------------------------");
	printar_linha(maior, 1);
	for(i = 0; i < tam; i++){
		printf("|-----------+-----------------------------------------");
		printar_linha(maior, 1);
		printf("|%010d |%s %s|%s ", lojas[i].cod, lojas[i].nome, &white[strlen(lojas[i].nome)], lojas[i].site);
		printar_linha(maior, strlen(lojas[i].site));
	}
	printf("+-----------+-----------------------------------------");
	printar_linha(maior, 1);
}

void cadastrar_preco()
{
	/*Este procedimento cadastra novos precos*/
	FILE * pin;
	int ultimo_prod = pegar_cod(FPD, 1);
	int ultimo_loja = pegar_cod(FLJ, 2);

	printf("************************************\n");
	printf("*        CADASTRO DE PRECOS        *\n");
	printf("************************************\n");
	printf("* Para sair, digite um valor < 0 em*\n");
	printf("* qualquer campo para sair         *\n");
	if((pin = fopen(FPC, "a+b")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\n", FPC);
		exit(1);
	}
	tpreco preco;
	register int i;
	int cod_loja, cod_produto, tam;
	float valor;

	do{
		printf("************************************\n");
		printf("Codigo do produto: ");
		scanf("%d", &cod_produto);
		if(cod_produto < 0)
			break;
		while(cod_produto > ultimo_prod){
			fprintf(stderr, "\n\aCodigo do produto nao reconhecido!\nCodigo do produto: ");
			scanf("%d", &cod_produto);
			if(cod_produto < 0)
				return;
		}
		printf("Codigo da loja: ");
		scanf("%d", &cod_loja);
		if(cod_loja < 0)
			break;
		while(cod_loja > ultimo_loja){
			fprintf(stderr, "\n\aCodigo da loja nao reconhecido!\nCodigo da loja: ");
			scanf("%d", &cod_loja);
			if(cod_loja < 0)
				return;
		}
		fseek(pin, 0, SEEK_END);
		tam = ftell(pin)/sizeof(tpreco);
		tpreco preco2;
		rewind(pin);

		while(fread(&preco2, sizeof(tpreco), 1, pin)){
			if(preco2.cod_produto == cod_produto && preco2.cod_loja == cod_loja){
				fprintf(stderr, "\n\aProduto ja cadastrado nessa loja!\n");
				break;
			}
		}
		if(preco2.cod_produto == cod_produto && preco2.cod_loja == cod_loja){
			continue;
		}
		printf("Preco: R$ ");
		scanf("%f", &valor);
		if(valor < 0)
			break;
		preco.cod_produto = cod_produto;
		preco.cod_loja = cod_loja;
		preco.preco = valor;
		fwrite(&preco, sizeof(tpreco), 1, pin);
	}while(cod_loja > 0 && cod_produto > 0 && valor > 0);
	fclose(pin);
	printf("\n");
}

void listar_preco()
{
	/*Este procedimento mostra as informações sobre os precos; baseia-se em ordem
	alfabética dos nomes*/
	FILE * pout;

	printf("************************************\n");
	printf("*       LISTAGEM DOS PRECOS        *\n");
	printf("************************************\n");

	//Lê as informações sobre as lojas
	if((pout = fopen(FLJ, "rb")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\nTalvez ele esteja vazio!\n", FLJ);
		return;
	}

	fseek(pout, 0, SEEK_END);
	int tam = ftell(pout)/sizeof(tloja);
	tloja lojas[tam];
	rewind(pout);
	fread(lojas, sizeof(tloja), tam, pout);
	fclose(pout);
	ordenar(2, tam, NULL, lojas, NULL);

	//Lê as informações sobre os preços
	if((pout = fopen(FPC, "rb")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\nTalvez ele esteja vazio!\n", FPC);
		return;
	}

	fseek(pout, 0, SEEK_END);
	int tam2 = ftell(pout)/sizeof(tpreco);
	tpreco precos[tam2];
	rewind(pout);
	fread(precos, sizeof(tpreco), tam2, pout);
	fclose(pout);
	ordenar(3, tam2, NULL, lojas, precos);

	//Lê as informações sobre os produtos
	if((pout = fopen(FPD, "rb")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\nTalvez ele esteja vazio!\n", FPD);
		return;
	}

	fseek(pout, 0, SEEK_END);
	int tam3 = ftell(pout)/sizeof(tproduto);
	tproduto produtos[tam3];
	rewind(pout);
	fread(produtos, sizeof(tproduto), tam3, pout);
	fclose(pout);

	register int i, j, maior = 22;//strlen("Descricao do produto") + 1
	char white[] = "                                         ";

	//Define o tamanho à direita da tabela
	for(i = 0; i < tam3; i++){
		if(strlen(produtos[i].descricao) > maior){
			for(j = 0; j < tam2; j++){
				if(precos[j].cod_loja == produtos[i].cod){
					maior = strlen(produtos[i].descricao) + 1;
				}
			}
		}
	}

	//Printa a tabela
	printf("+-----------------------------------------+------------------");
	printar_linha(maior, 1);
	printf("|Nome da loja                             | Preco do produto | Descricao do produto");
	printar_linha(maior, strlen("Descricao do produto"));
	printf("+-----------------------------------------+------------------");
	printar_linha(maior, 1);
	//Printa o nome das lojas em ordem alfabética
	for(i = 0; i < tam; i++){
		printf("+-----------------------------------------+------------------");
		printar_linha(maior, 1);
		printf("|%s%s|", lojas[i].nome, &white[strlen(lojas[i].nome)]);
		int controle = 1, nao_nulo = 0;
		//Printa os preços e descrições dos produtos correspondentes à loja
		for(j = 0; j < tam2; j++){
			if(lojas[i].cod == precos[j].cod_loja){
				nao_nulo = 1;
				if(controle){//Formata e printa a primeira linha de preço e descrição
					printf(" R$%.2f\t     | %s", precos[j].preco, produtos[precos[j].cod_produto - 1].descricao);
					printar_linha(maior, strlen(produtos[precos[j].cod_produto - 1].descricao));
					controle = 0;
					continue;
				}
				//Printa o restante das linhas de preços e descrições
				printf("|%s|------------------", white);
				printar_linha(maior, 1);
				printf("|%41s| R$%.2f\t     | %s", " ", precos[j].preco, produtos[precos[j].cod_produto - 1].descricao);
				printar_linha(maior, strlen(produtos[precos[j].cod_produto - 1].descricao));
			}
		}
		//Printa os produtos que não estão cadastrados em nenhuma loja
		if(!nao_nulo){
			printf("\t\t     | ");
			printar_linha(maior, 0);
			continue;
		}
		printf("|%s|------------------", white);
		printar_linha(maior, 1);
	}
	printf("+-----------------------------------------+------------------");	
	printar_linha(maior, 1);
}

void consultar_preco()
{
	/*Este procedimento faz consulta nos precos dos produtos*/
	FILE * pout;

	//produtos
	if((pout = fopen(FPD, "rb")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\nTalvez ele esteja vazio!\n", FPD);
		return;
	}
	fseek(pout, 0, SEEK_END);
	int tam1 = ftell(pout)/sizeof(tproduto);
	tproduto produtos[tam1];
	rewind(pout);
	fread(produtos, sizeof(tproduto), tam1, pout);
	fclose(pout);
	
	//preços
	if((pout = fopen(FPC, "rb")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\nTalvez ele esteja vazio!\n", FPC);
		return;
	}
	fseek(pout, 0, SEEK_END);
	int tam2 = ftell(pout)/sizeof(tpreco);
	tpreco precos[tam2];
	rewind(pout);
	fread(precos, sizeof(tpreco), tam2, pout);
	fclose(pout);
	
	//lojas
	if((pout = fopen(FLJ, "rb")) == NULL){
		fprintf(stderr, "\n\aErro ao abrir '%s'!\nTalvez ele esteja vazio!\n", FLJ);
		return;
	}
	fseek(pout, 0, SEEK_END);
	int tam3 = ftell(pout)/sizeof(tloja);
	tloja lojas[tam3];
	rewind(pout);
	fread(lojas, sizeof(tloja), tam3, pout);
	fclose(pout);

	printf("************************************\n");
	printf("*         CONSULTAR PRECOS         *\n");
	printf("************************************\n");
	printf("* Digite um codigo < 0 para sair   *\n");
	printf("************************************\n");

	int codigo;
	int ultimo_prod = pegar_cod(FPD, 1);
	char white[] = "                                                  ";
	do{
		printf("\nCodigo do produto: ");
		scanf("%d", &codigo);
		if(codigo < 0)
			break;
		while(codigo > ultimo_prod){
			fprintf(stderr, "\n\aCodigo invalido!\nCodigo do produto: ");
			scanf("%d", &codigo);
			if(codigo < 0)
				return;
		}

		register int i, maior = 14;//strlen(" Nome da loja") + 1
		for(i = 0; i < tam3; i++){
			if(strlen(lojas[i].nome) > maior){
				maior = strlen(lojas[i].nome) + 1;
			}
		}

		printf("+--------------------------------------------------+---------------------");
		printar_linha(maior, 1);
		printf("| Descricao do produto                             | Preco(s) do produto | Nome da loja");
		printar_linha(maior, strlen("Nome da loja"));
		printf("+--------------------------------------------------+---------------------");
		printar_linha(maior, 1);
		printf("|%s%s|", produtos[codigo - 1].descricao, &white[strlen(produtos[codigo - 1].descricao)]);
		int controle = 1, a = 1;
		i = 0;
		while(i < tam2){
			if(precos[i].cod_produto == codigo){
				a = 0;
				if(controle){
					printf(" R$ %.2f  \t         | %s", precos[i].preco, lojas[precos[i].cod_loja - 1].nome);
					printar_linha(maior, strlen(lojas[precos[i].cod_loja - 1].nome));
					controle = 0;
					i++;
					continue;
				}
				printf("|%s|---------------------", white);
				printar_linha(maior, 1);
				printf("|%s| R$ %.2f\t         | %s", white, precos[i].preco, lojas[precos[i].cod_loja - 1].nome);
				printar_linha(maior, strlen(lojas[precos[i].cod_loja - 1].nome));
			}
			i++;
		}
		printf("+--------------------------------------------------+---------------------");
		printar_linha(maior, 1);
		if(a)
			fprintf(stderr, "\n\aNao ha' precos cadastrados para esse produto!\n");
	}while(codigo > 0);
}
