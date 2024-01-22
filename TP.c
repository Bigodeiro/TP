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

void printaNonograma(char **mat, coord matSize, coord maxOffset, int **xCabeçalho, int **yCabeçalho)
{
    //LogicaPrintaNonograma.png
    //Parte 1
    for (int i = 0; i < maxOffset.y; i++)
    {
        printaChar((maxOffset.x + 1) * 2, ' ');


        for (int j = 0; j < matSize.x; j++)
        {
            if (xCabeçalho[j][0] > maxOffset.y - i - 1)
            {
                printf("%d", xCabeçalho[j][xCabeçalho[j][0] - maxOffset.y + i + 1]);
            }else printaChar(1, ' ');

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
            if (yCabeçalho[i][0] > maxOffset.x - j - 1)
            {
                printf("%d", yCabeçalho[i][yCabeçalho[i][0] - maxOffset.x + j + 1]);
            }else
                printaChar(1, ' ');
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

void* alocMat(int y, int x, int typeSize)
{
    int** mat = NULL;

    mat = malloc(y * sizeof(void*));
    
    for (int i = 0; i < y; i++)
    {
        mat[i] = malloc(x * typeSize);
    }

    return mat;
}

int main()
{
    coord maxOffset = {0, 0};
    coord matSize;
    int **xCabeçalho;
    int **yCabeçalho;
    char** mat;

    FILE *arqNonograma = fopen("nonograma.txt", "r");
    fscanf(arqNonograma, "%d %d", &matSize.y, &matSize.x);

    xCabeçalho = malloc(matSize.x * sizeof(int*));
    yCabeçalho = malloc(matSize.y * sizeof(int*));

    for (int i = 0; i < matSize.y; i++)
    {
        int aux;
        fscanf(arqNonograma, "%d", &aux);
        yCabeçalho[i] = malloc((aux + 1) * sizeof(int));
        yCabeçalho[i][0] = aux;

        for (int j = 0; j < aux; j++)
        {
            fscanf(arqNonograma, "%d", &yCabeçalho[i][j+1]);
        }

        maxOffset.y = aux > maxOffset.y ? aux : maxOffset.y;
    }
    for (int i = 0; i < matSize.x; i++)
    {
        int aux;
        fscanf(arqNonograma, "%d", &aux);
        xCabeçalho[i] = malloc((aux + 1) * sizeof(int));
        xCabeçalho[i][0] = aux;

        for (int j = 0; j < aux; j++)
        {
            fscanf(arqNonograma, "%d", &xCabeçalho[i][j+1]);
        }

        maxOffset.x = aux > maxOffset.x ? aux : maxOffset.x;
    }

    mat = alocMat(matSize.y, matSize.x, sizeof(char));

    for (int i = 0; i < matSize.y; i++)
    {
        for (int j = 0; j < matSize.x; j++)
        {
            char aux;

            fscanf(arqNonograma, "%c", &aux);

            if (aux == '\n' || aux == ' ')
            {
                j--;
                continue;
            }

            mat[i][j] = aux;
        }
    }

    printaNonograma(mat, matSize, maxOffset, xCabeçalho, yCabeçalho);


    fclose(arqNonograma);
    return 0;
}