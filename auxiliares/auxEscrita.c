/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/funcoesAuxiliares.h"
#include "../bibliotecas/funcoesAuxiliaresB.h"
#include "../bibliotecas/funcoes_fornecidas.h"
#include "../bibliotecas/funcionalidades.h"
#include "../bibliotecas/definicoesTipos.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

// Função que escreve o cabeçalho no arquivo de dados
void EscritaCabecalho(CABECALHO* cabecalho, FILE* arquivoBinario){
    fseek(arquivoBinario, 0, SEEK_SET);
    fwrite(&cabecalho->status,           sizeof(cabecalho->status),         1, arquivoBinario);
    fwrite(&cabecalho->topo,             sizeof(cabecalho->topo),           1, arquivoBinario);
    fwrite(&cabecalho->proxByteOffset,   sizeof(cabecalho->proxByteOffset), 1, arquivoBinario);
    fwrite(&cabecalho->nroRegArq,        sizeof(cabecalho->nroRegArq),      1, arquivoBinario);
    fwrite(&cabecalho->nroRegRem,        sizeof(cabecalho->nroRegRem),      1, arquivoBinario);
}

// Função que escreve o registro no arquivo de dados
void EscritaRegistro(DADOS* registro, FILE* arquivoBinario){
    fwrite(&registro->removido,          sizeof(registro->removido),         1,                         arquivoBinario);
    fwrite(&registro->tamanhoRegistro,   sizeof(registro->tamanhoRegistro),  1,                         arquivoBinario);
    fwrite(&registro->prox,              sizeof(registro->prox),             1,                         arquivoBinario);
    fwrite(&registro->id,                sizeof(registro->id),               1,                         arquivoBinario);
    fwrite(&registro->idade,             sizeof(registro->idade),            1,                         arquivoBinario);
    fwrite(&registro->tamNomeJog,        sizeof(registro->tamNomeJog),       1,                         arquivoBinario);
    fwrite(&registro->nomeJogador,       sizeof(char),                      registro->tamNomeJog,       arquivoBinario);
    fwrite(&registro->tamNacionalidade,  sizeof(registro->tamNacionalidade), 1,                         arquivoBinario);
    fwrite(&registro->nacionalidade,     sizeof(char),                      registro->tamNacionalidade, arquivoBinario);
    fwrite(&registro->tamNomeClube,      sizeof(registro->tamNomeClube),     1,                         arquivoBinario);
    fwrite(&registro->nomeClube,         sizeof(char),                      registro->tamNomeClube,     arquivoBinario);
}

// Função que escreve o registro no arquivo de dados
void EscritaRegistroFixo(DADOS_FIXOS* registro, FILE* arquivoBinario){
    fwrite(&registro->removido,          sizeof(registro->removido),         1,                         arquivoBinario);
    fwrite(&registro->tamanhoRegistro,   sizeof(registro->tamanhoRegistro),  1,                         arquivoBinario);
    fwrite(&registro->prox,              sizeof(registro->prox),             1,                         arquivoBinario);
    fwrite(&registro->id,                sizeof(registro->id),               1,                         arquivoBinario);
    fwrite(&registro->idade,             sizeof(registro->idade),            1,                         arquivoBinario);
    fwrite(&registro->tamNomeJog,        sizeof(registro->tamNomeJog),       1,                         arquivoBinario);
    fwrite(&registro->nomeJogador,       sizeof(char),                      registro->tamNomeJog,       arquivoBinario);
    fwrite(&registro->tamNacionalidade,  sizeof(registro->tamNacionalidade), 1,                         arquivoBinario);
    fwrite(&registro->nacionalidade,     sizeof(char),                      registro->tamNacionalidade, arquivoBinario);
    fwrite(&registro->tamNomeClube,      sizeof(registro->tamNomeClube),     1,                         arquivoBinario);
    fwrite(&registro->nomeClube,         sizeof(char),                      registro->tamNomeClube,     arquivoBinario);
}

// Função que escreve o status do cabeçalho no arquivo binário
void writeStatusCabecalho(CABECALHO *cabecalho, FILE *arquivoBin) {
    const int statusByte = 0;
    fseek(arquivoBin, statusByte, SEEK_SET);
    char status = cabecalho->status;
    fwrite(&status, sizeof(char), 1, arquivoBin);
}

// Função que escreve o topo do cabeçalho no arquivo binário
void writeTopoCabecalho(CABECALHO *cabecalho, FILE *arquivoBin) {
    const int topoByte = 1;
    fseek(arquivoBin, topoByte, SEEK_SET);
    int64_t topo = cabecalho->topo;
    fwrite(&topo, sizeof(int64_t), 1, arquivoBin);
}

// Função que escreve o próximo byte offset do cabeçalho no arquivo binário
void writeProxByteOffsetCabecalho(CABECALHO *cabecalho, FILE *arquivoBin) {
    const int proxByteOffsetByte = 9;
    fseek(arquivoBin, proxByteOffsetByte, SEEK_SET);
    int64_t proxByteOffset = cabecalho->proxByteOffset;
    fwrite(&proxByteOffset, sizeof(int64_t), 1, arquivoBin);
}

void writeNroRegArqCabecalho(CABECALHO *cabecalho, FILE *arquivoBin) {
    const int nroRegArqByte = 17;
    fseek(arquivoBin, nroRegArqByte, SEEK_SET);
    int nroRegArq = cabecalho->nroRegArq;
    fwrite(&nroRegArq, sizeof(int), 1, arquivoBin);
}

// Função que escreve o número de registros removidos no cabeçalho do arquivo binário
void writeNroRegRemCabecalho(CABECALHO *cabecalho, FILE *arquivoBin) {
    const int nroRegRemByte = 21;
    fseek(arquivoBin, nroRegRemByte, SEEK_SET);
    int nroRem = cabecalho->nroRegRem;
    fwrite(&nroRem, sizeof(int), 1, arquivoBin);
}

// Função que escreve o registro de índice no arquivo de índices
void EscritaRegIndex(REGISTRO_INDEX* registro_index, FILE* nomeArquivoBinarioDeIndices) {
    fwrite(&registro_index->id, sizeof(registro_index->id), 1, nomeArquivoBinarioDeIndices);
    fwrite(&registro_index->byteOffset, sizeof(registro_index->byteOffset), 1, nomeArquivoBinarioDeIndices);
}

// Função que escreve o cabeçalho no arquivo de índices
void EscritaCabIndex(CABECALHO_INDEX* cabecalho_index, FILE* nomeArquivoBinarioDeIndices){
    fwrite(&cabecalho_index->status,           sizeof(cabecalho_index->status),         1, nomeArquivoBinarioDeIndices);
}