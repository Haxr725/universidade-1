#include <stdio.h>
#include <string.h>
/*
Escreva uma fun��o que receba uma string e substitua cada segmento de dois ou mais espa�os em branco por um s� caracter ' '.
*/
int main()
{
    char str[301];
    int i, j, k, cont = 0;

    scanf("%300[^\n]", str);
    for(i = 0; str[i] != '\0'; i++){
        cont = 0;
        if(str[i] == ' '){/*PROCURA POR ESPA�OS*/
            for(j = i; str[j] == ' '; j++){/*CONTA QUANTOS ESPA�OS SEGUIDOS TEM*/
                cont++;
            }
            if(cont > 1){/*SE TIVER MAIS DE 1 ESPA�O, ENT�O PRECISO OS RETIRAR*/
                for(k = i + 1; str[k] != '\0'; k++, j++){/*PEGA O �NDICE DO PR�XIMO CARACTERE AP�S O �LTIMO ESPA�O E COPIA O QUE TEM EM DIANTE PARA O �NDICE DO 2 ESPA�O EM DIANTE*/
                    str[k] = str[j];
                }
            }
        }
    }
    printf("\n%s\n", str);
    return 0;
}
