#ifndef FUNCOES_H
#define FUNCOES_H

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

#endif