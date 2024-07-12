/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/definicoesTipos.h"
#include "../bibliotecas/funcoes_fornecidas.h"
#include "../bibliotecas/funcoesAuxiliares.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Função para imprimir registros buscados
void imprimeRegistrosBuscados(char *arquivo, int buscaId, char *nomeArquivoArvoreB) {
    int numOperacoes;
    scanf("%d", &numOperacoes); // Lê o número de buscas a serem feitas

    for (int i = 0; i < numOperacoes; i++) {
        FILE *file = fopen(arquivo, "rb"); // Abre o arquivo binário no modo leitura
        if (file == NULL) {
            printf("Falha no processamento do arquivo."); // Verifica se ocorreu um erro ao abrir o arquivo
            return;
        }

        CABECALHO *cabecalho = retornaCabecalhoBinario(file); // Cria um cabeçalho e atribui os valores a ele

        imprimirRegistrosPorCampos(file, cabecalho, buscaId, nomeArquivoArvoreB, i); // Chama a função para imprimir registros
        
        fclose(file); // Fecha o arquivo
        limpaCabecalho(cabecalho); // Libera a memória do cabeçalho
    }
}