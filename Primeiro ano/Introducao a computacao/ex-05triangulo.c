#include <stdio.h>
/*
Escreva uma fun��o de prot�tipo void �triangulo(int �n,�char �ch); que desenha no
v�deo um tri�ngulo invertido formado por n caracteres ch em sua base. Por exemplo, se for feita
seguinte chamada a fun��o: triangulo(7,�'#');
A fun��o deve desenhar no v�deo o seguinte tri�ngulo:
#######
�#####
��###
���#
*/
void tri(int, char);

int main()
{
    int n, i;
    char ch;

    scanf("%d %c", &n, &ch);
    while(n % 2 == 0){
        printf("\nInforme um n�mero impar: ");
        scanf("%d", &n);
    }
    tri(n, ch);

    return 0;
}

void tri(int x, char c)
{
    int i, j;

    for(i = n; i < (n + 1) / 2; i--)
    {
        printf("%c");
    }

}
