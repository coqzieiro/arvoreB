#ifndef REGISTROARVOREB_H
    #define REGISTROARVOREB_H

    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include "definicoesTipos.h"

    #define ORDEM_ARVORE_B 4
    
    typedef struct _registroArvoreB REGISTRO_ARVORE_B;

    #define TAMANHO_REGISTRO_ARVORE_B 60

    REGISTRO_ARVORE_B *criarRegistroArvoreBVazio();
    int inserirChaveRegistroArvoreB(REGISTRO_ARVORE_B *registro, int chave, long long int byteOffset);
    int removerChaveRegistroArvoreB(REGISTRO_ARVORE_B *registro, int chave);
    int inserirDescendenteRegistroArvoreB(REGISTRO_ARVORE_B *registro, long long int descendente, int chaveDescendente);
    int removerDescendenteRegistroArvoreB(REGISTRO_ARVORE_B *registro, long long int descendente);
    int getRRNRegistroArvoreB(REGISTRO_ARVORE_B *registro);
    int getAlturaNoRegistroArvoreB(REGISTRO_ARVORE_B *registro);
    int setRRNRegistroArvoreB(REGISTRO_ARVORE_B *registro, int rrn);
    int setAlturaNoRegistroArvoreB(REGISTRO_ARVORE_B *registro, int alturaNo);
    int getNroChavesRegistroArvoreB(REGISTRO_ARVORE_B *registro);
    int getChave(REGISTRO_ARVORE_B *registro, int posicao);
    long long int getByteOffsetRegistroArvoreB(REGISTRO_ARVORE_B *registro, int posicao);
    int getDescendente(REGISTRO_ARVORE_B *registro, int posicao);
    int apagarRegistroArvoreB(REGISTRO_ARVORE_B *registro);
    int imprimirRegistroArvoreB(REGISTRO_ARVORE_B *registro);
    REGISTRO_ARVORE_B *lerRegistroArvoreB(FILE *arquivo, int rrn);
    int escreverRegistroArvoreB(REGISTRO_ARVORE_B *registro, FILE *arquivo, int rrn);

#endif