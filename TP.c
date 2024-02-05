#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdio_ext.h>

#define true 1
#define false 0

typedef struct
{
    int x;
    int y;
} coord;

void beep()
{
    printf("\a");
}

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
    printf("x <x><y>          - Marca a casa (x, y) com um X\n");
    printf("- <x><y>          - Marca a casa (x, y) com um -\n");
    printf(". <x><y>          - Marca a casa (x, y) com um .\n");
    printf("resolver          - Resolve o nonograma\n");
    printf("salvar <nome.txt> - Salva o nonograma\n\n");
    printf("sair              - Sai do programa\n");
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

void printaNonograma(char **mat, coord tamMat, coord espacamentoMax, int **xCabecalho, int **yCabecalho)
{
    //LogicaPrintaNonograma.png
    //!Parte 1
    for (int i = 0; i < espacamentoMax.y; i++)
    {
        printaChar((espacamentoMax.x * 3) + 1, ' ');

        for (int j = 0; j < tamMat.x; j++)
        {
            if (yCabecalho[j][0] > espacamentoMax.y - i - 1)
            {
                printf("%2d", yCabecalho[j][yCabecalho[j][0] - espacamentoMax.y + i + 1]);
            }else printaChar(2, ' ');

            printaChar(1, ' ');
        }
        printf("\n");
    }

    //!Parte 2
    printaChar((espacamentoMax.x * 3) + 2, ' ');

    for (int i = 0; i < tamMat.x; i++)
    {
        printaChar(1, int2letra(i));
        printaChar(2, ' ');

    }
    printf("\n");

    //!Parte 3
    for (int i = 0; i < tamMat.y; i++)
    {
        printaChar((espacamentoMax.x - xCabecalho[i][0]) * 3, ' ');

        for (int j = 0; j < xCabecalho[i][0]; j++)
        {
            printf("%2d ", xCabecalho[i][j+1]);
        }


        printaChar(1, int2letra(i));
        
        for (int j = 0; j < tamMat.x; j++)
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

int alteraNonograma(char **mat, coord tamMat, coord pos, char c, char *buffer)
{
    if (pos.x < 0 || pos.x >= tamMat.x || pos.y < 0 || pos.y >= tamMat.y) {
        return 2;
    }

    *buffer = mat[pos.y][pos.x];
    mat[pos.y][pos.x] = c;
    return 0;
}

void salvaArquivo(char *path, char **mat, coord tamMat, int **xCabecalho, int **yCabecalho)
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

    fprintf(arqNonograma, "%d %d\n", tamMat.y, tamMat.x);

    for (int i = 0; i < tamMat.y; i++)
    {
        fprintf(arqNonograma, "%d ", xCabecalho[i][0]);

        for (int j = 0; j < xCabecalho[i][0]; j++)
        {
            fprintf(arqNonograma, "%d ", xCabecalho[i][j+1]);
        }
        fprintf(arqNonograma, "\n");
    }

    for (int i = 0; i < tamMat.x; i++)
    {
        fprintf(arqNonograma, "%d ", yCabecalho[i][0]);

        for (int j = 0; j < yCabecalho[i][0]; j++)
        {
            fprintf(arqNonograma, "%d ", yCabecalho[i][j+1]);
        }
        fprintf(arqNonograma, "\n");
    }

    for (int i = 0; i < tamMat.y; i++)
    {
        for (int j = 0; j < tamMat.x; j++)
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

    int* vetCabecalhoMat = malloc((tamVetMat + 1) * sizeof(int));

    int flag = 0;
    int count = 0;
    int index = 1;

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

    //!Caso 1
    if (vetCabecalhoMat[0] > vetCabecalhoGabarito[0]) 
    {
        int nGruposVerdadeiro = 0;
        //? Calculando o numero real de possiveis grupos ex: considera-se "x-x" um grupo so quando o cabeçalho tem um 3
        for (int i = 0; i < tamVetMat; i++)
        {
            if (vetMat[i] == 'x')
            {
                nGruposVerdadeiro++;
                i += (vetCabecalhoGabarito[nGruposVerdadeiro] - 1);
            }
        }

        printf("N real de grupos: %d\n", nGruposVerdadeiro);

        if (nGruposVerdadeiro > vetCabecalhoGabarito[0])
        {
            printf("chego");
            free(vetCabecalhoMat);
            return 2;
        }else
        {
            return 1;
        }
    }
    //!Caso 2   
    else if (vetCabecalhoMat[0] < vetCabecalhoGabarito[0])  //Se o tamanho do cabecalho da matriz for menor que o do cabecalho do gabarito deve ser analisado com calma
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
    //!Caso 3
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

char * getMatVet(char **mat, coord tamMat, int isCol, int index)
{
    char *vet = malloc((isCol ? tamMat.y : tamMat.x) * sizeof(char));

    for (int i = 0; i < (isCol ? tamMat.y : tamMat.x); i++)
    {
        vet[i] = isCol ? mat[i][index] : mat[index][i];
    }

    return vet;
}

int checaVitoria(char **mat, coord tamMat, int **xCabecalho, int **yCabecalho)
{
    char *aux = NULL;
    int resolvido = 1;

    for (int i = 0; i < tamMat.y; i++)
    {
        aux = getMatVet(mat, tamMat, false, i);
        if (comparaVetMat_Cabecalho(mat[i], tamMat.x, xCabecalho[i]) != 0)
        {
            resolvido = 0;
            free(aux);
            break;
        }
        free(aux);
    }

    if (resolvido)
    {
        for (int i = 0; i < tamMat.x; i++)
        {
            aux = getMatVet(mat, tamMat, true, i);
            if (comparaVetMat_Cabecalho(aux, tamMat.y, yCabecalho[i]) != 0)
            {
                //print aux
                printf("Erro na coluna %d\n", i);
                for (int j = 0; j < tamMat.y; j++)
                { 
                    printf("%c", aux[j]);
                }
                printf("\n");
                
                //print cabecalho y
                for (int j = 0; j < yCabecalho[i][0]; j++)
                {
                    printf("%d ", yCabecalho[i][j+1]);
                }
                printf("\n");
                
                free(aux);
                resolvido = 0;
                break;
            }
            free(aux);
        }
    }

    return resolvido;
}

int checaJogada(char **mat, coord tamMat, int **xCabecalho, int **yCabecalho, coord pos, char c)
{
    char* aux = getMatVet(mat, tamMat, false, pos.y);
    if (comparaVetMat_Cabecalho(mat[pos.y], tamMat.x, xCabecalho[pos.y]) == 2)
    {
        free(aux);
        return 4;
    }
    free(aux);

    aux = getMatVet(mat, tamMat, true, pos.x);
    if (comparaVetMat_Cabecalho(aux, tamMat.y, yCabecalho[pos.x]) == 2)
    {
        free(aux);
        return 4;
    }
    free(aux);

    return 0;
}

int main (int argc, char *argv[])
{
    //? Criacao de variaveis
    int boasVindasFlag = 1;
    char path[260];
    strcpy(path, argv[1]);
    coord espacamentoMax = {0, 0};
    coord tamMat;
    int **xCabecalho;
    int **yCabecalho;
    int continuar = 1;
    char** mat;
    char input[10];
    int recado = 0; //? 0 -> sem recado, 1 -> recado de comando invalido, 2 -> recado de coordenada invalida, 3 -> recado lista de comandos

    //? Leitura de arquivo
    FILE *arqNonograma = fopen(path, "r");
    fscanf(arqNonograma, "%d %d", &tamMat.y, &tamMat.x);

    //? Alocacao dos cabecalhos
    xCabecalho = malloc(tamMat.y * sizeof(int*));
    yCabecalho = malloc(tamMat.x * sizeof(int*));

    //? Ler cabecalho x
    for (int i = 0; i < tamMat.y; i++)
    {
        int aux;
        fscanf(arqNonograma, "%d", &aux);
        xCabecalho[i] = malloc((aux + 1) * sizeof(int));
        xCabecalho[i][0] = aux;

        for (int j = 0; j < aux; j++)
        {
            fscanf(arqNonograma, "%d", &xCabecalho[i][j+1]);
        }

        espacamentoMax.x = aux > espacamentoMax.x ? aux : espacamentoMax.x;
    }

    //? Ler cabecalho y
    for (int i = 0; i < tamMat.x; i++)
    {
        int aux;
        fscanf(arqNonograma, "%d", &aux);
        yCabecalho[i] = malloc((aux + 1) * sizeof(int));
        yCabecalho[i][0] = aux;

        for (int j = 0; j < aux; j++)
        {
            fscanf(arqNonograma, "%d", &yCabecalho[i][j+1]);
        }

        espacamentoMax.y = aux > espacamentoMax.y ? aux : espacamentoMax.y;
    }

    //? Ler e alocar matriz nonograma
    mat = alocMat(tamMat.y, tamMat.x, sizeof(char));
    for (int i = 0; i < tamMat.y; i++)
    {
        for (int j = 0; j < tamMat.x; j++)
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
        char cordenadas[260] = {0, 0};
        char pathCaminho[260];

        if (boasVindasFlag)
        {
            printaChar(20, '\n');
            printf("Bem vindo ao Nonograma!");
            printaComandos();
            boasVindasFlag = 0;
        }else printaChar(20, '\n');
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
        printaNonograma(mat, tamMat, espacamentoMax, xCabecalho, yCabecalho);
        
        printf("\nDigite um comando: ");
        
        fflush(stdin);
        scanf("%s", input);
        fflush(stdin);

        recado = 0;
        switch (interpretaInput(input))
        {
        case 'v':
            fflush(stdin);
            scanf("%s", cordenadas);
            fflush(stdin);

            printf("tam input: %d\n", strlen(cordenadas));

            if (strlen(cordenadas) > 2)
            {
                recado = 1;
            }            

            int y = letra2int(cordenadas[0]);
            int x = letra2int(cordenadas[1]);
            char buffer;

            if (recado == 0)
            {
                recado = alteraNonograma(mat, tamMat, (coord){ x, y }, input[0], &buffer);
            }
        
            //*Checa se a jogada inflige regra
            if (recado == 0)//Se nao tiver erros
            {
                recado = checaJogada(mat, tamMat, xCabecalho, yCabecalho, (coord){ x, y }, input[0]);

                if (recado == 4)//Se a jogada tiver infligido regra
                {
                    //desfaz a alteracao
                    alteraNonograma(mat, tamMat, (coord){ x, y }, buffer, &buffer);
                }
            }

            //*checa se o nonograma foi resolvido
            if (recado == 0)//Se nao tiver erros
            {
                if (checaVitoria(mat, tamMat, xCabecalho, yCabecalho))
                {
                    beep(); //:)
                    printf("Parabens, voce resolveu o nonograma!\n");
                    printf("\nDeseja jogar continuar? (s/n): ");
                    char continuarChar;
                    scanf("%c", &continuarChar);
                    if (continuarChar == 'n' || continuarChar == 'N') continuar = 0;
                }
            }

            break;

        case 'r':
            printf("Resolver\n");
            break;

        case 'q':
            continuar = 0;
            break;

        case 's':
            scanf("%s", pathCaminho);
            salvaArquivo(pathCaminho, mat, tamMat, xCabecalho, yCabecalho);
            printf("Nonograma salvo com sucesso!\n");
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
    for (int i = 0; i < tamMat.y; i++)
    {
        free(mat[i]);
    }
    free(mat);

    for (int i = 0; i < tamMat.y; i++)
    {
        free(xCabecalho[i]);
    }
    free(xCabecalho);

    for (int i = 0; i < tamMat.x; i++)
    {
        free(yCabecalho[i]);
    }
    free(yCabecalho);

    fclose(arqNonograma);
    return 0;
}