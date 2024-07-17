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
    void lerCabecalhoB(FILE *arquivo, CABECALHO *cabecalho);
    int64_t *getBestFitArrayRegistros(REMOVIDOS *removidos, DADOS **registros, int quantidade, FILE *file);
    void inserirArvoreB(FILE *arquivo, int chave, int64_t byteOffset);
    void insercaoArvoreBRecursiva(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, REGISTRO_ARVORE_B **caminho, int nivel, int *tamCaminho);
    CABECALHO *criarCabecalho(void);
    void writeStatusCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeTopoCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeProxByteOffsetCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeNroRegArqCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeNroRegRemCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    CABECALHO *retornaCabecalhoBinario(FILE *file);
    CABECALHO_ARVORE_B *criarCabecalhoArvoreBVazio();
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
    int getChave(REGISTRO_ARVORE_B *registro, int posicao);
    int64_t getByteOffsetRegistroArvoreB(REGISTRO_ARVORE_B *registro, int posicao);
    int getDescendente(REGISTRO_ARVORE_B *registro, int posicao);
    int apagarRegistroArvoreB(REGISTRO_ARVORE_B *registro);
    REGISTRO_ARVORE_B *lerRegistroArvoreB(FILE *arquivo, int rrn);
    int escreverRegistroArvoreB(REGISTRO_ARVORE_B *registro, FILE *arquivo, int rrn);

    // Funções get/set de registros árvore-B
    DADOS **encontrar_registros_em_comum(DADOS **vetor1, DADOS **vetor2);
    DADOS *inicializa_registro();
    DADOS* atribui_valores_registro(char removido, int tamanhoRegistro, long prox, int id, int idade, int tamNomeJogador, char *nomeJogador, int tamNacionalidade, char *nacionalidade, int tamNomeClube, char *nomeClube);
    void free_registro(DADOS *registro);
    DADOS *leitura_registro_arquivoBin(int offset, FILE *arquivoBin);

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
    REGISTRO_INDICE *inicializa_registro_index();
    void removerRegistroRemovidoEAtualizarArquivo(REMOVIDOS *removidos, int posicao, FILE *file);
    LISTA_INDICE *criarListaIndice();
    bool apagarListaIndice(LISTA_INDICE *lista);
    void adicionarRegistroRemovido(REMOVIDOS *removidos, REGISTRO_INDICE *registroIndice, int tamanho);
    int64_t getBestFitAndFreeSpace(REMOVIDOS *removidos, int tamanho, DADOS *registro, FILE *file);
    int64_t getMaiorByteOffsetMenorQue(REMOVIDOS *removidos, int id);
    void shiftElementosListaRemovidosRight(REMOVIDOS *removidos, int pos);
    int buscarPosicaoRegistroIndiceLinear(LISTA_INDICE *lista, int id);

#endif