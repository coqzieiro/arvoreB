/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#ifndef FUNCOES_AUX_H
    #define FUNCOES_AUX_H

    #include "funcionalidades.h"
    #include "definicoesTipos.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    // Leitura de cabeçalho e registros
    void LeituraCabecalho(CABECALHO* cabecalho, FILE* arquivoBinario);
    void LeituraRegistro(DADOS* registro, FILE* arquivoBinario);
    void LeituraRegistroFixo(DADOS_FIXOS* registro, FILE* arquivoBinario);

    // Escrita de cabeçalho e registros
    void EscritaCabecalho(CABECALHO* cabecalho, FILE* arquivoBinario);
    void EscritaRegistro(DADOS* registro, FILE* arquivoBinario);
    void EscritaRegistroFixo(DADOS_FIXOS* registro, FILE* arquivoBinario);
    
    // Correspondecia de campos
    int todosCamposCorrespondem(DADOS registro, CAMPO_BUSCA camposBusca[], int numCamposBusca);
    int validarNomeCampo(const char *nomeCampo);
    int todosCamposCorrespondemFixo(DADOS_FIXOS registro, CAMPO_BUSCA camposBusca[], int numCamposBusca);
    
    // Escrita no index
    void EscritaCabecalho_Index(CABECALHO_INDEX* cabecalho_index, FILE* nomeArquivoBinarioDeIndices);
    void EscritaRegistro_Index(REGISTRO_INDEX* registro_index, FILE* nomeArquivoBinarioDeIndices);

    // Funções auxiliares de remoção
    void InicializaRegistroJogador(DADOS* registro);
    void AlocaMemoriaRegistro(DADOS* registro);
    int LeituraRemocao(FILE* arquivoBinario, DADOS* registro);
    int64_t RetornaUltimoRemovido(FILE* arquivoBinario);
    void LiberaMemoriaChar(char** nomeCampo, char** valorCampo, int numCamposBusca);
    void DesalocaMemoriaReg(DADOS* registro);
    void LeituraParametrosBusca(int numCamposBusca, char** nomeCampo, char** valorCampo);

    // Funções auxiliares de inserção
    void AtualizaCampos(DADOS* registro);
    LISTA* OrdenaRegistrosRem(FILE* arquivoBinario);
    void InserirOrdenado(LISTA** cabeca, LISTA* novoNo);
    int64_t MetodoBestFit(LISTA** removidos, int tamRegistro, FILE* arquivoBinario);
    int ReescreveRemovidos(FILE* arquivoBinario, LISTA* removidos);
    void DesalocaLista(LISTA* cabeca);
    void LeituraCampos(DADOS* registro);
    int AtualizaTamanhoStrings(DADOS* registro);
    int* ReutilizarRegistro(FILE* arquivoBinario, CABECALHO* cabecalho, DADOS* registro, int MetodoBestFitOffset, LISTA* removidos);
    void EscreveRegistroInsercao(FILE* arquivoBinario, DADOS* registro);
    void EscreveLixo(FILE* arquivoBinario, int tamRegistroRemovido, int tamRegistroInserido);
    void IniCabecalho(CABECALHO* cabecalho);

    // Funções auxiliares do cabeçalho árvore-B
    CABECALHO *criarCabecalho(void);
    char getStatus(CABECALHO *cabecalho);
    long long int getTopo(CABECALHO *cabecalho);
    long long int getProxByteOffset(CABECALHO *cabecalho);
    int getNroRegArq(CABECALHO *cabecalho);
    int getNroRem(CABECALHO *cabecalho);
    void setStatus(CABECALHO *cabecalho, char status);
    void setTopo(CABECALHO *cabecalho, long long int topo);
    void setProxByteOffset(CABECALHO *cabecalho, long long int proxByteOffset);
    void setNroRegArq(CABECALHO *cabecalho, int nroRegArq);
    void setNroRem(CABECALHO *cabecalho, int nroRem);
    void apagarCabecalho(CABECALHO *cabecalho);
    void writeStatusCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeTopoCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeProxByteOffsetCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeNroRegArqCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    void writeNroRegRemCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);
    CABECALHO *getCabecalhoFromBin(FILE *file);
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
    int apagarCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);

    // Funções auxiliares manipulação de registros árvore-B
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
    REGISTRO_ARVORE_B *lerRegistroArvoreB(FILE *arquivo, int rrn);
    int escreverRegistroArvoreB(REGISTRO_ARVORE_B *registro, FILE *arquivo, int rrn);

    // Funções get/set de registros árvore-B
    void imprimirRegistros(DADOS **registros);
    DADOS **intersecaoDoisRegistros(DADOS **registro1, DADOS **registro2);
    DADOS *criarRegistroNulo();
    DADOS* criarRegistro(char removido, int tamanhoRegistro, long prox, int id, int idade, int tamNomeJogador, char *nomeJogador, int tamNacionalidade, char *nacionalidade, int tamNomeClube, char *nomeClube);
    char get_removido(DADOS *registro);
    int get_tamanhoRegistro(DADOS *registro);
    long long int get_prox(DADOS *registro);
    int get_id(DADOS *registro);
    int get_idade(DADOS *registro);
    int get_tamNomeJogador(DADOS *registro);
    DADOS *buscarRegistroOffset(long long offset, FILE *file);
    char* get_nomeJogador(DADOS *registro);
    int get_tamNacionalidade(DADOS *registro);
    char* get_nacionalidade(DADOS *registro);
    int get_tamNomeClube(DADOS *registro);
    char* get_nomeClube(DADOS *registro);
    void set_prox(DADOS *registro, long long int prox);
    void set_removido(DADOS *registro, int removido);
    void set_tamanhoRegistro(DADOS *registro, int tamanhoRegistro);
    void set_id(DADOS *registro, int id);
    void set_idade(DADOS *registro, int idade);
    void set_tamNomeJogador(DADOS *registro, int tamNomeJogador);
    void set_nomeJogador(DADOS *registro, char *nomeJogador);
    void set_tamNacionalidade(DADOS *registro, int tamNacionalidade);
    void set_nacionalidade(DADOS *registro, char *nacionalidade);
    void set_tamNomeClube(DADOS *registro, int tamNomeClube);
    void set_nomeClube(DADOS *registro, char *nomeClube);
    void liberarRegistro(DADOS *registro);
    DADOS *lerRegistroFromBin(int posicao, FILE *arquivoBin);

    // Funções auxiliares para escrita árvore-B
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