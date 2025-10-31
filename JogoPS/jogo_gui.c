#define UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

char palavra[50], dica[100], mascara[50];
int tentativas = 6, acertos = 0, erros = 0;
int tempoRestante = 30; 

HFONT fonteTitulo, fonteDica, fontePalavra, fonteStatus;

void carregarPalavraGUI(void) {
    FILE* f = fopen("palavras.txt", "r");
    if (!f) {
        MessageBox(NULL, L"Erro ao abrir o arquivo de palavras!", L"Erro", MB_ICONERROR);
        exit(1);
    }

    char linha[100];
    int total = 0;

    while (fgets(linha, sizeof(linha), f))
        total++;

    if (total == 0) {
        MessageBox(NULL, L"O arquivo de palavras está vazio!", L"Erro", MB_ICONERROR);
        fclose(f);
        exit(1);
    }

    rewind(f);
    srand((unsigned int)time(NULL));
    int escolha = rand() % total;

    for (int i = 0; i <= escolha; i++)
        fgets(linha, sizeof(linha), f);

    fclose(f);

    linha[strcspn(linha, "\r\n")] = 0; 

    char* token = strtok(linha, ";");
    if (token)
        strcpy(palavra, token);
    token = strtok(NULL, ";");
    if (token)
        strcpy(dica, token);
    else
        strcpy(dica, "Sem dica disponível");

    int tam = strlen(palavra);
    for (int i = 0; i < tam; i++)
        mascara[i] = '_';
    mascara[tam] = '\0';
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"PalavraSecretaGUI";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Jogo da Palavra Secreta",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 420,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd)
        return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    carregarPalavraGUI();

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hDica, hPalavra, hInput, hButton, hStatus, hTempo;
    static HBRUSH fundoBranco, fundoAzul;

    switch (uMsg) {
    case WM_CREATE: {
        fonteTitulo = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");
        fonteDica = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");
        fontePalavra = CreateFont(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Consolas");
        fonteStatus = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");

        fundoBranco = CreateSolidBrush(RGB(255, 255, 255));
        fundoAzul = CreateSolidBrush(RGB(220, 235, 255));

        hTempo = CreateWindow(L"static", L"Tempo: 30s", WS_VISIBLE | WS_CHILD | SS_CENTER,
            420, 10, 140, 30, hwnd, NULL, NULL, NULL);
        SendMessage(hTempo, WM_SETFONT, (WPARAM)fonteTitulo, TRUE);

        CreateWindow(L"static", L"Dica:", WS_VISIBLE | WS_CHILD, 20, 60, 70, 20, hwnd, NULL, NULL, NULL);
        hDica = CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD | SS_LEFT, 90, 60, 480, 30, hwnd, NULL, NULL, NULL);
        SendMessage(hDica, WM_SETFONT, (WPARAM)fonteDica, TRUE);

        CreateWindow(L"static", L"Palavra:", WS_VISIBLE | WS_CHILD, 20, 110, 80, 20, hwnd, NULL, NULL, NULL);
        hPalavra = CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD | SS_CENTER, 110, 100, 400, 40, hwnd, NULL, NULL, NULL);
        SendMessage(hPalavra, WM_SETFONT, (WPARAM)fontePalavra, TRUE);

        CreateWindow(L"static", L"Digite uma letra:", WS_VISIBLE | WS_CHILD, 20, 170, 130, 20, hwnd, NULL, NULL, NULL);
        hInput = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
            160, 170, 40, 25, hwnd, NULL, NULL, NULL);
        hButton = CreateWindow(L"button", L"Tentar", WS_VISIBLE | WS_CHILD,
            220, 170, 90, 28, hwnd, (HMENU)1, NULL, NULL);
        SendMessage(hButton, WM_SETFONT, (WPARAM)fonteStatus, TRUE);

        hStatus = CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD | SS_CENTER,
            50, 220, 500, 25, hwnd, NULL, NULL, NULL);
        SendMessage(hStatus, WM_SETFONT, (WPARAM)fonteStatus, TRUE);

        wchar_t dicaW[100], mascaraW[50], status[100];
        swprintf(dicaW, 100, L"%hs", dica);
        swprintf(mascaraW, 50, L"%hs", mascara);
        swprintf(status, 100, L"Tentativas: %d | Acertos: %d | Erros: %d", tentativas, acertos, erros);
        SetWindowText(hDica, dicaW);
        SetWindowText(hPalavra, mascaraW);
        SetWindowText(hStatus, status);

        SetTimer(hwnd, 1, 1000, NULL);
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            wchar_t letraW[2] = L"";
            GetWindowText(hInput, letraW, 2);
            SetWindowText(hInput, L"");
            char letra = (char)letraW[0];

            int novo = 0;
            for (int i = 0; palavra[i]; i++) {
                if (palavra[i] == letra && mascara[i] == '_') {
                    mascara[i] = letra;
                    novo++;
                }
            }

            if (novo == 0) { erros++; tentativas--; }
            else { acertos += novo; }

            wchar_t mascaraW[50], status[100];
            swprintf(mascaraW, 50, L"%hs", mascara);
            SetWindowText(hPalavra, mascaraW);
            swprintf(status, 100, L"Tentativas: %d | Acertos: %d | Erros: %d", tentativas, acertos, erros);
            SetWindowText(hStatus, status);

            if (strcmp(palavra, mascara) == 0) {
                MessageBox(hwnd, L"Parabéns! Você venceu!", L"Vitória", MB_OK | MB_ICONINFORMATION);
                carregarPalavraGUI();
                tentativas = 6; acertos = erros = 0; tempoRestante = 30;
            }
            else if (tentativas <= 0) {
                wchar_t msg[100];
                swprintf(msg, 100, L"Você perdeu! A palavra era: %hs", palavra);
                MessageBox(hwnd, msg, L"Derrota", MB_OK | MB_ICONERROR);
                carregarPalavraGUI();
                tentativas = 6; acertos = erros = 0; tempoRestante = 30;
            }

            wchar_t dicaW[100];
            swprintf(dicaW, 100, L"%hs", dica);
            swprintf(mascaraW, 50, L"%hs", mascara);
            SetWindowText(hDica, dicaW);
            SetWindowText(hPalavra, mascaraW);
        }
        break;

    case WM_TIMER: {
        tempoRestante--;
        wchar_t tempoTxt[30];
        swprintf(tempoTxt, 30, L"Tempo: %ds", tempoRestante);
        SetWindowText(hTempo, tempoTxt);

        if (tempoRestante <= 0) {
            wchar_t msg[100];
            swprintf(msg, 100, L"Tempo esgotado! A palavra era: %hs", palavra);
            MessageBox(hwnd, msg, L"Fim de Tempo", MB_OK | MB_ICONWARNING);
            carregarPalavraGUI();
            tentativas = 6; acertos = erros = 0; tempoRestante = 30;
        }
        break;
    }

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        DeleteObject(fonteTitulo);
        DeleteObject(fonteDica);
        DeleteObject(fontePalavra);
        DeleteObject(fonteStatus);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

