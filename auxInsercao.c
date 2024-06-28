#include "funcoesAuxiliares.h"
#include "funcoes_fornecidas.h"
#include "funcionalidades.h"
#include "definicoesTipos.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// Função que insere um nó na lista de forma ordenada
void InserirOrdenado(LISTA** cabeca, LISTA* novoNo) {
    LISTA* atual;

    // Caso o nó seja inserido no início da lista
    if (*cabeca == NULL || (*cabeca)->tamRegistro > novoNo->tamRegistro) {
        novoNo->prox = *cabeca;
        *cabeca = novoNo;
    } else {
        // Encontra a posição correta para inserir o nó
        LISTA* anterior = NULL;
        atual = *cabeca;

        while (atual != NULL && atual->tamRegistro <= novoNo->tamRegistro) {
            anterior = atual;
            atual = atual->prox;
        }

        novoNo->prox = anterior->prox;
        anterior->prox = novoNo;
    }
}

// Função que reescreve os registros removidos no arquivo de dados
int ReescreveRemovidos(FILE* arquivoBinario, LISTA* removidos) {
    if (removidos == NULL) {
        fseek(arquivoBinario, 1, SEEK_SET);
        int64_t topo = -1;
        fwrite(&topo, sizeof(int64_t), 1, arquivoBinario);
        return true;
    }

    // Atualiza o topo da lista de removidos
    fseek(arquivoBinario, 1, SEEK_SET);
    fwrite(&removidos->byteOffset, sizeof(int64_t), 1, arquivoBinario);

    LISTA* atual = removidos;
    while (atual != NULL) {
        fseek(arquivoBinario, atual->byteOffset, SEEK_SET);

        char removido = '1';
        fwrite(&removido, sizeof(char), 1, arquivoBinario);
        fwrite(&atual->tamRegistro, sizeof(int), 1, arquivoBinario);

        if (atual->prox != NULL) {
            fwrite(&atual->prox->byteOffset, sizeof(int64_t), 1, arquivoBinario);
        } else {
            int64_t fimLista = -1;
            fwrite(&fimLista, sizeof(int64_t), 1, arquivoBinario);
        }

        atual = atual->prox;
    }
    return 1;
}

// Função que libera a memória da lista encadeada
void DesalocaLista(LISTA* cabeca) {
    LISTA* atual = cabeca;
    LISTA* proximo;

    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

// Função que encontra o melhor byte offset para inserção (best fit)
int64_t MetodoBestFit(LISTA** removidos, int tamRegistro, FILE* arquivoBinario) {
    LISTA* anterior = NULL;
    LISTA* atual = *removidos;

    // Percorre a lista para encontrar o melhor ajuste
    while (atual != NULL) {
        if (atual->tamRegistro >= tamRegistro) {
            int64_t offset = atual->byteOffset;

            if (anterior == NULL) {
                *removidos = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            return offset;
        }
        anterior = atual;
        atual = atual->prox;
    }

    // Retorna -1 se não encontrar um ajuste adequado
    return -1;
}

// Função que cria uma lista ordenada dos registros removidos
LISTA* OrdenaRegistrosRem(FILE* arquivoBinario) {
    fseek(arquivoBinario, 1, SEEK_SET);
    int64_t posicaoAtual = ftell(arquivoBinario);

    int64_t topo;
    LISTA* listaOrdenada = NULL;

    // Leitura do topo da lista de removidos
    fread(&topo, sizeof(int64_t), 1, arquivoBinario);

    if (topo == -1) {
        fseek(arquivoBinario, posicaoAtual, SEEK_SET);
        return listaOrdenada;
    }

    fseek(arquivoBinario, topo, SEEK_SET);
    DADOS registro_dados;

    // Aloca memória para o registro
    AlocaMemoriaRegistro(&registro_dados);

    while (topo != -1) {
        if (fread(&registro_dados.removido, sizeof(char), 1, arquivoBinario) != 1 ||
            fread(&registro_dados.tamanhoRegistro, sizeof(int), 1, arquivoBinario) != 1 ||
            fread(&registro_dados.prox, sizeof(int64_t), 1, arquivoBinario) != 1) {

            printf("Erro: falha em ler o próximo registro\n");
            break;
        }

        LISTA* no = (LISTA*)malloc(sizeof(LISTA));
        no->byteOffset = topo;
        no->tamRegistro = registro_dados.tamanhoRegistro;
        no->prox = NULL;

        // Insere o nó de forma ordenada
        InserirOrdenado(&listaOrdenada, no);

        topo = registro_dados.prox;

        if (topo != -1) {
            fseek(arquivoBinario, topo, SEEK_SET);
        }
    }

    free(registro_dados.nacionalidade);
    free(registro_dados.nomeClube);
    free(registro_dados.nomeJogador);
    
    fseek(arquivoBinario, posicaoAtual, SEEK_SET);

    return listaOrdenada;
}

// Função que lê os campos do registro
void LeituraCampos(DADOS* registro) {
    if (registro == NULL) {
        return;
    }

    // Leitura do id
    scanf("%d", &registro->id);

    char idade[15];
    // Leitura da idade
    scanf("%s", idade);
    
    if (strcmp(idade, "NULO") == 0) {
        registro->idade = -1;
    } else {
        registro->idade = atoi(idade); 
    }

    scan_quote_string(registro->nomeJogador);
    scan_quote_string(registro->nacionalidade);
    scan_quote_string(registro->nomeClube);
}

// Atualiza o tamanho das strings do registro
int AtualizaTamanhoStrings(DADOS* registro) {
    if (registro == NULL) {
        return 0;
    }
    registro->tamNomeJog = strlen(registro->nomeJogador);
    registro->tamNacionalidade = strlen(registro->nacionalidade);
    registro->tamNomeClube = strlen(registro->nomeClube);

    return 1;
}

// Função que reutiliza um registro removido ou insere um novo registro
int* ReutilizarRegistro(FILE* arquivoBinario, CABECALHO* cabecalho, DADOS* registro, int BestFit, LISTA* removidos) {
    int tamRegRemovido;

    if (BestFit != -1) {
        cabecalho->nroRegRem--;

        fseek(arquivoBinario, BestFit, SEEK_SET);
        fread(&registro->removido, sizeof(char), 1, arquivoBinario);
        fread(&tamRegRemovido, sizeof(int), 1, arquivoBinario);

        fseek(arquivoBinario, BestFit, SEEK_SET);
    } else {
        cabecalho->proxByteOffset += registro->tamanhoRegistro;
        tamRegRemovido = registro->tamanhoRegistro;
        fseek(arquivoBinario, 0, SEEK_END);
    }

    if (removidos && BestFit == cabecalho->topo) {
        cabecalho->topo = removidos->byteOffset;
    }

    int tamRegistroInserido = registro->tamanhoRegistro;
    registro->tamanhoRegistro = tamRegRemovido;

    int* tamanhoReg = (int*)malloc(2 * sizeof(int));
    tamanhoReg[0] = tamRegRemovido;
    tamanhoReg[1] = tamRegistroInserido;

    return tamanhoReg;
}

// Função que escreve o registro no arquivo binário
void EscreveRegistroInsercao(FILE* arquivoBinario, DADOS* registro) {
    fwrite(&registro->removido, sizeof(registro->removido), 1, arquivoBinario);
    fwrite(&registro->tamanhoRegistro, sizeof(registro->tamanhoRegistro), 1, arquivoBinario);
    fwrite(&registro->prox, sizeof(registro->prox), 1, arquivoBinario);
    fwrite(&registro->id, sizeof(registro->id), 1, arquivoBinario);
    fwrite(&registro->idade, sizeof(registro->idade), 1, arquivoBinario);
    fwrite(&registro->tamNomeJog, sizeof(registro->tamNomeJog), 1, arquivoBinario);
    fwrite(registro->nomeJogador, sizeof(char), registro->tamNomeJog, arquivoBinario);
    fwrite(&registro->tamNacionalidade, sizeof(registro->tamNacionalidade), 1, arquivoBinario);
    fwrite(registro->nacionalidade, sizeof(char), registro->tamNacionalidade, arquivoBinario);
    fwrite(&registro->tamNomeClube, sizeof(registro->tamNomeClube), 1, arquivoBinario);
    fwrite(registro->nomeClube, sizeof(char), registro->tamNomeClube, arquivoBinario);
}

// Função que escreve o preenchimento no final do registro
void EscreveLixo(FILE* arquivoBinario, int tamRegistroRemovido, int tamRegistroInserido) {
    if (tamRegistroRemovido == tamRegistroInserido) return;

    int diferencaBytes = tamRegistroRemovido - tamRegistroInserido;

    for (int i = 0; i < diferencaBytes; i++) {
        char cifrao = '$';
        fwrite(&cifrao, sizeof(char), 1, arquivoBinario);
    }
}

// Função que inicializa o cabeçalho
void IniCabecalho(CABECALHO* cabecalho) {
    cabecalho->status = '0';
    cabecalho->topo = -1;
    cabecalho->proxByteOffset = 25;
    cabecalho->nroRegArq = 0;
    cabecalho->nroRegRem = 0;
}