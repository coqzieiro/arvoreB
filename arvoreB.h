#ifndef ARVOREB_H
    #define ARVOREB_H

    #include "registroArvB.h"
    #include "cabecalhoArvB.h"

    void imprimirArvoreBGraphviz(FILE *arquivo);
    void inserirArvoreB(FILE *arquivo, int chave, long long int byteOffset);
    void insercaoArvoreBRecursiva(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, REGISTRO_ARVORE_B **caminho, int nivel, int *tamCaminho);

#endif