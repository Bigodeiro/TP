#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdio_ext.h>

typedef struct
{
    int x;
    int y;
} coord;

// 0 -> A, 1 -> B, ...
char int2letra(int n) 
{
    return n + 'A';
}

// A -> 0, B -> 1, ...
int letra2int(char c) 
{
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    } else if (c >= 'a' && c <= 'z') {
        return c - 'a';
    } else {
        return -1;
    }
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

void printaNonograma(char **mat, coord matSize, coord maxOffset, int **xCabecalho, int **yCabecalho)
{
    //LogicaPrintaNonograma.png
    //!Parte 1
    for (int i = 0; i < maxOffset.y; i++)
    {
        printaChar((maxOffset.x * 3) + 1, ' ');

        for (int j = 0; j < matSize.x; j++)
        {
            if (yCabecalho[j][0] > maxOffset.y - i - 1)
            {
                printf("%2d", yCabecalho[j][yCabecalho[j][0] - maxOffset.y + i + 1]);
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
        printaChar((maxOffset.x - xCabecalho[i][0]) * 3, ' ');

        for (int j = 0; j < xCabecalho[i][0]; j++)
        {
            printf("%2d ", xCabecalho[i][j+1]);
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

int alteraNonograma(char **mat, coord matSize, coord pos, char c, char *buffer)
{
    if (pos.x < 0 || pos.x >= matSize.x || pos.y < 0 || pos.y >= matSize.y) {
        return 2;
    }

    *buffer = mat[pos.y][pos.x];
    mat[pos.y][pos.x] = c;
    return 0;
}

void salvaArquivo(char *path, char **mat, coord matSize, int **xCabecalho, int **yCabecalho)
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
        fprintf(arqNonograma, "%d ", xCabecalho[i][0]);

        for (int j = 0; j < xCabecalho[i][0]; j++)
        {
            fprintf(arqNonograma, "%d ", xCabecalho[i][j+1]);
        }
        fprintf(arqNonograma, "\n");
    }

    for (int i = 0; i < matSize.x; i++)
    {
        fprintf(arqNonograma, "%d ", yCabecalho[i][0]);

        for (int j = 0; j < yCabecalho[i][0]; j++)
        {
            fprintf(arqNonograma, "%d ", yCabecalho[i][j+1]);
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

int comparaVetMat_Cabecalho(char* vetMat, int tamVetMat, int* vetCabecalhoGabarito)
{
    //retorna 0 se igual, 1 se ainda esta sendo preenchido, 2 se erro
    // Cria o vetor que sera comparado com o cabecalho gabarito
    int* vetCabecalhoMat = malloc((tamVetMat + 1) * sizeof(int));

    int flag = 0;
    int count = 0;
    int index = 1;

    //Essa variavel guarda o maior elemento do cabecalho do gabarito
    int maiorElemGabarito = 0;
    for (int i = 1; i <= vetCabecalhoGabarito[0]; i++) 
    {
        maiorElemGabarito = (vetCabecalhoGabarito[i] > maiorElemGabarito) ? vetCabecalhoGabarito[i] : maiorElemGabarito;
    }

    for (int i = 0; i < tamVetMat; i++) 
    {
        // Conta a quantidade de 'x' seguidos
        if (vetMat[i] == 'x' || vetMat[i] == 'X') 
        {
            count++;
        } else if (count > 0) 
        {
            // Guarda a quantidade de 'x' seguidos no vetor
            vetCabecalhoMat[index] = count;
            count = 0;
            index++;
        }
    }
    if (count > 0) 
    {
        vetCabecalhoMat[index] = count;
        index++;
    }
    vetCabecalhoMat[0] = index - 1;

    // Se o tamanho do cabecalho da matriz for maior que o do cabecalho do gabarito retorna 2 (erro)
    if (vetCabecalhoMat[0] > vetCabecalhoGabarito[0]) 
    {
        free(vetCabecalhoMat);
        return 2;
    }else if (vetCabecalhoMat[0] < vetCabecalhoGabarito[0])  //Se o tamanho do cabecalho da matriz for menor que o do cabecalho do gabarito deve ser analisado com calma
    {
        //Esse eh um caso em que o tamanho dos cabecalhos sao diferentes mas ainda esta apenas parcialmente preenchido, portanto nao eh erro
        for (int i = 1; i <= vetCabecalhoMat[0]; i++) 
        {
            //Se tiver algum elemento do cabecalho da matriz maior que o maior elemento do cabecalho do gabarito, retorna 2 (erro)
            if (vetCabecalhoMat[i] > maiorElemGabarito)
            {
                free(vetCabecalhoMat);
                return 2;
            }
        }

        //caso contrario, retorna 1 (nao eh erro), o nonograma ainda esta sendo preenchido
        free(vetCabecalhoMat);
        return 1;
    }

    // Caso o programa chegue aqui, os tamanhos dos cabecalhos sao iguais, oque significa que so resta comparar os elementos
    // Compare each element of the vectors
    for (int i = 1; i <= vetCabecalhoMat[0]; i++) 
    {
        if (vetCabecalhoMat[i] > vetCabecalhoGabarito[i]) 
        {
            free(vetCabecalhoMat);
            return 2;
        }
        else if (vetCabecalhoMat[i] < vetCabecalhoGabarito[i]) flag = 1;
    }

    free(vetCabecalhoMat);
    return flag;
}


int main (int argc, char *argv[])
{
    printf("Bem vindo ao Nonograma!\n\n");
    printaComandos();

    //? Criacao de variaveis
    char path[260];
    strcpy(path, argv[1]);
    coord maxOffset = {0, 0};
    coord matSize;
    int **xCabecalho;
    int **yCabecalho;
    int continuar = 1;
    char** mat;
    char input[10];
    int recado = 0; //? 0 -> sem recado, 1 -> recado de comando invalido, 2 -> recado de coordenada invalida, 3 -> recado lista de comandos

    //? Leitura de arquivo
    FILE *arqNonograma = fopen(path, "r");
    fscanf(arqNonograma, "%d %d", &matSize.y, &matSize.x);

    //? Alocacao dos cabecalhos
    xCabecalho = malloc(matSize.y * sizeof(int*));
    yCabecalho = malloc(matSize.x * sizeof(int*));

    //? Ler cabecalho x
    for (int i = 0; i < matSize.y; i++)
    {
        int aux;
        fscanf(arqNonograma, "%d", &aux);
        xCabecalho[i] = malloc((aux + 1) * sizeof(int));
        xCabecalho[i][0] = aux;

        for (int j = 0; j < aux; j++)
        {
            fscanf(arqNonograma, "%d", &xCabecalho[i][j+1]);
        }

        maxOffset.x = aux > maxOffset.x ? aux : maxOffset.x;
    }

    //? Ler cabecalho y
    for (int i = 0; i < matSize.x; i++)
    {
        int aux;
        fscanf(arqNonograma, "%d", &aux);
        yCabecalho[i] = malloc((aux + 1) * sizeof(int));
        yCabecalho[i][0] = aux;

        for (int j = 0; j < aux; j++)
        {
            fscanf(arqNonograma, "%d", &yCabecalho[i][j+1]);
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

    //! Codigo a ser executado em loop
    while (continuar)
    {
        char cordenadas[3] = {0, 0};
        char pathCaminho[260];

        //Recado a ser dado ao jogador entre um resultado e outro
        switch (recado)
        {
        case 1:
            printf("Comando invalido, digite novamente\n");
            break;
        
        case 2:
            printf("Coordenada invalida, digite novamente\n");
            break;
        
        case 3:
            printaComandos();
            break;
        case 4:
            printf("Você infligiu as regras do nonograma, portanto o ultimo movimento foi desfeito\n");
            break;
        
        default:
            break;
        }
        printaNonograma(mat, matSize, maxOffset, xCabecalho, yCabecalho);
        
        printf("\nDigite um comando: ");
        scanf("%s", input);
        //fflush(stdin);

        recado = 0;
        switch (interpretaInput(input))
        {
        case 'v':
            scanf("%s", cordenadas);
            int y = letra2int(cordenadas[0]);
            int x = letra2int(cordenadas[1]);
            char buffer;
            recado = alteraNonograma(mat, matSize, (coord){ x, y }, input[0], &buffer);

            //Checa se a alteracao eh valida

            if (recado != 2) 
            {
                printf("%d", x);
                if (comparaVetMat_Cabecalho(mat[y], matSize.x, xCabecalho[y]) == 2)
                {
                    recado = 4;
                    //desfaz a alteracao
                    alteraNonograma(mat, matSize, (coord){ x, y }, buffer, &buffer);
                }
            }
            //TODO: FAZER O MESMO PARA A COLUNA E DEPOIS CHECAR SE O NONOGRAMA FOI RESOLVIDO
            
            //checa se o nonograma foi resolvido

            break;

        case 'r':
            printf("Resolver\n");
            break;

        case 'q':
            continuar = 0;
            break;

        case 's':
            scanf("%s", pathCaminho);
            salvaArquivo(pathCaminho, mat, matSize, xCabecalho, yCabecalho);
            break;
        
        case 'c':
            recado = 3;
            break;

        default:
            recado = 1;
            break;

        }
    }

    //? Liberacao de memoria
    for (int i = 0; i < matSize.y; i++)
    {
        free(mat[i]);
    }
    free(mat);

    for (int i = 0; i < matSize.y; i++)
    {
        free(xCabecalho[i]);
    }
    free(xCabecalho);

    for (int i = 0; i < matSize.x; i++)
    {
        free(yCabecalho[i]);
    }
    free(yCabecalho);

    fclose(arqNonograma);
    return 0;
}