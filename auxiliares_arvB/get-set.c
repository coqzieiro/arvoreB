/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/funcoesAuxiliaresB.h"

// Função para obter a altura de um nó em um registro de árvore B
int getAlturaNoRegistroArvoreB(REGISTRO_ARVORE_B *registro) {
    if (registro == NULL) {
        return -1;
    }
    
    return registro->alturaNo;
}

// Função para definir o RRN de um registro de árvore B
int setRRNRegistroArvoreB(REGISTRO_ARVORE_B *registro, int rrn) {
    if (registro == NULL) {
        return 0;
    }
    
    registro->rrn = rrn;
    return 1;
}

// Função para definir a altura de um nó em um registro de árvore B
int setAlturaNoRegistroArvoreB(REGISTRO_ARVORE_B *registro, int alturaNo) {
    if (registro == NULL) {
        return 0;
    }
    
    registro->alturaNo = alturaNo;
    return 1;
}

// Função para obter o número de chaves em um registro de árvore B
int getNroChavesRegistroArvoreB(REGISTRO_ARVORE_B *registro) {
    if (registro == NULL) {
        return -1;
    }
    
    return registro->nroChaves;
}

// Função para obter uma chave em uma posição específica de um registro de árvore B
int getChave(REGISTRO_ARVORE_B *registro, int posicao) {
    if (registro == NULL || posicao < 0 || posicao >= ORDEM_ARVORE_B - 1) {
        return -1;
    }

    return registro->chaves[posicao];
}

// Função para obter o byte offset de uma chave em uma posição específica de um registro de árvore B
int64_t getByteOffsetRegistroArvoreB(REGISTRO_ARVORE_B *registro, int posicao) {
    if (registro == NULL || posicao < 0 || posicao >= ORDEM_ARVORE_B - 1) {
        return -1;
    }

    return registro->byteOffsets[posicao];
}

// Função para obter um descendente em uma posição específica de um registro de árvore B
int getDescendente(REGISTRO_ARVORE_B *registro, int posicao) {
    if (registro == NULL || posicao < 0 || posicao >= ORDEM_ARVORE_B) {
        return -1;
    }

    return registro->descendentes[posicao];
}
