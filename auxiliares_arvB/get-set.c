/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/funcoesAuxiliaresB.h"

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
