#ifndef ESCREVE_BIN_H
    #define ESCREVE_BIN_H
    #include "definicoesTipos.h"


    // void imprimeRegistrosFromBin(char *filePath);
    int imprimeRegistro(DADOS *registro);
    // void imprimeRegistrosBuscados(char *arquivo, int buscaId, char *nomeArquivoArvoreB);
    // void imprimirRegistrosPorCampos(FILE *file, CABECALHO *cabecalho, int buscaId, char *nomeArquivoArvoreB, int i);
    long long int buscarRegistroIdRec(FILE *fileArvoreB, int id, int rrnAtual);
    // void removerRegistrosBuscados(char *arquivoBin, char *arquivoIndice);
    // void removeById(int id, LISTA_INDICE *listaIndices, FILE *file, FILE *fileIndices, REMOVIDOS *listaRemovidos, CABECALHO *cabecalho, char* arquivoIndice);
    // bool escreverRegistro(REGISTRO *registro, int byteOffset, int tamRegistroAtual, FILE *arquivoBin);

    void imprimirIdArvoreB(int id, FILE *file, char *nomeArquivoArvoreB, int i, int buscaMinuscula);
    int criarArquivoArvoreB(char *arquivoBin, char *arquivoArvB);
#endif