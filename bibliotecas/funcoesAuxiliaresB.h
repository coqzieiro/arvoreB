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
    CABECALHO_DADOS *criarCabecalhoDados();
    CABECALHO_ARVORE_B *criarCabecalhoArvB();
    CABECALHO_DADOS *lerCabecalhoDados(FILE* arquivoBinario);
    CABECALHO_ARVORE_B *lerCabecalhoArvB(FILE *arquivoBinario);
    void escreverCabecalhoArvB(FILE *arquivoBinario, CABECALHO_ARVORE_B *cabecalho);

    // FunçÕes auxiliares para registros árvore-B
    DADOS_ARVORE_B *inicializaRegistroArvB();
    DADOS_ARVORE_B *lerRegistroArvB(FILE *arquivoBinario, int rrn);
    DADOS **registrosIguaisArvB(DADOS **registro1, DADOS **registro2);
    DADOS *inicializaRegistroDados(); 
    DADOS *lerRegistroBinario(int offset, FILE *arquivoBinario);
    DADOS* preencheRegistroDdos(char removido, int tamanhoRegistro, int64_t prox, int id, int idade, int tamNomeJogador, char *nomeJogador, int tamNacionalidade, char *nacionalidade, int tamNomeClube, char *nomeClube);
    DADOS_INDICE *inicializaRegistroIndex();
    LISTA_INDICE *criarListaIndex();
    int escreverRegistroArvB(DADOS_ARVORE_B *registro, FILE *arquivoBinario, int rrn);
    int escreverRegistroDados(DADOS *registro, int byteOffset, int tamRegistroAtual, FILE *arquivoBinario);
    bool apagarListaIndice(LISTA_INDICE *lista);
    void apagarRegistroArvB(DADOS_ARVORE_B *registro);
    void liberaRegistroDados(DADOS *registro);
    
    // Funções auxiliares para busca árvore-B
    int64_t buscarRegistroRecursivo(FILE *arquivoBinario, int id, int rrnAtual);
    int buscarPosicaoRegistro(LISTA_INDICE *lista, int id);
    void imprimirRegistrosDados(DADOS *registro);
    void recuperarRegistros(char *nomeArquivoBinario, int buscaId, char *nomeArquivoArvoreB);
    void imprimirRegistrosCampos(FILE *arquivoBinario, CABECALHO_DADOS *cabecalho, int buscaId, char *nomeArquivoArvoreB, int i);

    // Funcões auxiliares para remoção árvore-B
    REMOVIDOS *criarListaRemovidos(FILE *arquivoBinario);
    REMOVIDOS *inicializaListaRemovidos();
    int64_t *bestFitRegistros(REMOVIDOS *removidos, DADOS **registros, int quantidade, FILE *arquivoBinario);
    void apagarListaRemovidos(REMOVIDOS *removidos);
    void removerRegistroIndice(LISTA_INDICE *lista, int index);
    void removerRegistroRemovidoPosicao(REMOVIDOS *removidos, int posicao);
    void adicionarRegistroRemovido(REMOVIDOS *removidos, DADOS_INDICE *registroIndice, int tamanho);
    void atualizarRegistroRemovido(REMOVIDOS *removidos, int posicao, FILE *arquivoBinario);
    //void shiftRegistrosRemovidos(REMOVIDOS *removidos, int pos);

    // Funções auxiliares para inserção árvore-B
    int64_t bestFitLiberarMemoria(REMOVIDOS *removidos, int tamanho, DADOS *registro, FILE *arquivoBinario);
    int inserirChaveRegistroArvB(DADOS_ARVORE_B *registro, int chave, int64_t byteOffset);
    int inserirFilhoRegistroArvB(DADOS_ARVORE_B *registro, int64_t descendente, int chaveDescendente);
    void inserirArvoreB(FILE *arquivoBinario, int chave, int64_t byteOffset);
    void insercaoRecursivaArvB(FILE *arquivoBinario, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, DADOS_ARVORE_B **caminho, int nivel, int *tamCaminho);

#endif