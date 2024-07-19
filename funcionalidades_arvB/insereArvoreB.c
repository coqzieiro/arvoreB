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
bool insercaoArvB(char *binFileName, char *arvBFileName, int quantidade_insercoes) {
    
    // inicialização e alocação de variáveis
    
    FILE *arvBFile = fopen(arvBFileName, "rb+");
    FILE *binFile = fopen(binFileName, "rb+");

    CABECALHO_DADOS *cabecalho = lerCabecalhoDados(binFile);
    CABECALHO_ARVORE_B *cabecalhoArvB = lerCabecalhoArvB(arvBFile); 

    REMOVIDOS *list_of_removed = criarListaRemovidos(binFile);
    DADOS **registros = malloc(sizeof(DADOS*) * quantidade_insercoes);

    char string_idade[10];
    char **nomeJogador = malloc(sizeof(char*) * quantidade_insercoes);
    char **nacionalidade = malloc(sizeof(char*) * quantidade_insercoes);
    char **nomeClube = malloc(sizeof(char*) * quantidade_insercoes);
    int id;

    for(int i = 0; i < quantidade_insercoes; i++) {
        nacionalidade[i] = malloc(sizeof(char) * 50);
        nomeClube[i] = malloc(sizeof(char) * 50);
        nomeJogador[i] = malloc(sizeof(char) * 50);
    }

    // faz as verificações necessárias nos ifs abaixo
    if (binFile == NULL || arvBFile == NULL) {
        printf("Falha no processamento do arquivo.\n");

        fclose(arvBFile);
        fclose(binFile);
        return false;
    }

    if (cabecalho == NULL || cabecalhoArvB == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return false;
    }

    if(cabecalho->status == '0' || cabecalhoArvB->status == '0') {
        printf("Falha no processamento do arquivo.\n");

        free(cabecalhoArvB);
        free(cabecalho);

        fclose(arvBFile);
        fclose(binFile);
        return false;
    }

    for(int i = 0; i < quantidade_insercoes; i++) {
        // Usuário digita o id
        scanf("%i", &id);

        scan_quote_string(string_idade);
        scan_quote_string(nomeJogador[i]);
        scan_quote_string(nacionalidade[i]);
        scan_quote_string(nomeClube[i]);

        int rrn;

        if(cabecalhoArvB == NULL)
            rrn = -1;
        else
            rrn = cabecalhoArvB->noRaiz;

        int64_t byteoffsetRegistro = buscarRegistroRecursivo(arvBFile, id, rrn);

        if(byteoffsetRegistro != -1) { // Registro já é existente
            nomeJogador[i] = '\0';
            nacionalidade[i] = '\0';
            nomeClube[i] = '\0';

            registros[i] = preencheRegistroDdos('1', 0, 0, 0, 0, 0, nomeJogador[i], 0, nomeClube[i], 0, nacionalidade[i]);

            continue;
        }

        int idade = (strcmp(string_idade, "NULO") == 0 || strcmp(string_idade, "") == 0) ? -1 : atoi(string_idade);

        if(strcmp(nomeJogador[i], "NULO") == 0 || strcmp(nomeJogador[i], "") == 0) strcpy(nomeJogador[i], "");
        if(strcmp(nacionalidade[i], "NULO") == 0 || strcmp(nacionalidade[i], "") == 0) strcpy(nacionalidade[i], "");
        if(strcmp(nomeClube[i], "NULO") == 0 || strcmp(nomeClube[i], "") == 0) strcpy(nomeClube[i], "");

        registros[i] = preencheRegistroDdos('0', 33 + strlen(nomeJogador[i]) + strlen(nomeClube[i]) + strlen(nacionalidade[i]), -1, id, idade, strlen(nomeJogador[i]), nomeJogador[i], strlen(nacionalidade[i]), nacionalidade[i], strlen(nomeClube[i]), nomeClube[i]);
    }
    
    free(cabecalhoArvB);

    int tamanho_registro = 0;

    // Gera um array contendo o byte offset com a estratégia best-fit para cada registro    
    int64_t *best_fit_registros = bestFitRegistros(list_of_removed, registros, quantidade_insercoes, binFile);
    
    for(int i = 0; i < quantidade_insercoes; i++) {
        if(best_fit_registros[i] == 0) continue; // Registro já é existente
        if(best_fit_registros[i] == -1) { // Registro será inserido no fim
            fseek(binFile, 0, SEEK_END);

            best_fit_registros[i] = ftell(binFile);
            tamanho_registro = 0;
            cabecalho->proxByteOffset = best_fit_registros[i] + registros[i]->tamanhoRegistro;

            // Escreve o próximo byteOffset
            const int gapProxByteOffset = 9;
            fseek(binFile, gapProxByteOffset, SEEK_SET);
            int64_t proxByteOffset = cabecalho->proxByteOffset;
            fwrite(&proxByteOffset, sizeof(int64_t), 1, binFile);
            
        } else {
            DADOS *registro = lerRegistroBinario(best_fit_registros[i], binFile);
            tamanho_registro = registro->tamanhoRegistro;
            liberaRegistroDados(registro);
        }

        registros[i]->prox = -1;

        // Registro é escrito no arquivo binário
        cabecalho->status = '0';
        
        // Escreve status antes de escrever o registro
        const int statusByteAntes = 0;
        fseek(binFile, statusByteAntes, SEEK_SET);
        char statusAntes = cabecalho->status;
        fwrite(&statusAntes, sizeof(char), 1, binFile);
        
        escreverRegistroDados(registros[i], best_fit_registros[i], tamanho_registro, binFile);

        // Escreve status depois de escrever o registro
        const int statusByteDepois = 0;
        cabecalho->status = '1';
        fseek(binFile, statusByteDepois, SEEK_SET);
        char statusDepois = cabecalho->status;
        fwrite(&statusDepois, sizeof(char), 1, binFile);


        // Status do arquivo da árvore B é atualizado
        char status_ArvBFile = '0';
        fseek(arvBFile, 0, SEEK_SET);
        fwrite(&status_ArvBFile, sizeof(char), 1, arvBFile);


        // Id e byteOffset são inseridos no arquivo da árvore-B
        inserirArvoreB(arvBFile, registros[i]->id, best_fit_registros[i]);


        // Status do arquivo da árvore B é atualizado
        status_ArvBFile = '1';
        fseek(arvBFile, 0, SEEK_SET);
        fwrite(&status_ArvBFile, sizeof(char), 1, arvBFile);
    }


    for(int i = 0; i < quantidade_insercoes; i++){
        liberaRegistroDados(registros[i]);
    }

    free(nomeJogador);
    free(nacionalidade);
    free(nomeClube);
    free(registros);
    free(best_fit_registros);
    free(cabecalho);
    apagarListaRemovidos(list_of_removed);
    fclose(arvBFile);
    fclose(binFile);
    return true;
}