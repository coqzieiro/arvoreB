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
int criarArvoreB(char *nomeArquivoBinario, char *nomeArquivoIndex) {
    FILE *arquivoArvB = fopen(nomeArquivoIndex, "wb+");
    FILE *arquivoBinario = fopen(nomeArquivoBinario, "rb"); 

    // Verifica a consistencia do arquivo binário e de indices (arvoreB)
    if(arquivoBinario == NULL || arquivoArvB == NULL){
        printf("Falha no processamento do arquivo.\n");

        //fclose(arvBFile);
        //fclose(binFile);
        return(0);
    }

    // Inicialização dos cabeçalhos
    CABECALHO_DADOS *cabecalho = criarCabecalhoDados();
    CABECALHO_ARVORE_B *cabecalhoArvB = criarCabecalhoArvB(); // Cria um cabeçalho vazio para a árvore B

    // Vai para o começo do arquivo e lê o cabeçalho do arquivo de dados
    fseek(arquivoBinario, 0, SEEK_SET);
    cabecalho = lerCabecalhoDados(arquivoBinario);

    // Escreve o cabeçalho do arquivo da arvB no arquivo da arvB
    cabecalhoArvB->status = '0';    
    escreverCabecalhoArvB(arquivoArvB, cabecalhoArvB);

    if(cabecalho->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBinario);
        fclose(arquivoArvB);
        free(cabecalho);
        return(0);
    }

    // Posição inicial do primeiro registro no arquivo binário
    int64_t posicaoDoRegistro = 25;

    int numero_registros = cabecalho->nroRegArq + cabecalho->nroRegRem;
    int i;

    for(i = 0; i < numero_registros; i++) {
        DADOS *registro = lerRegistroBinario(posicaoDoRegistro, arquivoBinario);

        // Se o registro for removido, pulamos ele
        if(registro->removido == '1') { 
            posicaoDoRegistro += registro->tamanhoRegistro;

            continue;
        }

        int id_registro =  registro->id;
        
        // Registro é inserido na arvB
        inserirArvoreB(arquivoArvB, id_registro, posicaoDoRegistro);
        
        // Atualiza a posição
        posicaoDoRegistro += registro->tamanhoRegistro;

        liberaRegistroDados(registro);
    }

    // Libera a memória alocada
    free(cabecalhoArvB);

    // Lê cabeçalho do arquivo da árvoreB e atualiza o status
    cabecalhoArvB = lerCabecalhoArvB(arquivoArvB);
    cabecalhoArvB->status = '1';

    // Cabeçalho é escrito no arquivo da árvoreB
    escreverCabecalhoArvB(arquivoArvB, cabecalhoArvB);

    // Libera a memória alocada
    free(cabecalhoArvB);
    free(cabecalho);
    fclose(arquivoArvB);
    fclose(arquivoBinario);

    return(1);
}