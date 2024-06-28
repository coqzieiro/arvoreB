/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#ifndef FUNCOES_H
    #define FUNCOES_H

    #define MAX_CAMPO 50
    #define TAM_INICIAL_BYTEOFFSET 25
    #define ORDEM_ARVORE_B 4
    #define TAMANHO_REGISTRO_ARVORE_B 60

    #include <stdint.h>
    #include <stdio.h>
    
    // Estrutura de cabeçalho para arquivos de dados
    typedef struct {
        char status;                 // Status do arquivo
        int64_t topo;                // Topo da lista de registros removidos
        int64_t proxByteOffset;      // Próximo byte offset disponível
        int nroRegArq;               // Número de registros no arquivo
        int nroRegRem;               // Número de registros removidos
    } CABECALHO;

    // Estrutura de dados para registros de variáveis
    typedef struct {
        char removido;               // Indicador de registro removido
        int tamanhoRegistro;         // Tamanho do registro
        int64_t prox;                // Próximo registro removido
        int id;                      // ID do registro
        int idade;                   // Idade do jogador
        int tamNomeJog;              // Tamanho do nome do jogador
        char* nomeJogador;           // Nome do jogador
        int tamNacionalidade;        // Tamanho da nacionalidade
        char* nacionalidade;         // Nacionalidade do jogador
        int tamNomeClube;            // Tamanho do nome do clube
        char* nomeClube;             // Nome do clube
    } DADOS;

    // Estrutura de dados para registros com campos fixos
    typedef struct {
        char removido;
        int tamanhoRegistro;
        int64_t prox;
        int id;
        int idade;
        int tamNomeJog;
        char nomeJogador[100];
        int tamNacionalidade;
        char nacionalidade[100];
        int tamNomeClube;
        char nomeClube[100];
    } DADOS_FIXOS;

    // Estrutura para armazenar informações de campos de busca
    typedef struct {
        char nomeCampo[MAX_CAMPO];   // Nome do campo
        int valorInt;                // Valor inteiro do campo
        char valorString[MAX_CAMPO]; // Valor string do campo
    } CAMPO_BUSCA;

    // Estrutura de cabeçalho para índices
    typedef struct {
        char status; // Status do índice
    } CABECALHO_INDEX;

    // Estrutura para registros de índice
    typedef struct {
        int id;                      // ID do registro
        int64_t byteOffset;          // Byte offset do registro
    } REGISTRO_INDEX;

    // Estrutura para listas encadeadas
    typedef struct LISTA {
        struct LISTA *prox;          // Próximo nó da lista
        int tamRegistro;             // Tamanho do registro
        int64_t byteOffset;          // Byte offset do registro
    } LISTA;

    // Estrutura de cabeçalho para árvores B
    typedef struct CABECALHO_ARVORE_B {
        char status;                 // Status da árvore
        int noRaiz;                  // RRN do nó raiz
        int proxRRN;                 // Próximo RRN disponível
        int nroChaves;               // Número de chaves na árvore
    } CABECALHO_ARVORE_B;

    // Estrutura para registros da árvore B
    typedef struct _registroArvoreB {
        int rrn;                     // RRN do registro
        int alturaNo;                // Altura do nó
        int nroChaves;               // Número de chaves no nó
        int chaves[ORDEM_ARVORE_B - 1]; // Chaves do nó
        long long int byteOffsets[ORDEM_ARVORE_B - 1]; // Byte offsets das chaves
        int descendentes[ORDEM_ARVORE_B]; // Descendentes do nó
    } REGISTRO_ARVORE_B;

    // Estrutura para registros de índice
    typedef struct REGISTRO_INDICE {
        int index;                   // Índice do registro
        long long int byteOffset;    // Byte offset do registro
    } REGISTRO_INDICE;

    // Estrutura para listas de índices
    typedef struct LISTA_INDICE {
        int tamanho;                 // Tamanho atual da lista
        int max_tamanho;             // Tamanho máximo da lista
        REGISTRO_INDICE **registros; // Vetor de ponteiros para registros de índice
    } LISTA_INDICE;

    // Estrutura para armazenar registros removidos
    typedef struct REMOVIDOS {
        LISTA_INDICE *lista;         // Lista de índices de registros removidos
        int *tamanhos;               // Tamanhos dos registros removidos
    } REMOVIDOS;

#endif