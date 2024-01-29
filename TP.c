#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>

typedef struct
{
    int x;
    int y;
} coord;

char int2letra(int n) // 0 -> A, 1 -> B, ...
{
    return n + 'A';
}

int letra2int(int n) // A -> 0, B -> 1, ...
{
    return n - 'A';
}

void printaComandos ()
{
    printf("Comandos:\n");
    printf("x <x><y> - Marca a casa (x, y) com um X\n");
    printf("- <x><y> - Marca a casa (x, y) com um -\n");
    printf(". <x><y> - Marca a casa (x, y) com um .\n");
    printf("resolver  - Resolve o nonograma\n");
    printf("salvar    - Salva o nonograma\n\n");
    printf("sair      - Sai do programa\n");
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
        printaChar((maxOffset.x * 3) + 1, ' ');

        for (int j = 0; j < matSize.x; j++)
        {
            if (yCabeçalho[j][0] > maxOffset.y - i - 1)
            {
                printf("%2d", yCabeçalho[j][yCabeçalho[j][0] - maxOffset.y + i + 1]);
            }else printaChar(2, ' ');

            printaChar(1, ' ');
        }
        printf("\n");
    }

    //!Parte 2
    printaChar((maxOffset.x * 3) + 2, ' ');

    for (int i = 0; i < matSize.x; i++)
    {
        printaChar(1, int2letra(i));
        printaChar(2, ' ');

    }
    printf("\n");

    //!Parte 3
    for (int i = 0; i < matSize.y; i++)
    {
        printaChar((maxOffset.x - xCabeçalho[i][0]) * 3, ' ');

        for (int j = 0; j < xCabeçalho[i][0]; j++)
        {
            printf("%2d ", xCabeçalho[i][j+1]);
        }


        printaChar(1, int2letra(i));
        
        for (int j = 0; j < matSize.x; j++)
        {
            
            printf(" %c ", mat[i][j]);
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

int alteraNonograma(char **mat, coord matSize, coord pos, char c)
{
    if (pos.x < 0 || pos.x >= matSize.x || pos.y < 0 || pos.y >= matSize.y) {
        return 2;
    }

    mat[pos.y][pos.x] = c;
    return 0;
}

void salvaArquivo(char *path, char **mat, coord matSize, int **xCabeçalho, int **yCabeçalho)
{
    FILE *arqNonograma = fopen(path, "w");
    
    if (arqNonograma == NULL) 
    {
        arqNonograma = fopen(path, "a");
        if (arqNonograma == NULL) 
        {
            printf("Erro ao criar o arquivo.\n");
            return;
        }
    }

    fprintf(arqNonograma, "%d %d\n", matSize.y, matSize.x);

    for (int i = 0; i < matSize.y; i++)
    {
        fprintf(arqNonograma, "%d ", xCabeçalho[i][0]);

        for (int j = 0; j < xCabeçalho[i][0]; j++)
        {
            fprintf(arqNonograma, "%d ", xCabeçalho[i][j+1]);
        }
        fprintf(arqNonograma, "\n");
    }

    for (int i = 0; i < matSize.x; i++)
    {
        fprintf(arqNonograma, "%d ", yCabeçalho[i][0]);

        for (int j = 0; j < yCabeçalho[i][0]; j++)
        {
            fprintf(arqNonograma, "%d ", yCabeçalho[i][j+1]);
        }
        fprintf(arqNonograma, "\n");
    }

    for (int i = 0; i < matSize.y; i++)
    {
        for (int j = 0; j < matSize.x; j++)
        {
            fprintf(arqNonograma, "%c", mat[i][j]);
        }
        fprintf(arqNonograma, "\n");
    }

    fclose(arqNonograma);
}

int main (int argc, char *argv[])
{
    printf("Bem vindo ao Nonograma!\n\n");
    printaComandos();

    //? Criação de variáveis
    char path[260];
    strcpy(path, argv[1]);
    coord maxOffset = {0, 0};
    coord matSize;
    int **xCabeçalho;
    int **yCabeçalho;
    int continuar = 1;
    char** mat;
    char input[10];
    int recado = 0; //? 0 -> sem recado, 1 -> recado de comando inválido, 2 -> recado de coordenada inválida, 3 -> recado lista de comandos

    //? Leitura de arquivo
    FILE *arqNonograma = fopen(path, "r");
    fscanf(arqNonograma, "%d %d", &matSize.y, &matSize.x);

    //? Alocação dos cabeçalhos
    xCabeçalho = malloc(matSize.y * sizeof(int*));
    yCabeçalho = malloc(matSize.x * sizeof(int*));

    //? Ler cabeçalho x
    for (int i = 0; i < matSize.y; i++)
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

    //? Ler cabeçalho y
    for (int i = 0; i < matSize.x; i++)
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
        char cordenadas[3] = {0, 0};

        //Recado a ser dado ao jogador entre um resultado e outro
        switch (recado)

        {
        case 1:
            printf("Comando inválido, digite novamente\n");
            break;
        
        case 2:
            printf("Coordenada inválida, digite novamente\n");
            break;
        
        case 3:
            printaComandos();
            break;
        
        default:
            break;
        }
        printaNonograma(mat, matSize, maxOffset, xCabeçalho, yCabeçalho);
        
        printf("\nDigite um comando: ");
        scanf("%s", input);
        fflush(stdin);

        recado = 0;
        switch (interpretaInput(input))
        {
        case 'v':
            scanf("%s", cordenadas);
            char x = cordenadas[0];
            char y = cordenadas[1];
            recado = alteraNonograma(mat, matSize, (coord){ letra2int(x), letra2int(y) }, input[0]);
            break;

        case 'r':
            printf("Resolver\n");
            break;

        case 'q':
            continuar = 0;
            break;

        case 's':
            char pathCaminho[260];
            scanf("%s", pathCaminho);
            salvaArquivo(pathCaminho, mat, matSize, xCabeçalho, yCabeçalho);
            break;
        
        case 'c':
            recado = 3;
            break;

        default:
            recado = 1;
            break;

        }
    }

    //? Liberação de memória
    for (int i = 0; i < matSize.y; i++)
    {
        free(mat[i]);
    }
    free(mat);

    for (int i = 0; i < matSize.y; i++)
    {
        free(xCabeçalho[i]);
    }
    free(xCabeçalho);

    for (int i = 0; i < matSize.x; i++)
    {
        free(yCabeçalho[i]);
    }
    free(yCabeçalho);

    fclose(arqNonograma);
    return 0;
}