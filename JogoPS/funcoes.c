#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

void carregarPalavra(char* palavra, char* dica) {
    FILE* f = fopen("palavras.txt", "r");
    if (!f) {
        printf("Erro ao abrir o arquivo de palavras!\n");
        exit(1);
    }

    char linha[100] = "";
    int total = 0;
    while (fgets(linha, sizeof(linha), f))
        total++;

    rewind(f);

    srand((unsigned int)time(NULL));
    int escolha = rand() % total;

    for (int i = 0; i <= escolha; i++) {
        fgets(linha, sizeof(linha), f);
    }

    char* token = strtok(linha, ";");
    if (token)
        strcpy(palavra, token);
    token = strtok(NULL, ";");
    if (token)
        strcpy(dica, token);

    fclose(f);
}

void mostrarInterface(int tentativas, int acertos, int erros, const char* palavra, const char* dica) {
    system("cls");
    printf("===== JOGO PALAVRA SECRETA =====\n");
    printf("Dica: %s\n", dica);
    printf("Palavra: %s\n", palavra);
    printf("Tentativas restantes: %d\n", tentativas);
    printf("Acertos: %d | Erros: %d\n", acertos, erros);
    printf("================================\n");
}

int verificarLetra(char letra, const char* palavra, char* mascara) {
    int acertos = 0;
    for (int i = 0; palavra[i] != '\0'; i++) {
        if (palavra[i] == letra && mascara[i] == '_') {
            mascara[i] = letra;
            acertos++;
        }
    }
    return acertos;
}

void jogar() {
    char palavra[50], dica[100], mascara[50];
    carregarPalavra(palavra, dica);

    int tam = strlen(palavra);
    for (int i = 0; i < tam; i++)
        mascara[i] = '_';
    mascara[tam] = '\0';

    int tentativas = 6, acertos = 0, erros = 0;

    while (tentativas > 0) {
        mostrarInterface(tentativas, acertos, erros, mascara, dica);
        printf("Digite uma letra: ");
        char letra;
        scanf(" %c", &letra);

        int novo = verificarLetra(letra, palavra, mascara);
        if (novo > 0)
            acertos += novo;
        else {
            erros++;
            tentativas--;
        }

        if (strcmp(palavra, mascara) == 0) {
            mostrarInterface(tentativas, acertos, erros, mascara, dica);
            printf("\nParabéns! Você acertou a palavra!\n");
            return;
        }
    }

    printf("\nVocê perdeu! A palavra era: %s\n", palavra);
}
