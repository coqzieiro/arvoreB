#ifndef ESCREVE_BIN_H
    #define ESCREVE_BIN_H
    #include "definicoesTipos.h"
    #include "funcoes_fornecidas.h"
    #include <stdbool.h>

    typedef struct REGISTRO_INDICE {
        int index;
        long long int byteOffset;
    } REGISTRO_INDICE;

    typedef struct LISTA_INDICE {
        int tamanho;
        int max_tamanho;
        REGISTRO_INDICE **registros; // a lista possui um vetor de endereços de registros
    } LISTA_INDICE;

    typedef struct REMOVIDOS {
        LISTA_INDICE *lista;
        int *tamanhos;
    } REMOVIDOS;

    int imprimeRegistro(DADOS *registro);
    bool escreverRegistro(DADOS *registro, int byteOffset, int tamRegistroAtual, FILE *arquivoBin);
    void imprimeRegistrosBuscados(char *arquivo, int buscaId, char *nomeArquivoArvoreB);
    void imprimirRegistrosPorCampos(FILE *file, CABECALHO *cabecalho, int buscaId, char *nomeArquivoArvoreB, int i);
    long long int buscarRegistroIdRec(FILE *fileArvoreB, int id, int rrnAtual);
    REMOVIDOS *criarListaRemovidos(FILE *file);
    void imprimirIdArvoreB(int id, FILE *file, char *nomeArquivoArvoreB, int i, int buscaMinuscula);
    int criarArquivoArvoreB(char *arquivoBin, char *arquivoArvB);
    bool inserirNovoDadoArvoreB(char *arquivoBinario, char *arquivoArvoreB, int numOperacoes);
    bool adicionarNoArvoreB(int chave, long long int byteOffset, FILE *arquivoArvoreB);
    REMOVIDOS *criarListaRemovidosVazia();
    void apagarListaRemovidos(REMOVIDOS *removidos);
    void removerRegistroIndice(LISTA_INDICE *lista, int index);
    void removerRegistroRemovidoPosicao(REMOVIDOS *removidos, int posicao);
    REGISTRO_INDICE *criarRegistroIndice();
    void removerRegistroRemovidoEAtualizarArquivo(REMOVIDOS *removidos, int posicao, FILE *file);
    void setIndexRegistroIndice(REGISTRO_INDICE *registro, int index);
    void setByteOffsetRegistroIndice(REGISTRO_INDICE *registro, long long int byteOffset);
    void setRegistroListaIndice(LISTA_INDICE *lista, int index, REGISTRO_INDICE *registro);
    void setTamanho(LISTA_INDICE *lista, int tamanho);
    LISTA_INDICE *criarListaIndice();
    bool apagarListaIndice(LISTA_INDICE *lista);
    void apagarRegistroIndice(REGISTRO_INDICE *registro);
    void adicionarRegistroRemovido(REMOVIDOS *removidos, REGISTRO_INDICE *registroIndice, int tamanho);
    int getTamanhoListaIndice(LISTA_INDICE *lista);
    long long int getBestFitAndFreeSpace(REMOVIDOS *removidos, int tamanho, DADOS *registro, FILE *file);
    long long int getMaiorByteOffsetMenorQue(REMOVIDOS *removidos, int id);
    void shiftElementosListaRemovidosRight(REMOVIDOS *removidos, int pos);
    REGISTRO_INDICE *getRegistroIndice(LISTA_INDICE *lista, int index);
    int buscarPosicaoRegistroIndiceLinear(LISTA_INDICE *lista, int id);
    int getIndexRegistroIndice(REGISTRO_INDICE *registro);
    long long int getByteOffsetRegistroIndice(REGISTRO_INDICE *registro);
    int getTamanhoListaRemovidos(REMOVIDOS *removidos);

#endif