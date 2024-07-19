/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/definicoesTipos.h"
#include "../bibliotecas/funcoes_fornecidas.h"
#include "../bibliotecas/funcoesAuxiliaresB.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Função que imprime os registros com base nos campos de busca
void recuperarRegistros(char *nomeArquivoBinario, int buscaId, char *nomeArquivoArvoreB) {
    int qtd_buscas;
    scanf("%d", &qtd_buscas); // Quantidade de buscas

    int i;

    for (i = 0; i < qtd_buscas; i++) {

        // Inicialização de variáveis
        FILE *fp = fopen(nomeArquivoBinario, "rb"); // modo leitura de binario
        CABECALHO_DADOS *header = lerCabecalhoDados(fp); // Cria um cabeçalho e atribui os valores a ele
        
        if (fp == NULL) {
            printf("Falha no processamento do arquivo.");
            return;
        }
         // Função que de fato imprime os registros
        imprimirRegistrosCampos(fp, header, buscaId, nomeArquivoArvoreB, i);
        
        fclose(fp); 

        free(header);
    }
}