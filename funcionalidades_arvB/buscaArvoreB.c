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
void buscaIdArvore(int id, FILE *file, char *nomeArquivoArvoreB, int i, int buscaMinuscula) {
    FILE *fp = fopen(nomeArquivoArvoreB, "rb"); // Abre o arquivo da árvore B no modo leitura

    if(fp == NULL) {
        printf("Falha no processamento do arquivo.\n"); // Verifica se ocorreu um erro ao abrir o arquivo
        return;
    }

    CABECALHO_ARVORE_B *cabecalhoArvoreB = lerCabecalhoArvB(fp); // Lê o cabeçalho da árvore B

    if(cabecalhoArvoreB == NULL){
        cabecalhoArvoreB->status = '$';
    }
    
    if(cabecalhoArvoreB->status == '0') {
        printf("Falha no processamento do arquivo.\n"); // Verifica se o status do cabeçalho é inválido
        free(cabecalhoArvoreB);
        fclose(fp); // Fecha o arquivo
        return;
    }

    int rrnAtual;

    if(cabecalhoArvoreB == NULL) {
        rrnAtual = -1;
    } else {
        rrnAtual = cabecalhoArvoreB->noRaiz;
    }

    free(cabecalhoArvoreB);


    printf(buscaMinuscula ? "Busca %d\n\n" : "BUSCA %d\n\n", i + 1); // Imprime o número da busca

    if(rrnAtual != -1) {
        int64_t byteOffsetRegistroBuscado = buscarRegistroIdRec(fp, id, rrnAtual); // Busca o byte offset do registro na árvore B

        if(byteOffsetRegistroBuscado != -1) { // Se o registro foi encontrado
            DADOS *registro = leitura_registro_arquivoBin(byteOffsetRegistroBuscado, file); // Busca o registro no arquivo binário
            imprimeRegistro(registro); // Imprime o registro
            free_registro(registro); // Libera a memória do registro
        } else {
            printf("Registro inexistente.\n\n"); // Se o registro não foi encontrado
        }
    } else {
        printf("Registro inexistente.\n\n"); // Se a árvore B está vazia
    }

    fclose(fp); // Fecha o arquivo da árvore B
}