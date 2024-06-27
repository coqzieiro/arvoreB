 #ifndef REGISTRO_H
    #define REGISTRO_H
    #include "definicoesTipos.h"
    #include <stdio.h>
    #include <stdlib.h>

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
#endif