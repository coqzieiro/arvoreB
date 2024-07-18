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
    CABECALHO_DADOS *cabecalho = criarCabecalhoDados();

    // Vai para o começo do cabeçalho
    fseek(arquivoBinario, 0, SEEK_SET);

    // Lê o cabeçalho
    cabecalho = lerCabecalhoDados(arquivoBinario);

    if(cabecalho->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBinario);
        fclose(arquivoArvoreB);
        free(cabecalho);
        return(0);
    }

    CABECALHO_ARVORE_B *cabecalhoArvoreB = criarCabecalhoArvB(); // Cria um cabeçalho vazio para a árvore B

    cabecalhoArvoreB->status = '0';
    
    escreverCabecalhoArvB(arquivoArvoreB, cabecalhoArvoreB); // Escreve o cabeçalho no arquivo de índices

    int64_t posicao = 25; // Posição inicial do primeiro registro

    int quantidade = cabecalho->nroRegArq + cabecalho->nroRegRem; // Quantidade total de registros

    free(cabecalho);

    for(int i = 0; i < quantidade; i++) {
        DADOS *registro = leitura_registro_arquivoBin(posicao, arquivoBinario); // Lê um registro do arquivo binário

        if(registro->removido == '1') { // Pula o registro removido
            posicao += registro->tamanhoRegistro;
            continue;
        }

        int chave =  registro->id; // Obtém a chave do registro
        int64_t byteOffset = posicao; // Obtém o byteOffset do registro
        
        inserirArvoreB(arquivoArvoreB, chave, byteOffset); // Insere o registro na árvore B

        posicao += registro->tamanhoRegistro; // Atualiza a posição para o próximo registro
        free_registro(registro); // Libera a memória do registro
    }

    free(cabecalhoArvoreB);

    cabecalhoArvoreB = lerCabecalhoArvB(arquivoArvoreB); // Lê o cabeçalho atualizado da árvore B

    cabecalhoArvoreB->status = '1';

    escreverCabecalhoArvB(arquivoArvoreB, cabecalhoArvoreB); // Escreve o cabeçalho no arquivo de índices

    free(cabecalhoArvoreB);

    fclose(arquivoBinario); // Fecha o arquivo binário
    fclose(arquivoArvoreB); // Fecha o arquivo de índices

    return(1);
}