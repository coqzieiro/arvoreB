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

// Função para criar um arquivo de árvore B a partir de um arquivo binário
int criarArquivoArvoreB(char *nomeArquivoBinario, char *nomeArquivoIndex) {
    FILE *arquivoBinario = fopen(nomeArquivoBinario, "rb"); // Abre o arquivo binário no modo leitura
    FILE *arquivoArvoreB = fopen(nomeArquivoIndex, "wb+"); // Abre o arquivo de índices no modo leitura e escrita

    // Verifica a consistencia do arquivo binário e de index
    if(!arquivoBinario || !arquivoArvoreB){
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBinario);
        fclose(arquivoArvoreB);
        return(0);
    }

    // Inicializa o cabeçalho
    CABECALHO *cabecalho = criarCabecalho();

    // Vai para o começo do cabeçalho
    fseek(arquivoBinario, 0, SEEK_SET);

    // Lê o cabeçalho
    lerCabecalhoB(arquivoBinario, cabecalho);

    //CABECALHO *cabecalho = retornaCabecalhoBinario(arquivoBinario); // Lê o cabeçalho do arquivo binário

    if(cabecalho->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBinario);
        fclose(arquivoArvoreB);
        free(cabecalho);
        return(0);
    }

    CABECALHO_ARVORE_B *cabecalhoArvoreB = criarCabecalhoArvoreBVazio(); // Cria um cabeçalho vazio para a árvore B

    setStatusCabecalhoArvoreB(cabecalhoArvoreB, '0');
    
    escreverCabecalhoArvoreB(arquivoArvoreB, cabecalhoArvoreB); // Escreve o cabeçalho no arquivo de índices

    int64_t posicao = 25; // Posição inicial do primeiro registro

    int quantidade = cabecalho->nroRegArq + cabecalho->nroRegRem; // Quantidade total de registros

    free(cabecalho);

    for(int i = 0; i < quantidade; i++) {
        DADOS *registro = lerRegistroFromBin(posicao, arquivoBinario); // Lê um registro do arquivo binário

        if(retornaRemovido(registro) == '1') { // Pula o registro removido
            posicao += retornaTamanhoRegistro(registro);
            continue;
        }

        int chave = retornaId(registro); // Obtém a chave do registro
        int64_t byteOffset = posicao; // Obtém o byteOffset do registro
        
        inserirArvoreB(arquivoArvoreB, chave, byteOffset); // Insere o registro na árvore B

        posicao += retornaTamanhoRegistro(registro); // Atualiza a posição para o próximo registro
        liberarRegistro(registro); // Libera a memória do registro
    }

    limpaCabecalhoArvoreB(cabecalhoArvoreB); // Libera a memória do cabeçalho da árvore B

    cabecalhoArvoreB = lerCabecalhoArvoreB(arquivoArvoreB); // Lê o cabeçalho atualizado da árvore B

    setStatusCabecalhoArvoreB(cabecalhoArvoreB, '1'); // Define o status do cabeçalho como consistente

    escreverCabecalhoArvoreB(arquivoArvoreB, cabecalhoArvoreB); // Escreve o cabeçalho no arquivo de índices

    limpaCabecalhoArvoreB(cabecalhoArvoreB); // Libera a memória do cabeçalho da árvore B

    fclose(arquivoBinario); // Fecha o arquivo binário
    fclose(arquivoArvoreB); // Fecha o arquivo de índices

    return(1);
}