/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#ifndef FUNCOES_AUX_B_H
    #define FUNCOES_AUX_B_H

    #include "funcionalidades.h"
    #include "definicoesTipos.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    // Funções auxiliares do cabeçalho árvore-B
    void lerCabecalhoFromBin(FILE *file, CABECALHO *cabecalho);
    int64_t *getBestFitArrayRegistros(REMOVIDOS *removidos, DADOS **registros, int quantidade, FILE *file);
    void inserirArvoreB(FILE *arquivo, int chave, int64_t byteOffset);
    void insercaoArvoreBRecursiva(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, REGISTRO_ARVORE_B **caminho, int nivel, int *tamCaminho);
    CABECALHO *criarCabecalho(void);
    char getStatus(CABECALHO *cabecalho);
    int64_t getTopo(CABECALHO *cabecalho);
    int64_t getProxByteOffset(CABECALHO *cabecalho);
    int getNroRegArq(CABECALHO *cabecalho);
    int getNroRem(CABECALHO *cabecalho);
    void setStatus(CABECALHO *cabecalho, char status);
    void setTopo(CABECALHO *cabecalho, int64_t topo);
    void setProxByteOffset(CABECALHO *cabecalho, int64_t proxByteOffset);
    void setNroRegArq(CABECALHO *cabecalho, int nroRegArq);
    void setNroRem(CABECALHO *cabecalho, int nroRem);
    void limpaCabecalho(CABECALHO *cabecalho);
    void writeStatusCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeTopoCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeProxByteOffsetCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeNroRegArqCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeNroRegRemCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    CABECALHO *retornaCabecalhoBinario(FILE *file);
    CABECALHO_ARVORE_B *criarCabecalhoArvoreBVazio();
    char getStatusCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);
    int getNoRaizCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);
    int getProxRRNCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);
    int getNroChavesCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);
    int setStatusCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, char status);
    int setNoRaizCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int noRaiz);
    int setProxRRNCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int proxRRN);
    int setNroChavesCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int nroChaves);
    CABECALHO_ARVORE_B *lerCabecalhoArvoreB(FILE *file);
    int escreverCabecalhoArvoreB(FILE *file, CABECALHO_ARVORE_B *cabecalho);
    int limpaCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);

    // Funções auxiliares manipulação de registros árvore-B
    REGISTRO_ARVORE_B *criarRegistroArvoreBVazio();
    int inserirChaveRegistroArvoreB(REGISTRO_ARVORE_B *registro, int chave, int64_t byteOffset);
    int removerChaveRegistroArvoreB(REGISTRO_ARVORE_B *registro, int chave);
    int inserirDescendenteRegistroArvoreB(REGISTRO_ARVORE_B *registro, int64_t descendente, int chaveDescendente);
    int removerDescendenteRegistroArvoreB(REGISTRO_ARVORE_B *registro, int64_t descendente);
    int getRRNRegistroArvoreB(REGISTRO_ARVORE_B *registro);
    int getAlturaNoRegistroArvoreB(REGISTRO_ARVORE_B *registro);
    int setRRNRegistroArvoreB(REGISTRO_ARVORE_B *registro, int rrn);
    int setAlturaNoRegistroArvoreB(REGISTRO_ARVORE_B *registro, int alturaNo);
    int getNroChavesRegistroArvoreB(REGISTRO_ARVORE_B *registro);
    int getChave(REGISTRO_ARVORE_B *registro, int posicao);
    int64_t getByteOffsetRegistroArvoreB(REGISTRO_ARVORE_B *registro, int posicao);
    int getDescendente(REGISTRO_ARVORE_B *registro, int posicao);
    int apagarRegistroArvoreB(REGISTRO_ARVORE_B *registro);
    REGISTRO_ARVORE_B *lerRegistroArvoreB(FILE *arquivo, int rrn);
    int escreverRegistroArvoreB(REGISTRO_ARVORE_B *registro, FILE *arquivo, int rrn);

    // Funções get/set de registros árvore-B
    void imprimirRegistros(DADOS **registros);
    DADOS **intersecaoDoisRegistros(DADOS **registro1, DADOS **registro2);
    DADOS *criarRegistroNulo();
    DADOS* criarRegistro(char removido, int tamanhoRegistro, long prox, int id, int idade, int tamNomeJogador, char *nomeJogador, int tamNacionalidade, char *nacionalidade, int tamNomeClube, char *nomeClube);
    char retornaRemovido(DADOS *registro);
    int retornaTamanhoRegistro(DADOS *registro);
    int64_t retornaProx(DADOS *registro);
    int retornaId(DADOS *registro);
    int retornaIdade(DADOS *registro);
    int RetornaTamNomeJogador(DADOS *registro);
    DADOS *buscaOfsset(long long offset, FILE *file);
    char* retornaNomeJogador(DADOS *registro);
    int retornaTamNacionalidade(DADOS *registro);
    char* retornaNacionalidade(DADOS *registro);
    int retornaTamNomeClube(DADOS *registro);
    char* retornaNomeClube(DADOS *registro);
    void setaProx(DADOS *registro, int64_t prox);
    void setaRemovido(DADOS *registro, int removido);
    void setaTamanhoRegistro(DADOS *registro, int tamanhoRegistro);
    void setaId(DADOS *registro, int id);
    void setaIdade(DADOS *registro, int idade);
    void setaTamNomeJogador(DADOS *registro, int tamNomeJogador);
    void setaNomeJogador(DADOS *registro, char *nomeJogador);
    void setaTamNacionalidade(DADOS *registro, int tamNacionalidade);
    void setaNacionalidade(DADOS *registro, char *nacionalidade);
    void setaTamNomeClube(DADOS *registro, int tamNomeClube);
    void setaNomeClube(DADOS *registro, char *nomeClube);
    void liberarRegistro(DADOS *registro);
    DADOS *lerRegistroFromBin(int posicao, FILE *arquivoBin);

    // Funções auxiliares para escrita árvore-B
    int imprimeRegistro(DADOS *registro);
    bool escreverRegistro(DADOS *registro, int byteOffset, int tamRegistroAtual, FILE *arquivoBin);
    void imprimeRegistrosBuscados(char *arquivo, int buscaId, char *nomeArquivoArvoreB);
    void imprimirRegistrosPorCampos(FILE *file, CABECALHO *cabecalho, int buscaId, char *nomeArquivoArvoreB, int i);
    int64_t buscarRegistroIdRec(FILE *fileArvoreB, int id, int rrnAtual);
    REMOVIDOS *criarListaRemovidos(FILE *file);
    bool inserirNovoDadoArvoreB(char *arquivoBinario, char *arquivoArvoreB, int numOperacoes);
    bool adicionarNoArvoreB(int chave, int64_t byteOffset, FILE *arquivoArvoreB);
    REMOVIDOS *criarListaRemovidosVazia();
    void apagarListaRemovidos(REMOVIDOS *removidos);
    void removerRegistroIndice(LISTA_INDICE *lista, int index);
    void removerRegistroRemovidoPosicao(REMOVIDOS *removidos, int posicao);
    REGISTRO_INDICE *criarRegistroIndice();
    void removerRegistroRemovidoEAtualizarArquivo(REMOVIDOS *removidos, int posicao, FILE *file);
    void setIndexRegistroIndice(REGISTRO_INDICE *registro, int index);
    void setByteOffsetRegistroIndice(REGISTRO_INDICE *registro, int64_t byteOffset);
    void setRegistroListaIndice(LISTA_INDICE *lista, int index, REGISTRO_INDICE *registro);
    void setTamanho(LISTA_INDICE *lista, int tamanho);
    LISTA_INDICE *criarListaIndice();
    bool apagarListaIndice(LISTA_INDICE *lista);
    void apagarRegistroIndice(REGISTRO_INDICE *registro);
    void adicionarRegistroRemovido(REMOVIDOS *removidos, REGISTRO_INDICE *registroIndice, int tamanho);
    int getTamanhoListaIndice(LISTA_INDICE *lista);
    int64_t getBestFitAndFreeSpace(REMOVIDOS *removidos, int tamanho, DADOS *registro, FILE *file);
    int64_t getMaiorByteOffsetMenorQue(REMOVIDOS *removidos, int id);
    void shiftElementosListaRemovidosRight(REMOVIDOS *removidos, int pos);
    REGISTRO_INDICE *getRegistroIndice(LISTA_INDICE *lista, int index);
    int buscarPosicaoRegistroIndiceLinear(LISTA_INDICE *lista, int id);
    int getIndexRegistroIndice(REGISTRO_INDICE *registro);
    int64_t getByteOffsetRegistroIndice(REGISTRO_INDICE *registro);
    int getTamanhoListaRemovidos(REMOVIDOS *removidos);

#endif