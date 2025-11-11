/*Trabalho realizdo por:
    -Daniel Lib�rio
    -Gon�alo Taborda
    -Henrique Domingos
    - Manuel Roque*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <strings.h>

#define EMB_TOTAL 300
#define MAX_CHAR 3000

typedef struct
{
    char nome_navio[50];
    char tipo_navio[20];
    char bandeira[20];
    char indicativo_chamada[20];
    float comprimento;
    float calado_maximo;
    char porto_registo[50];
    char armador[150];
    float boca_sinal;
    float v_maxima;
    char data_construcao[50];

} embarcacoes;

typedef struct
{
    char col3[MAX_CHAR];
    char col7[MAX_CHAR];
    char col8[MAX_CHAR];
    char col20[MAX_CHAR];
    char col32[MAX_CHAR];
    char col30[MAX_CHAR];
    char col31[MAX_CHAR];
} Navio;

void SeletorRebocador(const char *nomeArquivo, const char *tipoNavio);
void SelecaoPassageiro(const char *nomeArquivo, const char *tipoNavio);
void inserirPassageiros(embarcacoes embarcacao[], int *contador);
void SeletorCargueiro(const char *nomeArquivo);
void SeletorTanque(const char *nomeArquivo);
void Resto(const char *nomeArquivo);
void retirarnewline(char *str);
void linha(int num);
void processarLinha(FILE *orig_file, FILE *dest, Navio *navios, int *total_navios, int *n_portugueses, const char *tipoNavio, int *numNavios);
void ordenarNavios(Navio *navios, int numNavios);
void escreverNoArquivo(FILE *dest, Navio *navio);
int compararNavios(const void *a, const void *b);

int main()
{
    int verificador, i, contador_passageiros = 0;
    float opcao;
    int n_portugueses = 0;
    int total_navios = 0;
    int numNavios = 0;
    char buffer[100];
    char tipoNavio[MAX_CHAR];
    embarcacoes dados_embarcacao[EMB_TOTAL];
    Navio *navios;
    FILE *dest;
    FILE *orig_file;
    
    setlocale(LC_ALL, "Portuguese");
    /* Loop principal do programa */
    do
    {
        /* Exibição do menu */
        linha(54);
        printf("\n| (1) Criar ficheiro navios_por_categoria.csv \t\t|\n");
        printf("| (2) Mostrar relatorio das embarcacoes por categoria\t|\n");
        printf("| (3) Inserir novas embarcacoes de passageiros\t\t|\n");
        printf("| (4) Sair\t\t\t\t\t\t|\n");
        linha(54);
        /* Solicitação da opção do usuário */
        printf("\n| Opcao ->    ");
        /* Leitura da opção do usuário */
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Opção inválida...\n");
            return 1;
        }
        else
        {
            /* Verificação da entrada do usuário */
            if (sscanf(buffer, "%f", &opcao) != 1)
            {
                printf("Entrada inválida. Digite um número inteiro.\n");
                continue; /* Volta ao início do loop para pedir uma nova opção*/
            }

            if (opcao >= 1 && opcao <= 4)
            {
                /* Switch para lidar com as diferentes opções */
                switch ((int)opcao)
                {
                case 1:

                    /* Abertura do arquivo original para leitura */
                    orig_file = fopen("apl_navios_porto.csv", "r");
                    if (!orig_file)
                    {
                        perror("Erro ao abrir o ficheiro existente");
                        return EXIT_FAILURE;
                    }
                    /* Abertura do arquivo de destino para escrita */
                    dest = fopen("navios_por_categoria.csv", "w");
                        if (!dest) 
                        {
                        perror("Erro ao abrir o arquivo de destino");
                        return EXIT_FAILURE;
                        }
                    /* Alocação dinâmica de memória para o array de navios */
                    navios = malloc(sizeof(Navio) * 5000); /* Ajuste o tamanho conforme necessário */
                        if (navios == NULL) 
                        {
                        perror("Erro ao alocar memória para navios");
                        return EXIT_FAILURE;
                        }

                    /* Escrita do cabeçalho no arquivo de destino */
                    fprintf(dest, "%-10s;%-17s;%-16s;%-10s;%-20s\n", "Embarcação", "Tipo", "Bandeira", "Comprimento", "Data de Construção");

                    processarLinha(orig_file, dest, navios, &total_navios, &n_portugueses, "SeuTipoNavio", &numNavios);
                    ordenarNavios(navios, numNavios);

                    fclose(orig_file);

                    /*Escrever no arquivo ordenado */
                    for (i = 0; i < numNavios; i++)
                    {
                        escreverNoArquivo(dest, &navios[i]);
                    }

                    fclose(dest);

                    printf("\nTotal de embarcações: %d\n\n", total_navios);
                    /* Liberação da memória alocada para o array de navios */
                    free(navios);
                                        
                    break;

                case 2:
                    do
                    {

                        verificador = 0;
                        /* Solicitação do tipo de navio desejado para o relatório */
                        printf("Insira o tipo de navio desejado para relatório:\n\n"
                               " - Rebocador\t- Passageiro\t- Cargueiro\t- Tanque\t- Outros\n"
                               "(Escreva a opção exatamente como está na linha anterior)\n\n"
                               "Opção ->    ");
                        fgets(tipoNavio, MAX_CHAR, stdin);
                        retirarnewline(tipoNavio);

                        printf("\n%-10s\t\t%-30s%-20s%-20s%-20s\n\n", "Embarcação", "Tipo", "Bandeira", "Comprimento", "Data de Construção");

                        if (strcasecmp("Rebocador", tipoNavio) == 0)
                            SeletorRebocador("apl_navios_porto.csv", tipoNavio);

                        else if (strcasecmp("Passageiro", tipoNavio) == 0)
                            SelecaoPassageiro("apl_navios_porto.csv", tipoNavio);

                        else if (strcasecmp("Cargueiro", tipoNavio) == 0)
                            SeletorCargueiro("apl_navios_porto.csv");

                        else if (strcasecmp("Tanque", tipoNavio) == 0)
                            SeletorTanque("apl_navios_porto.csv");

                        else if (strcasecmp("Outros", tipoNavio) == 0)
                            Resto("apl_navios_porto.csv");

                        else
                        {
                            printf("Opção inválida. Tente novamente...\n\n");
                            verificador = 1;
                        }

                    } while (verificador == 1);

                    break;

                case 3:
                    /* Inserção de novas embarcações de passageiros */
                    inserirPassageiros(dados_embarcacao, &contador_passageiros);
                    contador_passageiros++;
                    break;
                
                case 4:
                    /* Mensagem de encerramento do programa */
                    printf("Adeusss...\n");
                    break;

                default:
                    printf("Esta opção não existe. Tente novamente... \n");
                }
            }
            else
            {
                printf("Opção inválida. Tente novamente...\n\n");
            }
        }
    } while (opcao != 4);

    return 0;
}

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