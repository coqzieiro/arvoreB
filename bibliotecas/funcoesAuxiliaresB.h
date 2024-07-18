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
    CABECALHO_DADOS *criarCabecalhoDados(); //ok
    CABECALHO_ARVORE_B *criarCabecalhoArvB(); //ok
    CABECALHO_DADOS *lerCabecalhoDados(FILE* arquivoBinario); //ok
    CABECALHO_ARVORE_B *lerCabecalhoArvoreB(FILE *arquivoBinario); //ok
    int escreverCabecalhoArvoreB(FILE *file, CABECALHO_ARVORE_B *cabecalho);
    int limpaCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);

    // FunçÕes auxiliares para registros árvore-B
    DADOS_ARVORE_B *criarRegistroArvoreBVazio();
    DADOS_ARVORE_B *lerRegistroArvoreB(FILE *arquivo, int rrn);
    DADOS **encontrar_registros_em_comum(DADOS **vetor1, DADOS **vetor2);
    DADOS *inicializa_registro();
    DADOS *leitura_registro_arquivoBin(int offset, FILE *arquivoBin);
    DADOS* atribui_valores_registro(char removido, int tamanhoRegistro, long prox, int id, int idade, int tamNomeJogador, char *nomeJogador, int tamNacionalidade, char *nacionalidade, int tamNomeClube, char *nomeClube);
    DADOS_INDICE *inicializa_registro_index();
    LISTA_INDICE *criarListaIndice();
    int apagarRegistroArvoreB(DADOS_ARVORE_B *registro);
    int escreverRegistroArvoreB(DADOS_ARVORE_B *registro, FILE *arquivo, int rrn);
    bool escreverRegistro(DADOS *registro, int byteOffset, int tamRegistroAtual, FILE *arquivoBin);
    bool apagarListaIndice(LISTA_INDICE *lista);
    void free_registro(DADOS *registro);
    
    // Funções auxiliares para busca árvore-B
    int64_t buscarRegistroIdRec(FILE *fileArvoreB, int id, int rrnAtual);
    int buscarPosicaoRegistroIndiceLinear(LISTA_INDICE *lista, int id);
    int imprimeRegistro(DADOS *registro);
    void imprimeRegistrosBuscados(char *arquivo, int buscaId, char *nomeArquivoArvoreB);
    void imprimirRegistrosPorCampos(FILE *file, CABECALHO_DADOS *cabecalho, int buscaId, char *nomeArquivoArvoreB, int i);

    // Funcões auxiliares para remoção árvore-B
    REMOVIDOS *criarListaRemovidos(FILE *file);
    REMOVIDOS *criarListaRemovidosVazia();
    int64_t getMaiorByteOffsetMenorQue(REMOVIDOS *removidos, int id);
    int64_t *getBestFitArrayRegistros(REMOVIDOS *removidos, DADOS **registros, int quantidade, FILE *file);
    int removerChaveRegistroArvoreB(DADOS_ARVORE_B *registro, int chave);
    int removerDescendenteRegistroArvoreB(DADOS_ARVORE_B *registro, int64_t descendente);
    void apagarListaRemovidos(REMOVIDOS *removidos);
    void removerRegistroIndice(LISTA_INDICE *lista, int index);
    void removerRegistroRemovidoPosicao(REMOVIDOS *removidos, int posicao);
    void adicionarRegistroRemovido(REMOVIDOS *removidos, DADOS_INDICE *registroIndice, int tamanho);
    void removerRegistroRemovidoEAtualizarArquivo(REMOVIDOS *removidos, int posicao, FILE *file);
    void shiftElementosListaRemovidosRight(REMOVIDOS *removidos, int pos);

    // Funções auxiliares para inserção árvore-B
    int64_t getBestFitAndFreeSpace(REMOVIDOS *removidos, int tamanho, DADOS *registro, FILE *file);
    int inserirChaveRegistroArvoreB(DADOS_ARVORE_B *registro, int chave, int64_t byteOffset);
    int inserirDescendenteRegistroArvoreB(DADOS_ARVORE_B *registro, int64_t descendente, int chaveDescendente);
    bool inserirNovoDadoArvoreB(char *arquivoBinario, char *arquivoArvoreB, int numOperacoes);
    bool adicionarNoArvoreB(int chave, int64_t byteOffset, FILE *arquivoArvoreB);
    void inserirArvoreB(FILE *arquivo, int chave, int64_t byteOffset);
    void insercaoArvoreBRecursiva(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, DADOS_ARVORE_B **caminho, int nivel, int *tamCaminho);

#endif