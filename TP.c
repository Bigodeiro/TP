#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CINZA  0
#define PRETO  1
#define BRANCO 2

typedef struct
{
    int x;
    int y;
} coord;

char int2letra(int n) // 0 -> A, 1 -> B, ...
{
    return n + 'A';
}

void printaChar(int n, char c)
{
    for (int i = 0; i < n; i++) printf("%c", c);
}

void printaNonograma(char **mat, coord matSize, coord maxOffset)
{
    //LogicaPrintaNonograma.png
    //Parte 1
    for (int i = 0; i < maxOffset.y; i++)
    {
        printaChar((maxOffset.x + 1) * 2, ' ');


        for (int j = 0; j < matSize.x; j++)
        {
            printaChar(1, 'C');
            printaChar(1, ' ');
        }
        printf("\n");
    }

    //Parte 2
    printaChar((maxOffset.y + 1) * 2, ' ');

    for (int i = 0; i < matSize.x; i++)
    {
        printaChar(1, int2letra(i));
        printaChar(1, ' ');

    }
    printf("\n");
    
    //Parte 3
    for (int i = 0; i < matSize.y; i++)
    {
        for (int j = 0; j < maxOffset.x; j++)
        {
            printaChar(1, 'C');
            printaChar(1, ' ');
        }

        printaChar(1, int2letra(i));

        for (int j = 0; j < matSize.x; j++)
        {
            printf(" %c", mat[i][j]);
        }
    
        printf("\n");
    }
}

int main()
{
    coord maxOffset = {0, 0};
    coord matSize;
    int **xCabeçalho;
    int **yCabeçalho;

    FILE *arqNonograma = fopen("nonograma.txt", "r");
    fscanf(arqNonograma, "%d %d", &matSize.y, &matSize.x);

    xCabeçalho = malloc(matSize.x * sizeof(int*));
    yCabeçalho = malloc(matSize.y * sizeof(int*));

    for (int i = 0; i < matSize.y; i++)
    {
        int aux;
        fscanf(arqNonograma, "%d", &aux);
        yCabeçalho[i] = malloc(aux * sizeof(int));

        for (int j = 0; j < aux; j++)
        {
            fscanf(arqNonograma, "%d", &yCabeçalho[i][j]);
        }

        maxOffset.y = aux > maxOffset.y ? aux : maxOffset.y;
    }
    for (int i = 0; i < matSize.x; i++)
    {
        int aux;
        fscanf(arqNonograma, "%d", &aux);
        xCabeçalho[i] = malloc(aux * sizeof(int));

        for (int j = 0; j < aux; j++)
        {
            fscanf(arqNonograma, "%d", &xCabeçalho[i][j]);
        }

        maxOffset.x = aux > maxOffset.x ? aux : maxOffset.x;
    }

    //Cria e preenche o nonograma
    char **mat = malloc(matSize.y * sizeof(char*));
    for (int i = 0; i < matSize.y; i++)
    {
        mat[i] = malloc(matSize.x * sizeof(char));
        for (int j = 0; j < matSize.x; j++)
        {
            char aux;
            fscanf(arqNonograma, "%c", &aux);

            if (aux != '\n' && aux != ' ')
            {
                mat[i][j] = aux - '0';
            }
            else
            {
                j--;
            } 
        }
    }

    printaNonograma(mat, matSize, maxOffset);


    return 0;
}