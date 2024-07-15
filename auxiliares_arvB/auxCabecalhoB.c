/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/funcoesAuxiliaresB.h"

// Cria um cabeçalho de árvore B com valores padrão
CABECALHO_ARVORE_B *criarCabecalhoArvoreBVazio() {
    CABECALHO_ARVORE_B *cabecalho = malloc(sizeof(CABECALHO_ARVORE_B));
    cabecalho->status = '0';
    cabecalho->noRaiz = 0;
    cabecalho->proxRRN = 0;
    cabecalho->nroChaves = 0;

    return(cabecalho);
}

// Função que cria o cabeçalho e define os valores iniciais de seus campos
CABECALHO *criarCabecalho(void){
    CABECALHO *cabecalho = (CABECALHO*)malloc(sizeof(CABECALHO));
    cabecalho->status = '1';
    cabecalho->topo = -1;
    cabecalho->proxByteOffset = 0;
    cabecalho->nroRegArq = 0;
    cabecalho->nroRegRem = 0;

    return(cabecalho);
}

// Define o status do cabeçalho da árvore B
int setStatusCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, char status) {
    if (cabecalho == NULL) {
        return(0);
    }
    cabecalho->status = status;
    return(1);
}

// Define o RRN da raiz no cabeçalho da árvore B
int setNoRaizCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int noRaiz) {
    if (cabecalho == NULL) {
        return(0);
    }
    cabecalho->noRaiz = noRaiz;
    return(1);
}

// Define o próximo RRN no cabeçalho da árvore B
int setProxRRNCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int proxRRN) {
    if (cabecalho == NULL) {
        return(0);
    }
    cabecalho->proxRRN = proxRRN;
    return(1);
}

// Define o número de chaves no cabeçalho da árvore B
int setNroChavesCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int nroChaves) {
    if (cabecalho == NULL) {
        return(0);
    }
    cabecalho->nroChaves = nroChaves;
    return(1);
}

// Lê o cabeçalho da árvore B a partir de um arquivo
CABECALHO_ARVORE_B *lerCabecalhoArvoreB(FILE *file) {
    if (file == NULL) {
        return(NULL);
    }
    CABECALHO_ARVORE_B *cabecalho = malloc(sizeof(CABECALHO_ARVORE_B));

    fseek(file, 0, SEEK_SET);
    fread(&cabecalho->status, sizeof(char), 1, file);
    fread(&cabecalho->noRaiz, sizeof(int), 1, file);
    fread(&cabecalho->proxRRN, sizeof(int), 1, file);
    fread(&cabecalho->nroChaves, sizeof(int), 1, file);

    return(cabecalho);
}

// Escreve o cabeçalho da árvore B em um arquivo
int escreverCabecalhoArvoreB(FILE *file, CABECALHO_ARVORE_B *cabecalho) {
    if (file == NULL || cabecalho == NULL) {
        return(0);
    }
    fseek(file, 0, SEEK_SET);
    int tamanhoCabecalho = sizeof(char) + sizeof(int) * 3;

    fwrite(&cabecalho->status, sizeof(char), 1, file);
    fwrite(&cabecalho->noRaiz, sizeof(int), 1, file);
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, file);
    fwrite(&cabecalho->nroChaves, sizeof(int), 1, file);

    char lixo = '$';
    for (int i = tamanhoCabecalho; i < TAMANHO_REGISTRO_ARVORE_B; i++) {
        fwrite(&lixo, sizeof(char), 1, file); // Preenche com lixo para manter o tamanho do registro
    }
    return(1);
}

// Libera a memória alocada para o cabeçalho da árvore B
int limpaCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho) {
    if (cabecalho == NULL){
        return(0);
    }
    free(cabecalho);
    return(1);
}

// Imprime os valores do cabeçalho da árvore B
int imprimirCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho) {
    if (cabecalho == NULL) {
        return (0);
    }
    printf("Status: %c\n", cabecalho->status);
    printf("No raiz: %d\n", cabecalho->noRaiz);
    printf("Prox RRN: %d\n", cabecalho->proxRRN);
    printf("Nro chaves: %d\n", cabecalho->nroChaves);
    return(1);
}

// Função para ler os valores do cabeçalho do arquivo binário e salvar na estrutura 'cabecalho'
void lerCabecalhoB(FILE *file, CABECALHO *cabecalho) {
    char status;
    // Lê o status do cabeçalho do arquivo
    fread(&status, sizeof(char), 1, file);
    cabecalho->status = status;

    int64_t topo;
    // Lê o topo da lista de registros removidos do cabeçalho do arquivo
    fread(&topo, sizeof(int64_t), 1, file);
    
    cabecalho->topo = topo;

    int64_t proxByteOffset;
    // Lê o próximo byte offset do cabeçalho do arquivo
    fread(&proxByteOffset, sizeof(int64_t), 1, file);

    cabecalho->proxByteOffset = proxByteOffset;

    int nroRegArq;
    // Lê o número de registros não removidos do cabeçalho do arquivo
    fread(&nroRegArq, sizeof(int), 1, file);
    
    cabecalho->nroRegArq = nroRegArq;

    int nroRem;
    // Lê o número de registros removidos do cabeçalho do arquivo
    fread(&nroRem, sizeof(int), 1, file);
    cabecalho->nroRegRem = nroRem;
}

// Função para obter o cabeçalho do arquivo binário e salvar em uma estrutura do tipo 'CABECALHO'
CABECALHO *retornaCabecalhoBinario(FILE *file) {
    // Cria uma estrutura de cabeçalho e lê os valores do arquivo binário para ela
    CABECALHO *cabecalho = criarCabecalho();
    fseek(file, 0, SEEK_SET);
    lerCabecalhoB(file, cabecalho);

    return cabecalho;
}
