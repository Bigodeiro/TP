#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void printaNonograma(char **mat, coord matSize, coord maxOffset, int **xCabeçalho, int **yCabeçalho)
{
    //LogicaPrintaNonograma.png
    //!Parte 1
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

    //!Parte 2
    printaChar((maxOffset.y + 1) * 2, ' ');

    for (int i = 0; i < matSize.x; i++)
    {
        printaChar(1, int2letra(i));
        printaChar(1, ' ');

    }
    printf("\n");
    
    //!Parte 3
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

char interpretaInput(char *str)
{
    if (strcmp(str, "x"       ) == 0) return 'v';
    if (strcmp(str, "-"       ) == 0) return 'v';
    if (strcmp(str, "."       ) == 0) return 'v';
    if (strcmp(str, "resolver") == 0) return 'r';
    if (strcmp(str, "sair"    ) == 0) return 'q';
    if (strcmp(str, "salvar"  ) == 0) return 's';
    if (strcmp(str, "comandos") == 0) return 'c';

    return -1;
}

void alteraNonograma(char **mat, coord matSize, coord pos, char c)
{
    if (pos.x < 0 || pos.x >= matSize.x || pos.y < 0 || pos.y >= matSize.y) {
        printf("Coordenada inválida\n");
        return;
    }

    mat[pos.y][pos.x] = c;
}

int main()
{
    printf("Bem vindo ao Nonograma!\n\n");
    printf("Comandos:\n");
    printf("x <x> <y> - Marca a casa (x, y) com um X\n");
    printf("- <x> <y> - Marca a casa (x, y) com um -\n");
    printf(". <x> <y> - Marca a casa (x, y) com um .\n");
    printf("resolver  - Resolve o nonograma\n");
    printf("salvar    - Salva o nonograma\n");
    printf("sair      - Sai do programa\n\n\n");

    //? Criação de variáveis
    coord maxOffset = {0, 0};
    coord matSize;
    int **xCabeçalho;
    int **yCabeçalho;
    int continuar = 1;
    char** mat;
    char input[10];

    //? Leitura de arquivo
    FILE *arqNonograma = fopen("nonograma.txt", "r");
    fscanf(arqNonograma, "%d %d", &matSize.y, &matSize.x);

    //? Alocação dos cabeçalhos
    xCabeçalho = malloc(matSize.x * sizeof(int*));
    yCabeçalho = malloc(matSize.y * sizeof(int*));

    //? Ler cabeçalho y
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

    //? Ler cabeçalho x
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

    //? Ler e alocar matriz nonograma
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

    //! Código a ser executado em loop
    while (continuar)
    {
        int x, y;
        printaNonograma(mat, matSize, maxOffset, xCabeçalho, yCabeçalho);
        printf("\nDigite um comando: ");
        scanf("%s", input);

        switch (interpretaInput(input))
        {
        case 'v':
            scanf("%d %d", &x, &y);
            alteraNonograma(mat, matSize, (coord){x, y}, input[0]);
            break;

        case 'r':
            printf("Resolver\n");
            break;

        case 'q':
            printf("Sair\n");
            continuar = 0;
            break;

        case 's':
            printf("Salvar\n");
            break;
        
        case 'c':
            printf("Comandos:\n");
            printf("x <x> <y> - Marca a casa (x, y) com um X\n");
            printf("- <x> <y> - Marca a casa (x, y) com um -\n");
            printf(". <x> <y> - Marca a casa (x, y) com um .\n");
            printf("resolver  - Resolve o nonograma\n");
            printf("salvar    - Salva o nonograma\n\n");
            printf("sair      - Sai do programa\n");
            break;

        default:
            printf("Comando inválido!\n");
            break;

        }
        printaChar(19, '\n');
    }

    //? Liberação de memória
    for (int i = 0; i < matSize.y; i++)
    {
        free(mat[i]);
    }
    free(mat);
    for (int i = 0; i < matSize.x; i++)
    {
        free(xCabeçalho[i]);
    }
    free(xCabeçalho);
    for (int i = 0; i < matSize.y; i++)
    {
        free(yCabeçalho[i]);
    }
    free(yCabeçalho);
    fclose(arqNonograma);


    return 0;
}