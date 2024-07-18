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
void EscritaCabecalho(CABECALHO_DADOS* cabecalho, FILE* arquivoBinario){
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

// Função que escreve o registro de índice no arquivo de índices
void EscritaRegIndex(DADOS_INDICE* registro_index, FILE* nomeArquivoBinarioDeIndices) {
    fwrite(&registro_index->index, sizeof(registro_index->index), 1, nomeArquivoBinarioDeIndices);
    fwrite(&registro_index->byteOffset, sizeof(registro_index->byteOffset), 1, nomeArquivoBinarioDeIndices);
}

// Função que escreve o cabeçalho no arquivo de índices
void EscritaCabIndex(CABECALHO_INDICE* cabecalho_index, FILE* nomeArquivoBinarioDeIndices){
    fwrite(&cabecalho_index->status,           sizeof(cabecalho_index->status),         1, nomeArquivoBinarioDeIndices);
}