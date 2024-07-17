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

// Função para apagar a lista de registros removidos
void apagarListaRemovidos(REMOVIDOS *removidos) {
    apagarListaIndice(removidos->lista);
    free(removidos->tamanhos);
    free(removidos);
}

// Função para remover um registro da lista de índices
void removerRegistroIndice(LISTA_INDICE *lista, int index) {
    free(lista->registros[index]);

    // Desloca todos os registros após o registro removido para a esquerda
    for (int i = index; i < lista->tamanho - 1; i++) {
        lista->registros[i] = lista->registros[i + 1];
    }

    lista->tamanho--;
}

// Função para remover um registro da posição especificada na lista de removidos
void removerRegistroRemovidoPosicao(REMOVIDOS *removidos, int posicao) {
    removerRegistroIndice(removidos->lista, posicao);

    for(int i = posicao; i < removidos->lista->tamanho; i++) {
        removidos->tamanhos[i] = removidos->tamanhos[i + 1];
    }
}

// Função para remover um descendente de um registro de árvore B
int removerDescendenteRegistroArvoreB(DADOS_ARVORE_B *registro, int64_t descendente) {
    if (registro->nroChaves == 0) {
        return 0;
    }

    int i = 0;
    // Encontra a posição do descendente a ser removido
    while (i < registro->nroChaves && registro->descendentes[i] != descendente) {
        i++;
    }

    if (i == ORDEM_ARVORE_B) {
        // Se não encontrou o descendente, retorna falha
        return 0;
    }

    // Remove o descendente
    registro->descendentes[i] = -1;

    return 1;
}

// Função para criar uma lista de registros removidos a partir de um arquivo binário
REMOVIDOS *criarListaRemovidos(FILE *file) {
    CABECALHO_DADOS *cabecalho = cabecalhoLido(file); // Obtém o cabeçalho do arquivo
    REMOVIDOS *removidos = criarListaRemovidosVazia(); // Cria uma lista vazia de registros removidos

    fseek(file, 0, SEEK_END);
    int finalArquivo = ftell(file); // Obtém o final do arquivo

    int proxByteOffset = cabecalho->topo; // Obtém o topo da lista de removidos
    
    int count = 0;

    // Percorre todos os registros removidos
    while(proxByteOffset != -1 && proxByteOffset < finalArquivo) {
        DADOS *registro = leitura_registro_arquivoBin(proxByteOffset, file);

        count++;

        if(registro->removido == '1') {
            DADOS_INDICE *registroIndice = inicializa_registro_index();
            registroIndice->index = registro->id;
            registroIndice->byteOffset = proxByteOffset;

            adicionarRegistroRemovido(removidos, registroIndice, registro->tamanhoRegistro);
        }

        proxByteOffset =registro->prox;

        free_registro(registro);

        DADOS *proxRegistro;

        if(proxByteOffset != -1 && proxByteOffset < finalArquivo)
            proxRegistro = leitura_registro_arquivoBin(proxByteOffset, file);

        // Anota o último registro removido
        if(proxRegistro->prox == -1 && registro->removido == '1') {
            DADOS_INDICE *registroIndice = inicializa_registro_index();
            registroIndice->index = registro->id;
            registroIndice->byteOffset = proxByteOffset;

            adicionarRegistroRemovido(removidos, registroIndice, registro->tamanhoRegistro);
            free_registro(proxRegistro);
            break;
        }

        free_registro(proxRegistro);
    }

    free(cabecalho);

    return removidos;
}

// Função para criar uma lista vazia de registros removidos
REMOVIDOS *criarListaRemovidosVazia() {
    REMOVIDOS *removidos = malloc(sizeof(REMOVIDOS));
    removidos->lista = criarListaIndice();
    removidos->tamanhos = malloc(sizeof(int) * 1000);

    return removidos;
}

// Função para adicionar um novo registro na lista em ordem de tamanho
void adicionarRegistroRemovido(REMOVIDOS *removidos, DADOS_INDICE *registroIndice, int tamanho) {
    int right = removidos->lista->tamanho;
    int left = 0;
    
    while (left < right) {
        int middle = (left + right) / 2;
        if (removidos->tamanhos[middle] > tamanho) {
            right = middle;
        } else {
            left = middle + 1;
        }
    }

    // Move elementos à direita para abrir espaço para o novo registro
    shiftElementosListaRemovidosRight(removidos, left);

    // Adiciona o novo registro na posição encontrada
    removidos->lista->registros[left] = registroIndice;

    removidos->tamanhos[left] = tamanho;

    // Atualiza o tamanho da 
    removidos->lista->tamanho = removidos->lista->tamanho + 1;
}

// Função para remover uma chave de um registro de árvore B
int removerChaveRegistroArvoreB(DADOS_ARVORE_B *registro, int chave) {
    if (registro->nroChaves == 0) {
        // Se o registro estiver vazio, não há chave para remover
        return 0;
    } else {
        // Encontra a posição da chave a ser removida
        int i = 0;
        while (i < registro->nroChaves && registro->chaves[i] != chave) {
            i++;
        }
        if (i == registro->nroChaves) {
            // Se não encontrou a chave, retorna falha
            return 0;
        } else {
            // Desloca as chaves e byte offsets para preencher o espaço da chave removida
            for (int j = i; j < registro->nroChaves - 1; j++) {
                registro->chaves[j] = registro->chaves[j + 1];
                registro->byteOffsets[j] = registro->byteOffsets[j + 1];
            }
            registro->nroChaves--;
        }
    }

    return 1;
}

// Função para deslocar elementos da lista de removidos para a direita
void shiftElementosListaRemovidosRight(REMOVIDOS *removidos, int pos) {
    for(int i = removidos->lista->tamanho; i > pos; i--) {
        DADOS_INDICE *registro = removidos->lista->registros[i - 1];
        removidos->lista->registros[i] = registro;
        removidos->tamanhos[i] = removidos->tamanhos[i - 1];
    }
}

// Função para liberar a memória de um registro de árvore B
int apagarRegistroArvoreB(DADOS_ARVORE_B *registro) {
    if (registro == NULL) {
        return 0;
    }
    
    free(registro);
    return 1;
}

// Função para remover um registro da lista de removidos e atualizar o arquivo
void removerRegistroRemovidoEAtualizarArquivo(REMOVIDOS *removidos, int posicao, FILE *file) {
    DADOS *registro = leitura_registro_arquivoBin(removidos->lista->registros[posicao]->byteOffset, file);

    if(posicao == -1) {
        return;
    }

    int tamanhoLista = removidos->lista->tamanho;
    const int byteProx = 5;
    CABECALHO_DADOS *cabecalho = cabecalhoLido(file);

    cabecalho->status = '0';


    cabecalho->nroRegArq = cabecalho->nroRegArq + 1;
    writeNroRegArqCabecalho(cabecalho, file);
    cabecalho->nroRegRem = cabecalho->nroRegRem - 1;
    writeNroRegRemCabecalho(cabecalho, file);
    
    if(tamanhoLista == 1) { // Lista só tem um elemento removido
        writeTopoCabecalho(cabecalho, file);
    } else if(posicao == 0) { // Removendo o primeiro elemento
        
        DADOS_INDICE *registroIndice = removidos->lista->registros[1];

        int64_t byteOffset = registroIndice->byteOffset;

        cabecalho->topo = byteOffset;

        writeTopoCabecalho(cabecalho, file);
    } else if(posicao == tamanhoLista - 1) { // Removendo o último elemento
        
        DADOS_INDICE *registroIndice = removidos->lista->registros[posicao - 1];

        int64_t byteOffset = registroIndice->byteOffset;

        int prox = -1;
        
        byteOffset += byteProx;
        fseek(file, byteOffset, SEEK_SET);
        fwrite(&prox, sizeof(int), 1, file);
    } else {
        DADOS_INDICE *registroIndiceAnterior = removidos->lista->registros[posicao - 1];

        DADOS_INDICE *registroIndiceProximo = removidos->lista->registros[posicao + 1];

        int64_t byteOffsetAnterior = registroIndiceAnterior->byteOffset;

        int64_t byteOffsetProximo = registroIndiceProximo->byteOffset;

        byteOffsetAnterior += byteProx;
        fseek(file, byteOffsetAnterior, SEEK_SET);
        fwrite(&byteOffsetProximo, sizeof(int), 1, file);
    }

    free_registro(registro);
    free(cabecalho);
    removerRegistroRemovidoPosicao(removidos, posicao);
}