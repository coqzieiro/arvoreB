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
    apagarListaIndice(removidos->lista); // Libera a memória da lista de índices
    free(removidos->tamanhos); // Libera a memória dos tamanhos de registros removidos
    free(removidos); // Libera a memória da estrutura REMOVIDOS
}

// Função para remover um registro da lista de índices
void removerRegistroIndice(LISTA_INDICE *lista, int index) {
    free(lista->registros[index]); // Libera a memória do registro na posição index

    // Desloca todos os registros após o registro removido para a esquerda
    for (int i = index; i < lista->tamanho - 1; i++) {
        lista->registros[i] = lista->registros[i + 1];
    }

    lista->tamanho--; // Reduz o tamanho da lista
}

// Função para remover um registro da posição especificada na lista de removidos
void removerRegistroRemovidoPosicao(REMOVIDOS *removidos, int posicao) {
    removerRegistroIndice(removidos->lista, posicao); // Remove o registro na posição posicao da lista de índices

    // Desloca todos os tamanhos após a posição para a esquerda
    for(int i = posicao; i < removidos->lista->tamanho; i++) {
        removidos->tamanhos[i] = removidos->tamanhos[i + 1];
    }
}

// Função para remover um descendente de um registro de árvore B
int removerDescendenteRegistroArvoreB(DADOS_ARVORE_B *registro, int64_t descendente) {
    if (registro->nroChaves == 0) {
        return 0; // Retorna 0 se não há chaves no registro
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

    // Remove o descendente marcando como -1
    registro->descendentes[i] = -1;

    return 1; // Retorna 1 para indicar sucesso na remoção do descendente
}

// Função para criar uma lista de registros removidos a partir de um arquivo binário
REMOVIDOS *criarListaRemovidos(FILE *file) {
    CABECALHO_DADOS *cabecalho = lerCabecalhoDados(file); // Lê o cabeçalho do arquivo
    REMOVIDOS *removidos = criarListaRemovidosVazia(); // Cria uma lista vazia de registros removidos

    fseek(file, 0, SEEK_END); // Move para o final do arquivo para determinar o tamanho total
    int finalArquivo = ftell(file); // Obtém o tamanho do arquivo

    int proxByteOffset = cabecalho->topo; // Obtém o topo da lista de removidos

    // Percorre todos os registros removidos
    while(proxByteOffset != -1 && proxByteOffset < finalArquivo) {
        DADOS *registro = leitura_registro_arquivoBin(proxByteOffset, file); // Lê o registro do arquivo

        if(registro->removido == '1') {
            DADOS_INDICE *registroIndice = inicializa_registro_index(); // Inicializa um registro de índice
            registroIndice->index = registro->id; // Define o índice do registro de índice
            registroIndice->byteOffset = proxByteOffset; // Define o byte offset do registro de índice

            adicionarRegistroRemovido(removidos, registroIndice, registro->tamanhoRegistro); // Adiciona o registro removido à lista
        }

        proxByteOffset = registro->prox; // Atualiza o próximo byte offset

        free_registro(registro); // Libera a memória do registro lido

        // Lê o próximo registro para verificar o último registro removido
        if(proxByteOffset != -1 && proxByteOffset < finalArquivo) {
            DADOS *proxRegistro = leitura_registro_arquivoBin(proxByteOffset, file); // Lê o próximo registro

            // Anota o último registro removido se for o final do arquivo
            if(proxRegistro->prox == -1 && registro->removido == '1') {
                DADOS_INDICE *registroIndice = inicializa_registro_index(); // Inicializa um registro de índice
                registroIndice->index = registro->id; // Define o índice do registro de índice
                registroIndice->byteOffset = proxByteOffset; // Define o byte offset do registro de índice

                adicionarRegistroRemovido(removidos, registroIndice, registro->tamanhoRegistro); // Adiciona o registro removido à lista
                free_registro(proxRegistro); // Libera a memória do próximo registro
                break;
            }

            free_registro(proxRegistro); // Libera a memória do próximo registro
        }
    }

    free(cabecalho); // Libera a memória do cabeçalho lido

    return removidos; // Retorna a lista de registros removidos criada
}

// Função para criar uma lista vazia de registros removidos
REMOVIDOS *criarListaRemovidosVazia() {
    REMOVIDOS *removidos = malloc(sizeof(REMOVIDOS)); // Aloca memória para a estrutura REMOVIDOS
    removidos->lista = criarListaIndice(); // Cria uma lista vazia de índices
    removidos->tamanhos = malloc(sizeof(int) * 1000); // Aloca memória para os tamanhos de registros

    return removidos; // Retorna a estrutura REMOVIDOS criada
}

// Função para adicionar um novo registro na lista em ordem de tamanho
void adicionarRegistroRemovido(REMOVIDOS *removidos, DADOS_INDICE *registroIndice, int tamanho) {
    int right = removidos->lista->tamanho;
    int left = 0;

    // Busca binária para encontrar a posição correta para inserir o novo registro
    while (left < right) {
        int middle = (left + right) / 2;
        if (removidos->tamanhos[middle] > tamanho) {
            right = middle;
        }
        else {
            left = middle + 1;
        }
    }

    // Move elementos à direita para abrir espaço para o novo registro
    shiftElementosListaRemovidosRight(removidos, left);

    // Adiciona o novo registro na posição encontrada
    removidos->lista->registros[left] = registroIndice;

    removidos->tamanhos[left] = tamanho;

    // Atualiza o tamanho da lista de registros removidos
    removidos->lista->tamanho++;
}

// Função para remover uma chave de um registro de árvore B
int removerChaveRegistroArvoreB(DADOS_ARVORE_B *registro, int chave) {
    if (registro->nroChaves == 0) {
        // Se o registro estiver vazio, não há chave para remover
        return 0;
    }
    else {
        // Encontra a posição da chave a ser removida
        int i = 0;
        while (i < registro->nroChaves && registro->chaves[i] != chave) {
            i++;
        }
        if (i == registro->nroChaves) {
            // Se não encontrou a chave, retorna falha
            return 0;
        }
        else {
            // Desloca as chaves e byte offsets para preencher o espaço da chave removida
            for (int j = i; j < registro->nroChaves - 1; j++)
            {
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
    for (int i = removidos->lista->tamanho; i > pos; i--) {
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

    free(registro); // Libera a memória alocada para o registro de árvore B
    return 1; // Retorna 1 para indicar sucesso na operação de liberação de memória
}

// Função para remover um registro da lista de removidos e atualizar o arquivo
void removerRegistroRemovidoEAtualizarArquivo(REMOVIDOS *removidos, int posicao, FILE *file) {
    // Lê o registro a ser removido do arquivo binário
    DADOS *registro = leitura_registro_arquivoBin(removidos->lista->registros[posicao]->byteOffset, file);

    // Se a posição for inválida, retorna
    if (posicao == -1) {
        return;
    }

    // Obtém o tamanho atual da lista de removidos
    int tamanhoLista = removidos->lista->tamanho;

    // Constante para o deslocamento do próximo byte
    const int byteProx = 5;

    // Lê o cabeçalho do arquivo
    CABECALHO_DADOS *cabecalho = lerCabecalhoDados(file);

    // Define o status do cabeçalho como '0'
    cabecalho->status = '0';

    // Incrementa o número de registros de arquivo
    cabecalho->nroRegArq = cabecalho->nroRegArq + 1;

    // Escreve o número de registros de arquivo no arquivo
    const int gapNroRegArqByte = 17;
    fseek(file, gapNroRegArqByte, SEEK_SET);
    int nroRegArq = cabecalho->nroRegArq;
    fwrite(&nroRegArq, sizeof(int), 1, file);

    // Decrementa o número de registros removidos
    cabecalho->nroRegRem = cabecalho->nroRegRem - 1;

    // Escreve o número de registros removidos no arquivo
    const int gapNroRegRemByte = 21;
    fseek(file, gapNroRegRemByte, SEEK_SET);
    int nroRem = cabecalho->nroRegRem;
    fwrite(&nroRem, sizeof(int), 1, file);
    
    // Se a lista tiver apenas um elemento removido
    if (tamanhoLista == 1) {
        // Escreve o topo do cabeçalho no arquivo
        const int topoByte = 1;
        fseek(file, topoByte, SEEK_SET);
        int64_t topo = cabecalho->topo;
        fwrite(&topo, sizeof(int64_t), 1, file);

    } else if (posicao == 0) { // Se estiver removendo o primeiro elemento
        
        // Obtém o próximo registro na lista de removidos
        DADOS_INDICE *registroIndice = removidos->lista->registros[1];

        // Obtém o byte offset do próximo registro
        int64_t byteOffset = registroIndice->byteOffset;

        // Atualiza o topo do cabeçalho com o byte offset do próximo registro
        cabecalho->topo = byteOffset;

        // Escreve o topo do cabeçalho no arquivo
        const int topoByte = 1;
        fseek(file, topoByte, SEEK_SET);
        int64_t topo = cabecalho->topo;
        fwrite(&topo, sizeof(int64_t), 1, file);

    } else if (posicao == tamanhoLista - 1) { // Se estiver removendo o último elemento
        
        // Obtém o registro anterior na lista de removidos
        DADOS_INDICE *registroIndice = removidos->lista->registros[posicao - 1];

        // Obtém o byte offset do registro anterior
        int64_t byteOffset = registroIndice->byteOffset;

        int prox = -1;
        
        // Calcula o byte offset do próximo campo e escreve no arquivo
        byteOffset += byteProx;
        fseek(file, byteOffset, SEEK_SET);
        fwrite(&prox, sizeof(int), 1, file);
    } else {
        // Se estiver removendo um elemento no meio da lista

        // Obtém o registro anterior e o próximo na lista de removidos
        DADOS_INDICE *registroIndiceAnterior = removidos->lista->registros[posicao - 1];
        DADOS_INDICE *registroIndiceProximo = removidos->lista->registros[posicao + 1];

        // Obtém o byte offset do registro anterior e do próximo
        int64_t byteOffsetAnterior = registroIndiceAnterior->byteOffset;
        int64_t byteOffsetProximo = registroIndiceProximo->byteOffset;

        // Calcula o byte offset do próximo campo e escreve no arquivo
        byteOffsetAnterior += byteProx;
        fseek(file, byteOffsetAnterior, SEEK_SET);
        fwrite(&byteOffsetProximo, sizeof(int), 1, file);
    }

    // Libera a memória alocada para o registro lido
    free_registro(registro);

    // Libera a memória alocada para o cabeçalho lido
    free(cabecalho);

    // Remove o registro da lista de removidos na posição especificada
    removerRegistroRemovidoPosicao(removidos, posicao);
}