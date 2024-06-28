
/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionalidades.h"
#include "definicoesTipos.h"
#include "funcoes_fornecidas.h"
#include "funcoesAuxiliares.h"

int main(void){
    int opcao, numBuscas, numRemocoes, numInsercoes;
    char nomeArquivoCSV[50];
    char nomeArquivoBinario[50];
    char nomeArquivoIndex[50];

    scanf("%d", &opcao); // Determina qual funcionalidade irá executar

    switch(opcao){
        case 1: // Funcionalidade 1 (cria arquivo binário)
            scanf("%s %s", nomeArquivoCSV, nomeArquivoBinario);
            criarArquivoBinario(nomeArquivoCSV, nomeArquivoBinario);
            binarioNaTela(nomeArquivoBinario);
            break;
        case 2: // Funcionalidade 2 (lista os registros)
            scanf("%s", nomeArquivoBinario);
            listarRegistros(nomeArquivoBinario);
            break;
        case 3: // Funcionalidade 3 (faz a busca por campos)
            scanf("%s %d", nomeArquivoBinario, &numBuscas);
            buscarRegistros(nomeArquivoBinario, numBuscas);
            break;
        case 4: // Funcionalidade 4 (cria arquivo de index)
            scanf("%s %s", nomeArquivoBinario, nomeArquivoIndex);
            if (criarIndex(nomeArquivoBinario, nomeArquivoIndex, opcao)) {
                binarioNaTela(nomeArquivoIndex);
            }
            break;
        case 5: { // Funcionalidade 5 (remoção lógica do arquivo de dados)
            scanf("%s %s %d", nomeArquivoBinario, nomeArquivoIndex, &numRemocoes);
            // abertura do arquivo
            FILE* arquivoBinario = fopen(nomeArquivoBinario, "rb+");
            FILE* arquivoIndex = fopen(nomeArquivoIndex, "rb+");

            if (arquivoBinario == NULL) {
                printf("Falha no processamento do arquivo.\n");
                return 0;
            }

            if(arquivoIndex == NULL) {
                // Caso o arquivo de índice não exista, cria um novo
                arquivoIndex = fopen(nomeArquivoIndex, "wb");
            }

            int numCampos;

            for(int i = 0; i < numRemocoes; i++) {
                scanf("%d", &numCampos);
                remover(arquivoBinario, arquivoIndex, numCampos);
            }

            criarIndex(nomeArquivoBinario, nomeArquivoIndex, opcao);

            // Fechar os arquivos
            fclose(arquivoBinario);
            fclose(arquivoIndex);

            binarioNaTela(nomeArquivoBinario);
            binarioNaTela(nomeArquivoIndex);

            break;
        }
        case 6: {// Funcionalidade 6 (inserção de novos registros)
            scanf("%s %s %d", nomeArquivoBinario, nomeArquivoIndex, &numInsercoes);
            // Abertura do arquivo
            FILE* arquivoBinario = fopen(nomeArquivoBinario, "rb+");
            FILE* arquivoIndex = fopen(nomeArquivoIndex, "rb+");

            if (arquivoBinario == NULL) {
                printf("Falha no processamento do arquivo.\n");
                return 0;
            }

            if(arquivoIndex == NULL){
                // Caso o arquivo de índice não exista, cria um novo
                arquivoIndex = fopen(nomeArquivoIndex, "wb");
                criarIndex(nomeArquivoBinario, nomeArquivoIndex, opcao);

            }

            for(int i = 0; i < numInsercoes; i++) {
                inserir(arquivoBinario);
            }

            // Apagar o arquivo de índice
            fclose(arquivoIndex);
            arquivoIndex = fopen(nomeArquivoIndex, "wb");

            // Recria o arquivo binario de index
            criarIndex(nomeArquivoBinario, nomeArquivoIndex, opcao);

            // Fechar os arquivos
            fclose(arquivoBinario);
            fclose(arquivoIndex);

            binarioNaTela(nomeArquivoBinario);
            binarioNaTela(nomeArquivoIndex);

            break;
        }
        case 7: { // Funcionalidade 7 (Cria um arquivo de índice árvore-B)
            scanf("%s %s", nomeArquivoBinario, nomeArquivoIndex);

            if(criarArquivoArvoreB(nomeArquivoBinario, nomeArquivoIndex)){
                binarioNaTela(nomeArquivoIndex);
            }
            break;
        }
        case 8: { // Funcionalidade 8 (Recuperação de dados usando índice árvore-B)
            scanf("%s %s", nomeArquivoBinario, nomeArquivoIndex); // Lê o nome do arquivo binário

            FILE *fileBin = fopen(nomeArquivoBinario, "rb"); // Abre o arquivo binário no modo leitura
            if(fileBin == NULL) {
                printf("Falha no processamento do arquivo.\n"); // Verifica se ocorreu um erro ao abrir o arquivo
                return 0;
            }

            CABECALHO *cabecalhoBin = getCabecalhoFromBin(fileBin); // Lê o cabeçalho do arquivo binário
            if(getStatus(cabecalhoBin) == '0') {
                printf("Falha no processamento do arquivo.\n"); // Verifica se o status do cabeçalho é inválido
                apagarCabecalho(cabecalhoBin); // Libera a memória do cabeçalho
                fclose(fileBin); // Fecha o arquivo
                return 0;
            }

            apagarCabecalho(cabecalhoBin); // Libera a memória do cabeçalho

            int quantidadeBuscas;
            scanf("%d", &quantidadeBuscas); // Lê a quantidade de buscas

            for(int i=0; i<quantidadeBuscas; i++) {
                int id;
                scanf("\nid %d", &id); // Lê o id a ser buscado

                imprimirIdArvoreB(id, fileBin, nomeArquivoIndex, i, 0); // Chama a função para imprimir o registro correspondente ao id na árvore B
            }
            fclose(fileBin); // Fecha o arquivo binário
            break;
        }
        case 9: { // Funcionalidade 9 (Recuperação de dados usando qualquer campo de busca)
            char arquivoBin[50];
            scanf("%s", arquivoBin); // Lê o nome do arquivo binário

            char arquivoArvB[50];
            scanf("%s", arquivoArvB); // Lê o nome do arquivo da árvore B

            imprimeRegistrosBuscados(arquivoBin, 1, arquivoArvB); // Chama a função para imprimir registros buscados com a árvore B
            break;
        }
        case 10: { // Funcionalidade 10 (Inserção de novos registros usando índice árvore-B)
            char arquivoBin[50];
            scanf("%s", arquivoBin); // Lê o nome do arquivo binário

            char arquivoArvB[50];
            scanf("%s", arquivoArvB); // Lê o nome do arquivo da árvore B
            
            int numOperacoes;
            scanf("%d", &numOperacoes);

            // faz as insercoes dos registros no arquivo de dados e no arquivo da arvore b
            bool inseriu = inserirNovoDadoArvoreB(arquivoBin, arquivoArvB, numOperacoes);

            // se o registro foi inserido, imprime o binario na tela dos arquivos de dados e da arvore b
            if(inseriu) {
                binarioNaTela(arquivoBin);
                binarioNaTela(arquivoArvB);
            }
            break;
        }
        default: 
            printf("Opção inválida.\n");
            return(1);
    }
    return(0);
}