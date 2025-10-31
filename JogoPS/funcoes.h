#ifndef FUNCOES_H
#define FUNCOES_H

void carregarPalavra(char* palavra, char* dica);
void mostrarInterface(int tentativas, int acertos, int erros, const char* palavra, const char* dica);
int verificarLetra(char letra, const char* palavra, char* mascara);
void jogar();

#endif
