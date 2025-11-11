#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>
#include <strings.h>
#include "funcoes.h"

#define EMB_TOTAL 300
#define MAX_CHAR 3000

void processarLinha(FILE *orig_file, FILE *dest, Navio *navios, int *total_navios, int *n_portugueses, const char *tipoNavio, int *numNavios)
{
    char linha[MAX_CHAR];
    fgets(linha, MAX_CHAR, orig_file); /* Ignora a primeira linha (cabeçalho) */

    while (fgets(linha, MAX_CHAR, orig_file))
    {
        char *token;
        int colNum = 0;
        Navio navio;

        token = strtok(linha, ";");
        while (token != NULL)
        {
            colNum++;
            retirarnewline(token);

            switch (colNum)
            {
            case 3:
                strncpy(navio.col3, token, MAX_CHAR);
                break;
            case 7:
                strncpy(navio.col7, token, MAX_CHAR);
                break;
            case 8:
                strncpy(navio.col8, token, MAX_CHAR);
                break;
            case 20:
                strncpy(navio.col20, token, MAX_CHAR);
                break;
            case 29:
                strncpy(navio.col30, token, MAX_CHAR);
                break;
            case 30:
                strncpy(navio.col31, token, MAX_CHAR);
                break;
            case 31:
                strncpy(navio.col32, token, MAX_CHAR);
                break;
            }

            token = strtok(NULL, ";");
        }

        if (strstr(navio.col7, "Portugal") != NULL)
        {
            (*n_portugueses)++;
        }

        if (strcasecmp(navio.col8, tipoNavio) == 0 || strstr(navio.col8, "rebocador") != NULL ||
            strstr(navio.col8, tipoNavio) != NULL || strstr(navio.col8, "Cruzeiro") != NULL ||
            strcasecmp(navio.col8, "Cruzeiros") == 0 || strstr(navio.col8, "Passageiros") != NULL ||
            strstr(navio.col8, "Contentor") != NULL || strstr(navio.col8, "Graneleiro") != NULL ||
            strstr(navio.col8, "Batelão") != NULL || strstr(navio.col8, "BATELÃO") != NULL ||
            strstr(navio.col8, "Petroleiro") != NULL || strstr(navio.col8, "Tanque") != NULL)
        {
            escreverNoArquivo(dest, &navio);
            (*total_navios)++;
            navios[*numNavios] = navio;
            (*numNavios)++;
        }
    }
}

int compararNavios(const void *a, const void *b)
{
    return strcasecmp(((Navio *)a)->col8, ((Navio *)b)->col8);
}

void ordenarNavios(Navio *navios, int numNavios)
{
    qsort(navios, numNavios, sizeof(Navio), compararNavios);
}

void escreverNoArquivo(FILE *dest, Navio *navio)
{
    fprintf(dest, "%-15s;%-38s;%-14s;%-10s;%-28s\n", navio->col3, navio->col8, navio->col7, navio->col20, navio->col32);
}

void inserirPassageiros(embarcacoes embarcacao[], int *contador)
{
    FILE *file;
    printf("Novo navio de passageiros\n");
    printf("Nome do navio: ");
    fgets(embarcacao[*contador].nome_navio, 50, stdin);

    file = fopen("navios_por_categoria.csv", "a");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo %s.\n", "navios_por_categoria.csv");
        exit(1);
    }

    /* Posiciona o ponteiro de arquivo no final */
    fseek(file, 0, SEEK_END);

    /* Verifica se o arquivo est� vazio e escreve o cabe?alho */
    if (ftell(file) == 0)
    {
        fprintf(file, "Navio;Bandeira;Tipo Navio;Indicativo Chamada;Comprimento;Calado Maximo;Porto Registo;Armador;Boca Sinal;Velocidade Maxima;Data Construcao\n");
    }

    if (*contador == 0 || strcmp(embarcacao[*contador - 1].nome_navio, embarcacao[*contador].nome_navio) != 0)
    {
        printf("Bandeira: ");
        fgets(embarcacao[*contador].bandeira, 50, stdin);

        printf("Tipo do navio: ");
        fgets(embarcacao[*contador].tipo_navio, 50, stdin);

        printf("Indicativo de chamada\n "
               "Matricula do Navio"
               ": ");
        fgets(embarcacao[*contador].indicativo_chamada, 50, stdin);

        printf("Comprimento do navio: ");
        scanf("%f", &embarcacao[*contador].comprimento);
        getchar();
        while (getchar() != '\n')
            ;
        printf("Calado máximo: ");
        scanf("%f", &embarcacao[*contador].calado_maximo);
        getchar();
        while (getchar() != '\n')
            ;
        printf("Porto de registo: ");
        fgets(embarcacao[*contador].porto_registo, 50, stdin);
        while (getchar() != '\n')
            ;
        printf("Armador: ");
        fgets(embarcacao[*contador].armador, 50, stdin);

        printf("Boca sinal: ");
        scanf("%f", &embarcacao[*contador].boca_sinal);
        getchar();
        while (getchar() != '\n')
            ;
        printf("Velocidade máxima: ");
        scanf("%f", &embarcacao[*contador].v_maxima);
        getchar();
        if (embarcacao[*contador].v_maxima != 0)
        {
            printf("Data de construção:\t ");
            fgets(embarcacao[*contador].data_construcao, 50, stdin);
            while (getchar() != '\n')
                ;
        }
        else
        {
            printf("Como velocidade máxima menor que 0, não é possivel guardar os dados do navio, tente novamente.\n");
            return;
        }
    }
    else
    {
        printf("Nome do navio já foi introduzido anteriormente...\n");
        return;
    }
    fprintf(file, "%s;%s;%s;%s;%.2f;%.2f;%s;%s;%.2f;%.2f;%s\n",
            embarcacao[*contador].nome_navio, embarcacao[*contador].bandeira,
            embarcacao[*contador].tipo_navio, embarcacao[*contador].indicativo_chamada,
            embarcacao[*contador].comprimento, embarcacao[*contador].calado_maximo,
            embarcacao[*contador].porto_registo, embarcacao[*contador].armador,
            embarcacao[*contador].boca_sinal, embarcacao[*contador].v_maxima,
            embarcacao[*contador].data_construcao);
    fclose(file);
}

void retirarnewline(char *str)
{
    size_t length = strlen(str);
    if (length > 0 && str[length - 1] == '\n')
    {
        str[length - 1] = '\0';
    }
}

void SeletorRebocador(const char *nomeArquivo, const char *tipoNavio)
{

    int n_portugueses = 0;
    int total_navios = 0;
    FILE *file = fopen(nomeArquivo, "r");
    char linha[MAX_CHAR];
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    fgets(linha, MAX_CHAR, file); /* Ignora a primeira linha (cabe?alho) */

    while (fgets(linha, MAX_CHAR, file))
    {
        char *token;
        int colNum = 0;
        char col3[MAX_CHAR] = "N/A", col7[MAX_CHAR] = "N/A", col8[MAX_CHAR] = "N/A", col20[MAX_CHAR] = "N/A", col30[MAX_CHAR] = "N/A", col29[MAX_CHAR] = "N/A", col31[MAX_CHAR] = "N/A";

        token = strtok(linha, ";");
        while (token != NULL)
        {
            colNum++;
            retirarnewline(token); /* Limpeza de newlines no final do token */

            /* Captura dos valores para as respectivas colunas. */
            if (colNum == 3)
                strncpy(col3, token, MAX_CHAR);
            else if (colNum == 7)
                strncpy(col7, token, MAX_CHAR);
            else if (colNum == 8)
                strncpy(col8, token, MAX_CHAR);
            else if (colNum == 20)
                strncpy(col20, token, MAX_CHAR);
            else if (colNum == 29)
                strncpy(col29, token, MAX_CHAR);
            else if (colNum == 30)
                strncpy(col30, token, MAX_CHAR);
            else if (colNum == 31)
                strncpy(col31, token, MAX_CHAR);

            token = strtok(NULL, ";");
        }

        /* Verifica��o e impress�o para depura�o. */
        if (strcasecmp(col8, tipoNavio) == 0 || strstr(col8, "rebocador") != NULL)
        {
            if (colNum == 30)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col29);
            }
            else if (colNum == 31)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col30);
            }
            else if (colNum == 32)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col31);
            }

            if (strcasecmp(col7, "Portugal") == 0)
            {
                n_portugueses++;
            }
            total_navios++;
        }
    }

    fclose(file);
    printf("Total de embarcações: %d\t\tExistem %d embarcações portuguesas\n", total_navios, n_portugueses);
}

void SelecaoPassageiro(const char *nomeArquivo, const char *tipoNavio)
{

    char linha[MAX_CHAR];
    int n_portugueses = 0;
    int total_navios = 0;
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    fgets(linha, MAX_CHAR, file); /* Ignora a primeira linha (cabe?alho) */

    while (fgets(linha, MAX_CHAR, file))
    {
        char *token;
        int colNum = 0;
        char col3[MAX_CHAR] = "N/A", col7[MAX_CHAR] = "N/A", col8[MAX_CHAR] = "N/A", col20[MAX_CHAR] = "N/A", col30[MAX_CHAR] = "N/A", col29[MAX_CHAR] = "N/A", col31[MAX_CHAR] = "N/A";

        token = strtok(linha, ";");
        while (token != NULL)
        {
            colNum++;
            retirarnewline(token); /* Limpeza de newlines no final do token */

            /* Captura dos valores para as respectivas colunas. */
            if (colNum == 3)
                strncpy(col3, token, MAX_CHAR);
            else if (colNum == 7)
                strncpy(col7, token, MAX_CHAR);
            else if (colNum == 8)
                strncpy(col8, token, MAX_CHAR);
            else if (colNum == 20)
                strncpy(col20, token, MAX_CHAR);
            else if (colNum == 29)
                strncpy(col29, token, MAX_CHAR);
            else if (colNum == 30)
                strncpy(col30, token, MAX_CHAR);
            else if (colNum == 31)
                strncpy(col31, token, MAX_CHAR);

            token = strtok(NULL, ";");
        }

        /* Verifica??o e impress?o para depura??o. */
        if (strstr(col8, tipoNavio) != NULL || strstr(col8, "Cruzeiro") != NULL || strcasecmp(col8, "Cruzeiros") == 0 || strstr(col8, "Passageiros") != NULL)
        {
            if (colNum == 30)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col29);
            }
            else if (colNum == 31)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col30);
            }
            else if (colNum == 32)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col31);
            }

            if (strcasecmp(col7, "Portugal") == 0)
            {
                n_portugueses++;
            }
            total_navios++;
        }
    }

    fclose(file);
    printf("Total de embarcações: %d\t\tExistem %d embarcações portuguesas\n", total_navios, n_portugueses);
}

void SeletorCargueiro(const char *nomeArquivo)
{

    int n_portugueses = 0;
    int total_navios = 0;
    FILE *file = fopen(nomeArquivo, "r");
    char linha[MAX_CHAR];
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    fgets(linha, MAX_CHAR, file); /* Ignora a primeira linha (cabe�alho) */

    while (fgets(linha, MAX_CHAR, file))
    {
        char *token;
        int colNum = 0;
        char col3[MAX_CHAR] = "N/A", col7[MAX_CHAR] = "N/A", col8[MAX_CHAR] = "N/A", col20[MAX_CHAR] = "N/A", col30[MAX_CHAR] = "N/A", col29[MAX_CHAR] = "N/A", col31[MAX_CHAR] = "N/A";

        token = strtok(linha, ";");
        while (token != NULL)
        {
            colNum++;
            retirarnewline(token); /* Limpeza de newlines no final do token */

            /* Captura dos valores para as respectivas colunas. */
            if (colNum == 3)
                strncpy(col3, token, MAX_CHAR);
            else if (colNum == 7)
                strncpy(col7, token, MAX_CHAR);
            else if (colNum == 8)
                strncpy(col8, token, MAX_CHAR);
            else if (colNum == 20)
                strncpy(col20, token, MAX_CHAR);
            else if (colNum == 29)
                strncpy(col29, token, MAX_CHAR);
            else if (colNum == 30)
                strncpy(col30, token, MAX_CHAR);
            else if (colNum == 31)
                strncpy(col31, token, MAX_CHAR);

            token = strtok(NULL, ";");
        }

        if (strstr(col8, "Contentor") != NULL || strstr(col8, "Graneleiro") != NULL || strstr(col8, "Batel�o") != NULL || strstr(col8, "BATEL�O") != NULL)
        {
            if (colNum == 30)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col29);
            }
            else if (colNum == 31)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col30);
            }
            else if (colNum == 32)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col31);
            }

            if (strcasecmp(col7, "Portugal") == 0)
            {
                n_portugueses++;
            }
            total_navios++;
        }
    }

    fclose(file);
    printf("Total de embarcações: %d\t\tExistem %d embarcações portuguesas\n", total_navios, n_portugueses);
}

void SeletorTanque(const char *nomeArquivo)
{

    int n_portugueses = 0;
    int total_navios = 0;
    FILE *file = fopen(nomeArquivo, "r");
    char linha[MAX_CHAR];
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    fgets(linha, MAX_CHAR, file); /* Ignora a primeira linha (cabe�alho) */

    while (fgets(linha, MAX_CHAR, file))
    {
        char *token;
        int colNum = 0;
        char col3[MAX_CHAR] = "N/A", col7[MAX_CHAR] = "N/A", col8[MAX_CHAR] = "N/A", col20[MAX_CHAR] = "N/A", col30[MAX_CHAR] = "N/A", col29[MAX_CHAR] = "N/A", col31[MAX_CHAR] = "N/A";

        token = strtok(linha, ";");
        while (token != NULL)
        {
            colNum++;
            retirarnewline(token); /* Limpeza de newlines no final do token */

            /* Captura dos valores para as respectivas colunas. */
            if (colNum == 3)
                strncpy(col3, token, MAX_CHAR);
            else if (colNum == 7)
                strncpy(col7, token, MAX_CHAR);
            else if (colNum == 8)
                strncpy(col8, token, MAX_CHAR);
            else if (colNum == 20)
                strncpy(col20, token, MAX_CHAR);
            else if (colNum == 29)
                strncpy(col29, token, MAX_CHAR);
            else if (colNum == 30)
                strncpy(col30, token, MAX_CHAR);
            else if (colNum == 31)
                strncpy(col31, token, MAX_CHAR);

            token = strtok(NULL, ";");
        }

        /* Verifica��o e impress�o para depura��o. */
        if (strstr(col8, "Petroleiro") != NULL || strstr(col8, "Tanque") != NULL)
        {
            if (colNum == 30)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col29);
            }
            else if (colNum == 31)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col30);
            }
            else if (colNum == 32)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col31);
            }

            if (strcasecmp(col7, "Portugal") == 0)
            {
                n_portugueses++;
            }
            total_navios++;
        }
    }

    fclose(file);
    printf("Total de embarcações: %d\t\tExistem %d embarcações portuguesas\n", total_navios, n_portugueses);
}

void Resto(const char *nomeArquivo)
{

    int n_portugueses = 0;
    int total_navios = 0;
    FILE *file = fopen(nomeArquivo, "r");
    char linha[MAX_CHAR];
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    fgets(linha, MAX_CHAR, file); /* Ignora a primeira linha (cabe?alho) */

    while (fgets(linha, MAX_CHAR, file))
    {
        char *token;
        int colNum = 0;
        char col3[MAX_CHAR] = "N/A", col7[MAX_CHAR] = "N/A", col8[MAX_CHAR] = "N/A", col20[MAX_CHAR] = "N/A", col30[MAX_CHAR] = "N/A", col29[MAX_CHAR] = "N/A", col31[MAX_CHAR] = "N/A";

        token = strtok(linha, ";");
        while (token != NULL)
        {
            colNum++;
            retirarnewline(token); /* Limpeza de newlines no final do token */

            /* Captura dos valores para as respectivas colunas. */
            if (colNum == 3)
                strncpy(col3, token, MAX_CHAR);
            else if (colNum == 7)
                strncpy(col7, token, MAX_CHAR);
            else if (colNum == 8)
                strncpy(col8, token, MAX_CHAR);
            else if (colNum == 20)
                strncpy(col20, token, MAX_CHAR);
            else if (colNum == 29)
                strncpy(col29, token, MAX_CHAR);
            else if (colNum == 30)
                strncpy(col30, token, MAX_CHAR);
            else if (colNum == 31)
                strncpy(col31, token, MAX_CHAR);

            token = strtok(NULL, ";");
        }

        /* Verifica��o e impress�o para depura��o. */
        if (strstr(col8, "Contentores") == NULL && strstr(col8, "Graneleiro") == NULL && strstr(col8, "Batelel�o") == NULL && strstr(col8, "BATEL�O") == NULL && strstr(col8, "Petroleiro") == NULL && strstr(col8, "Tanque") == NULL && strstr(col8, "Passageiro") == NULL && strstr(col8, "Cruzeiro") == NULL && strcasecmp(col8, "Cruzeiros") != 0 && strcasecmp(col8, "Rebocador") != 0 && strstr(col8, "rebocador") == NULL)
        {
            if (colNum == 30)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col29);
            }
            else if (colNum == 31)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col30);
            }
            else if (colNum == 32)
            {
                printf("%-10s\t%-17s%-20s%-20s%-20s\n", col3, col8, col7, col20, col31);
            }

            if (strcasecmp(col7, "Portugal") == 0)
            {
                n_portugueses++;
            }
            total_navios++;
        }
    }

    fclose(file);
    printf("Total de embarcações: %d\t\tExistem %d embarcações portuguesas\n", total_navios, n_portugueses);
}

void linha(int num)
{
    int i;
    printf("|");
    for (i = 0; i <= num; i++)
    {
        printf("-");
    }
    printf("|");
}