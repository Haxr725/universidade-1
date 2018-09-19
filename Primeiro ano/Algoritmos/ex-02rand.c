/*
2)Crie uma fun��o que recebe uma string e transforma alguns dos caracteres
em mai�sculos e outros em min�sculos. Fa�a sorteios com a fun��o rand()
para gerar n�meros aleat�rios em C, que ser�o usados para escolher os
�ndices dos caracteres que ser�o alterados. Como par�metro deve ser
recebida a quantidade de sorteios que devem ser executados.

void transforma(char [], int qtde_sorteios);
------------------------------------------------------------------------
A FUN��O USADA CONSIDERA O ESPA�O EM BRANCO COMO CARACTERE.O EXERC�CIO PEDE
A TRANSFORMA��O DE CARACTERES, SEM LIMITAR A APENAS �S LETRAS DO ALFABETO.
ELA, TAMB�M, PODE SORTEAR O MESMO N�MEROS n VEZES.
*/

//BIBLIOTECAS E OS SEUS PORQU�S
#include <stdio.h>    /*padr�o*/
#include <string.h>   /*strcpy(); e strlen();*/
#include <stdlib.h>   /*rand(); e srand();*/
#include <ctype.h>    /*tolower(); e toupper();*/
#include <time.h>     /*rand(); e srand();*/

void transforma(char * func_str, int fun_lucky);//PROT�TIPO DA FUN��O

int main()
{
	char str[1001];
	int lucky;

	printf("string >>> ");
	scanf(" %1000[^\n]", str);
	printf("sorteio >>> ");
	scanf("%d", &lucky);

	transforma(str, lucky);
	printf("\n%s\n", str);

	return 0;
}

void transforma(char * func_str, int func_lucky)
{
	/*
	:A FUN��O USA UMA STRING AUXILIAR PARA SER MANIPULADA AO INV�S DA ORIGINAL.
	:S�O GERADOS N�MEROS ALEAT�RIOS POR srand(); E ESSE VALOR GERADO � LIMITADO
	PARA QUE SEJA APLICADO � STRING AUXILIAR, OU SEJA, FICA NO INTERVALOR DE 0
	E O TAMANHO M�XIMO DA STRING. ESSE VALOR QUE � LIMITADO VIRA, ENT�O, O �NDICE
	DO CARACTERE QUE SER� ALTERADO.
	:PARA DECIDIR SE UMA LETRA FICA MIN�SCULA OU MAI�SCULA � FEITA UMA AN�LISE,
	AP�S ISSO O "case sensitive" � ALTERADO PARA O OPOSTO.
	*/
	int i, random, lenght = strlen(func_str);
	char copy_str[lenght];

	strcpy(copy_str, func_str);//STRING AUXILIAR
	srand( (unsigned)time(NULL) );//GERA OS N�MEROS ALEAT�RIOS

	for(i = 0; i < func_lucky; i++){
		random  = rand() % lenght;//LIMITA O ALCANCE DOS N�MEROS
		//printf("INDICE DO CARACTERE SORTEADO: %d\n", random);//MOSTRA OS N�MEROS SORTEADOS
		if(copy_str[random] >= 'a' && copy_str[random] <= 'z')
			copy_str[random] = toupper(func_str[random]);
		else
			copy_str[random] = tolower(func_str[random]);
	}
	//printf("\n%s\n", copy_str);
	strcpy(func_str, copy_str);
}
/*
    NOME: RICARDO DE FREITAS OLIVERIA.
    R.A.: 5934078.
    CURSO: CI�NCIA DA COMPUTA��O.
*/
