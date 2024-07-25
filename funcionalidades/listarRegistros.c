/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/funcionalidades.h"
#include "../bibliotecas/definicoesTipos.h"
#include "../bibliotecas/funcoesAuxiliares.h"
#include "../bibliotecas/funcoes_fornecidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Função listar os registros 
void listarRegistros(const char* nomeArquivoBinario) {
    // abertura de arquivos e inicialização de variáveis

    FILE* arquivoBinario = fopen(nomeArquivoBinario, "rb");

    CABECALHO_DADOS cabecalho;

    // Leitura do cabeçalho
    LeituraCabecalho(&cabecalho, arquivoBinario);

    // Variável para armazenar o registro
    DADOS_FIXOS registro;

    // Abertura do arquivo binário
    if (arquivoBinario == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Se o arquivo estiver inconsistente
    if (cabecalho.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        
        fclose(arquivoBinario);
        return;
    }

    if (cabecalho.nroRegArq == 0)
        printf("Registro inexistente.\n\n");

    // Loop para ler e exibir cada registro
    int i;

    for (i = 0; i < cabecalho.nroRegArq + cabecalho.nroRegRem; i++) {

        // Leitura do registro campo a campo
        LeituraRegistroFixo(&registro, arquivoBinario);

        // Se o registro não estiver removido
        if (registro.removido == '0') {
            
            // Exibição dos dados do registro
            printf("Nome do Jogador: %s\n", registro.nomeJogador[0] ? registro.nomeJogador : "SEM DADO");
            printf("Nacionalidade do Jogador: %s\n", registro.nacionalidade[0] ? registro.nacionalidade : "SEM DADO");
            printf("Clube do Jogador: %s\n", registro.nomeClube[0] ? registro.nomeClube : "SEM DADO"); printf("\n");

        }

    }

    // Fechamento do arquivo
    fclose(arquivoBinario);
}