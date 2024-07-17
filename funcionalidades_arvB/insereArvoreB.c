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

// Função para inserir novos dados na árvore B
bool inserirNovoDadoArvoreB(char *arquivoBinario, char *arquivoArvoreB, int numOperacoes) {
    FILE *arquivoBin = fopen(arquivoBinario, "rb+");
    FILE *fileArvoreB = fopen(arquivoArvoreB, "rb+");

    if (arquivoBin == NULL || fileArvoreB == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBin);
        fclose(fileArvoreB);
        return false;
    }
    CABECALHO_DADOS *cabecalho = cabecalhoLido(arquivoBin);
    CABECALHO_ARVORE_B *cabecalhoArvoreB = lerCabecalhoArvoreB(fileArvoreB); // Lê o cabeçalho da árvore B

    if (cabecalho == NULL || cabecalhoArvoreB == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return false;
    }

    if(cabecalho->status == '0' || cabecalhoArvoreB->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        free(cabecalho);
        free(cabecalhoArvoreB);
        fclose(arquivoBin);
        fclose(fileArvoreB);
        return false;
    }

    REMOVIDOS *removidos = criarListaRemovidos(arquivoBin);
    DADOS **registros = malloc(sizeof(DADOS*) * numOperacoes);

    int id;
    char idadeStr[10];
    char **nomeJogador = malloc(sizeof(char*) * numOperacoes);
    char **nacionalidade = malloc(sizeof(char*) * numOperacoes);
    char **nomeClube = malloc(sizeof(char*) * numOperacoes);

    for(int i = 0; i < numOperacoes; i++) {
        nomeJogador[i] = malloc(sizeof(char) * 50);
        nacionalidade[i] = malloc(sizeof(char) * 50);
        nomeClube[i] = malloc(sizeof(char) * 50);
    }

    for(int i = 0; i < numOperacoes; i++) {
        // Lê o input do usuário
        scanf("%i", &id);
        scan_quote_string(idadeStr);
        scan_quote_string(nomeJogador[i]);
        scan_quote_string(nacionalidade[i]);
        scan_quote_string(nomeClube[i]);

        int rrnAtual;

        if(cabecalhoArvoreB == NULL) {
            rrnAtual = -1;
        } else {
            rrnAtual = cabecalhoArvoreB->noRaiz;
        }

        int64_t byteoffsetRegistro = buscarRegistroIdRec(fileArvoreB, id, rrnAtual);

        if(byteoffsetRegistro != -1) { // Registro já existe
            nomeJogador[i] = '\0';
            nacionalidade[i] = '\0';
            nomeClube[i] = '\0';
            registros[i] = atribui_valores_registro('1', 0, 0, 0, 0, 0, nomeJogador[i], 0, nomeClube[i], 0, nacionalidade[i]);
            continue;
        }

        int idade = (strcmp(idadeStr, "NULO") == 0 || strcmp(idadeStr, "") == 0) ? -1 : atoi(idadeStr);

        if(strcmp(nomeJogador[i], "NULO") == 0 || strcmp(nomeJogador[i], "") == 0) strcpy(nomeJogador[i], "");
        if(strcmp(nacionalidade[i], "NULO") == 0 || strcmp(nacionalidade[i], "") == 0) strcpy(nacionalidade[i], "");
        if(strcmp(nomeClube[i], "NULO") == 0 || strcmp(nomeClube[i], "") == 0) strcpy(nomeClube[i], "");

        registros[i] = atribui_valores_registro('0', 33 + strlen(nomeJogador[i]) + strlen(nomeClube[i]) + strlen(nacionalidade[i]), -1, id, idade, strlen(nomeJogador[i]), nomeJogador[i], strlen(nacionalidade[i]), nacionalidade[i], strlen(nomeClube[i]), nomeClube[i]);
    }

    limpaCabecalhoArvoreB(cabecalhoArvoreB); // Libera a memória do cabeçalho

    // Obtém o byteOffset do best fit de cada registro
    int64_t *byteOffsets = getBestFitArrayRegistros(removidos, registros, numOperacoes, arquivoBin);
    int tamanhoRegistroAtual = 0;

    for(int i = 0; i < numOperacoes; i++) {
        if(byteOffsets[i] == 0) continue; // Registro já existe
        if(byteOffsets[i] == -1) { // Registro será inserido no fim
            tamanhoRegistroAtual = 0;
            fseek(arquivoBin, 0, SEEK_END);
            byteOffsets[i] = ftell(arquivoBin);
            cabecalho->proxByteOffset = byteOffsets[i] + registros[i]->tamanhoRegistro;
            writeProxByteOffsetCabecalho(cabecalho, arquivoBin);
        } else {
            DADOS *registro = leitura_registro_arquivoBin(byteOffsets[i], arquivoBin);
            tamanhoRegistroAtual = registro->tamanhoRegistro;
            free_registro(registro);
        }

        registros[i]->prox = -1;
        // Escrevendo o registro no arquivo binário
        cabecalho->status = '0';
        writeStatusCabecalho(cabecalho, arquivoBin);
        escreverRegistro(registros[i], byteOffsets[i], tamanhoRegistroAtual, arquivoBin);
        cabecalho->status = '1';
        writeStatusCabecalho(cabecalho, arquivoBin);

        // Atualiza o status do arquivo da árvore B para '0'
        fseek(fileArvoreB, 0, SEEK_SET);
        char statusArquivoArvoreB = '0';
        fwrite(&statusArquivoArvoreB, sizeof(char), 1, fileArvoreB);

        // Insere a chave e o byteOffset no arquivo da árvore B
        inserirArvoreB(fileArvoreB,  registros[i]->id, byteOffsets[i]);

        // Atualiza o status do arquivo da árvore B para '1'
        fseek(fileArvoreB, 0, SEEK_SET);
        statusArquivoArvoreB = '1';
        fwrite(&statusArquivoArvoreB, sizeof(char), 1, fileArvoreB);
    }

    for(int i = 0; i < numOperacoes; i++) {
        free_registro(registros[i]);
    }

    free(registros);
    free(byteOffsets);
    free(nomeJogador);
    free(nacionalidade);
    free(nomeClube);

    free(cabecalho);
    apagarListaRemovidos(removidos);

    fclose(arquivoBin);
    fclose(fileArvoreB);

    return true;
}