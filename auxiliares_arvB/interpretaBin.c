
/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/funcoesAuxiliaresB.h"

// Função para ler os valores do cabeçalho do arquivo binário e salvar na estrutura 'cabecalho'
void lerCabecalhoFromBin(FILE *file, CABECALHO *cabecalho) {
    char status;
    // Lê o status do cabeçalho do arquivo
    fread(&status, sizeof(char), 1, file);
    setStatus(cabecalho, status);

    int64_t topo;
    // Lê o topo da lista de registros removidos do cabeçalho do arquivo
    fread(&topo, sizeof(int64_t), 1, file);
    setTopo(cabecalho, topo);

    int64_t proxByteOffset;
    // Lê o próximo byte offset do cabeçalho do arquivo
    fread(&proxByteOffset, sizeof(int64_t), 1, file);
    setProxByteOffset(cabecalho, proxByteOffset);

    int nroRegArq;
    // Lê o número de registros não removidos do cabeçalho do arquivo
    fread(&nroRegArq, sizeof(int), 1, file);
    setNroRegArq(cabecalho, nroRegArq);

    int nroRem;
    // Lê o número de registros removidos do cabeçalho do arquivo
    fread(&nroRem, sizeof(int), 1, file);
    setNroRem(cabecalho, nroRem);
}

// Função para obter o cabeçalho do arquivo binário e salvar em uma estrutura do tipo 'CABECALHO'
CABECALHO *retornaCabecalhoBinario(FILE *file) {
    // Cria uma estrutura de cabeçalho e lê os valores do arquivo binário para ela
    CABECALHO *cabecalho = criarCabecalho();
    fseek(file, 0, SEEK_SET);
    lerCabecalhoFromBin(file, cabecalho);

    return cabecalho;
}