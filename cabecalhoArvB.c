/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "funcoesAuxiliares.h"

// Cria um cabeçalho de árvore B com valores padrão
CABECALHO_ARVORE_B *criarCabecalhoArvoreBVazio() {
    CABECALHO_ARVORE_B *cabecalho = malloc(sizeof(CABECALHO_ARVORE_B));
    cabecalho->status = '0';
    cabecalho->noRaiz = 0;
    cabecalho->proxRRN = 0;
    cabecalho->nroChaves = 0;

    return(cabecalho);
}

// Obtém o status do cabeçalho da árvore B
char getStatusCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho) {
    if (cabecalho == NULL) {
        return('$');
    }
    return(cabecalho->status);
}

// Obtém o RRN da raiz do cabeçalho da árvore B
int getNoRaizCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho) {
    if (cabecalho == NULL) {
        return(-1);
    }

    return(cabecalho->noRaiz);
}

// Obtém o próximo RRN do cabeçalho da árvore B
int getProxRRNCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho) {
    if (cabecalho == NULL) {
        return(-1);
    }

    return(cabecalho->proxRRN);
}

// Obtém o número de chaves do cabeçalho da árvore B
int getNroChavesCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho) {
    if (cabecalho == NULL) {
        return(-1);
    }
    return(cabecalho->nroChaves);
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