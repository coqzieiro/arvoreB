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
int criarArvoreB(char *binFileName, char *arvBFileName) {
    FILE *arvBFile = fopen(arvBFileName, "wb+");
    FILE *binFile = fopen(binFileName, "rb"); 

    // Inicialização dos cabeçalhos
    CABECALHO_DADOS *cabecalho = criarCabecalhoDados();
    CABECALHO_ARVORE_B *cabecalhoArvB = criarCabecalhoArvB(); // Cria um cabeçalho vazio para a árvore B

    // Vai para o começo do arquivo e lê o cabeçalho do arquivo de dados
    fseek(binFile, 0, SEEK_SET);
    cabecalho = lerCabecalhoDados(binFile);

    // Escreve o cabeçalho do arquivo da arvB no arquivo da arvB
    cabecalhoArvB->status = '0';    
    escreverCabecalhoArvB(arvBFile, cabecalhoArvB);

    // Verifica a consistencia do arquivo binário e de indices (arvoreB)
    if(!binFile || !arvBFile){
        printf("Falha no processamento do arquivo.\n");

        fclose(arvBFile);
        fclose(binFile);
        return(0);
    }
    if(cabecalho->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(binFile);
        fclose(arvBFile);
        free(cabecalho);
        return(0);
    }

    // Posição inicial do primeiro registro no arquivo binário
    int64_t posicaoDoRegistro = 25;

    int numero_registros = cabecalho->nroRegArq + cabecalho->nroRegRem;
    int i;

    for(i = 0; i < numero_registros; i++) {
        DADOS *registro = lerRegistroBinario(posicaoDoRegistro, binFile);

        // Se o registro for removido, pulamos ele
        if(registro->removido == '1') { 
            posicaoDoRegistro += registro->tamanhoRegistro;

            continue;
        }

        int id_registro =  registro->id;
        
        // Registro é inserido na arvB
        inserirArvoreB(arvBFile, id_registro, posicaoDoRegistro);
        
        // Atualiza a posição
        posicaoDoRegistro += registro->tamanhoRegistro;

        liberaRegistroDados(registro);
    }

    // Libera a memória alocada
    free(cabecalhoArvB);

    // Lê cabeçalho do arquivo da árvoreB e atualiza o status
    cabecalhoArvB = lerCabecalhoArvB(arvBFile);
    cabecalhoArvB->status = '1';

    // Cabeçalho é escrito no arquivo da árvoreB
    escreverCabecalhoArvB(arvBFile, cabecalhoArvB);

    // Libera a memória alocada
    free(cabecalhoArvB);
    free(cabecalho);
    fclose(arvBFile);
    fclose(binFile);

    return(1);
}