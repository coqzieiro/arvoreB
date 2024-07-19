
/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/funcionalidades.h"
#include "../bibliotecas/definicoesTipos.h"
#include "../bibliotecas/funcoes_fornecidas.h"
#include "../bibliotecas/funcoesAuxiliares.h"
#include "../bibliotecas/funcoesAuxiliaresB.h"

int main(void){
    int opcao, numBuscas, numRemocoes, numInsercoes, id;
    char nomeArquivoCSV[50];
    char nomeArquivoBinario[50];
    char nomeArquivoIndex[50];

    scanf("%d", &opcao); // Escolhe a operação a ser realizada

    switch(opcao){
        case 1: // Geração de arquivo binário a partir de CSV
            scanf("%s %s", nomeArquivoCSV, nomeArquivoBinario);
            criarArquivoBinario(nomeArquivoCSV, nomeArquivoBinario);
            binarioNaTela(nomeArquivoBinario);
            break;
        case 2: // Apresentação de registros
            scanf("%s", nomeArquivoBinario);
            listarRegistros(nomeArquivoBinario);
            break;
        case 3: // Pesquisa de registros por campos
            scanf("%s %d", nomeArquivoBinario, &numBuscas);
            buscarRegistros(nomeArquivoBinario, numBuscas);
            break;
        case 4: // Criação de arquivo de índice
            scanf("%s %s", nomeArquivoBinario, nomeArquivoIndex);

            // Se o index é criado com sucesso
            if (criarIndex(nomeArquivoBinario, nomeArquivoIndex, opcao)) {
                // Imprime a o binário na tela
                binarioNaTela(nomeArquivoIndex);
            }
            break;
        case 5: { // Remoção lógica de registros
            scanf("%s %s %d", nomeArquivoBinario, nomeArquivoIndex, &numRemocoes);
            // Abrindo o arquivo
            FILE* arquivoBinario = fopen(nomeArquivoBinario, "rb+");
            FILE* arquivoIndex = fopen(nomeArquivoIndex, "rb+");

            if (arquivoBinario == NULL) {
                printf("Falha no processamento do arquivo.\n");
                return 0;
            }

            if(arquivoIndex == NULL) {
                // Cria um novo arquivo de índice se não existir
                arquivoIndex = fopen(nomeArquivoIndex, "wb");
            }

            int numCampos;

            // Faz a remoção dos registros
            for(int i = 0; i < numRemocoes; i++) {
                scanf("%d", &numCampos);
                remover(arquivoBinario, arquivoIndex, numCampos);
            }

            criarIndex(nomeArquivoBinario, nomeArquivoIndex, opcao);

            // Encerrando os arquivos
            fclose(arquivoBinario);
            fclose(arquivoIndex);

            binarioNaTela(nomeArquivoBinario);
            binarioNaTela(nomeArquivoIndex);

            break;
        }
        case 6: {// Inserção de novos registros
            scanf("%s %s %d", nomeArquivoBinario, nomeArquivoIndex, &numInsercoes);
            // Abrindo o arquivo
            FILE* arquivoBinario = fopen(nomeArquivoBinario, "rb+");
            FILE* arquivoIndex = fopen(nomeArquivoIndex, "rb+");

            if (arquivoBinario == NULL) {
                printf("Falha no processamento do arquivo.\n");
                return 0;
            }

            if(arquivoIndex == NULL){
                // Cria um novo arquivo de índice se não existir
                arquivoIndex = fopen(nomeArquivoIndex, "wb");
                criarIndex(nomeArquivoBinario, nomeArquivoIndex, opcao);

            }

            // Insere os dados
            for(int i = 0; i < numInsercoes; i++) {
                inserir(arquivoBinario);
            }

            // Limpa o arquivo de índice
            fclose(arquivoIndex);
            arquivoIndex = fopen(nomeArquivoIndex, "wb");

            // Recria o arquivo de índice
            criarIndex(nomeArquivoBinario, nomeArquivoIndex, opcao);

            // Encerrando os arquivos
            fclose(arquivoBinario);
            fclose(arquivoIndex);

            // Imprime o binário na tela
            binarioNaTela(nomeArquivoBinario);
            binarioNaTela(nomeArquivoIndex);

            break;
        }
        case 7: { // Criação de arquivo de índice tipo árvore-B
            scanf("%s %s", nomeArquivoBinario, nomeArquivoIndex);

            if(criarArvoreB(nomeArquivoBinario, nomeArquivoIndex)) {
                binarioNaTela(nomeArquivoIndex);
            }
            break;
        }
        case 8: { // Recuperação de dados via índice árvore-B
            scanf("%s %s %d", nomeArquivoBinario, nomeArquivoIndex, &numBuscas); // Recebe o nome do arquivo binário

            FILE *arquivoBinario = fopen(nomeArquivoBinario, "rb"); // Abre o arquivo binário para leitura

            if(arquivoBinario == NULL) {
                printf("Falha no processamento do arquivo.\n"); // Verifica erro na abertura do arquivo
                return 0;
            }

            CABECALHO_DADOS *cabecalhoBin = lerCabecalhoDados(arquivoBinario); // Obtém o cabeçalho do arquivo

            if(cabecalhoBin->status == '0') {
                printf("Falha no processamento do arquivo.\n"); // Checa a validade do status do cabeçalho
                free(cabecalhoBin);
                fclose(arquivoBinario); // Fecha o arquivo
                return(0);
            }

            free(cabecalhoBin); // Libera recursos do cabeçalho

            for(int i=0; i<numBuscas; i++) {
                scanf("\nid %d", &id); // Captura o id para busca
                buscaIdArvore(id, arquivoBinario, nomeArquivoIndex, i, 0); // Executa a busca do registro via id na árvore B
            }
            fclose(arquivoBinario); // Fecha o arquivo
            break;
        }
        case 9: { // Busca de dados por qualquer campo usando índice
            scanf("%s %s", nomeArquivoBinario, nomeArquivoIndex); // Recebe o nome dos arquivos

            recuperaRegistrosCorrespondentesBusca(nomeArquivoBinario, 1, nomeArquivoIndex); // Exibe registros buscados com a árvore B

            break;
        }
        case 10: { // Inserção de registros via índice árvore-B
            // Recebe o nome do arquivo binário, index e número de buscas
            scanf("%s %s %d", nomeArquivoBinario, nomeArquivoIndex, &numBuscas);

            // Realiza as inserções nos arquivos de dados e de árvore B
            bool inseriu = insercao_arvoreB(nomeArquivoBinario, nomeArquivoIndex, numBuscas);

            // Se inserido com sucesso
            if(inseriu) {
                // Imprime os arquivos binários de dados e índice
                binarioNaTela(nomeArquivoBinario);
                binarioNaTela(nomeArquivoIndex);
            }
            break;
        }
        default: 
            printf("Opção inválida.\n");
            return(1);
    }
    return(0);
}