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

// Função para imprimir um registro de ID específico na árvore B
void buscaIdArvore(int id, FILE *binFile, char *arvBFileName, int i, int eh_busca_minuscula) {
    // arquivo da árvoreB com modo leitura de binário
    FILE *arvBFile = fopen(arvBFileName, "rb"); 

    // Leitura cabeçalho ArvB
    CABECALHO_ARVORE_B *cabecalho_arvB = lerCabecalhoArvB(arvBFile); 

    // Faz as verificações de consistências nos ifs abaixo
    if(arvBFile == NULL) {
        printf("Falha no processamento do arquivo.\n");
        
        return;
    }
    if(cabecalho_arvB->status == '0') {
        // Libera a memória do cabeçalho da árvore B
        free(cabecalho_arvB);
        fclose(arvBFile);

        printf("Falha no processamento do arquivo.\n"); 

        return;
    }

    int rrn;

    // if e else atualizam o rrn para o rrn da raíz da árvore B
    if(cabecalho_arvB == NULL) rrn = -1;
    else rrn = cabecalho_arvB->noRaiz;

    // Libera a memória alocada para o cabeçalho da árvoreB
    free(cabecalho_arvB);

    printf(eh_busca_minuscula ? "Busca %d\n\n" : "BUSCA %d\n\n", i + 1);

    if(rrn != -1) { // ArvoreB não está vazia
        // Encontra a posição do registro
        int64_t posicao_registro = buscarRegistroIdRec(arvBFile, id, rrn);

        // Se o registro for encontrado
        if(posicao_registro != -1) {
            // Busca o registro no arquivo binário
            DADOS *registro = leitura_registro_arquivoBin(posicao_registro, binFile);
            
            imprimeRegistro(registro);
            free_registro(registro);
        } else
            printf("Registro inexistente.\n\n");
    } else
        printf("Registro inexistente.\n\n");

    fclose(arvBFile);
}