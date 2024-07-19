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
void recuperaRegistrosCorrespondentesBusca(char *binFileName, int buscaId, char *arvBFileName) {
    int qtd_buscas;
    scanf("%d", &qtd_buscas); // Quantidade de buscas

    int i;

    for (i = 0; i < qtd_buscas; i++) {

        // Inicialização de variáveis
        FILE *binFile = fopen(binFileName, "rb"); // modo leitura de binario
        CABECALHO_DADOS *header = lerCabecalhoDados(binFile); // Cria um cabeçalho e atribui os valores a ele
        
        if (binFile == NULL) {
            printf("Falha no processamento do arquivo.");
            return;
        }
         // Função que de fato imprime os registros
        imprimirRegistrosPorCampos(binFile, header, buscaId, arvBFileName, i);
        
        fclose(binFile); 

        free(header);
    }
}