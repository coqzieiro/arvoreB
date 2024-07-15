/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bibliotecas/funcoesAuxiliares.h"
#include "../bibliotecas/funcoesAuxiliaresB.h"

// Função que encontra registros em comum entre dois vetores de registros.
DADOS **encontrar_registros_em_comum(DADOS **vetor1, DADOS **vetor2){
    int qtd_registros = 0;
    int i, j;
    DADOS **resultado = (DADOS **)malloc(0);

    for (i = 0; vetor1[i] != NULL; i++){
        for (j = 0; vetor2[j] != NULL; j++){
            if ( vetor1[i]->id ==  vetor2[j]->id){
                resultado = (DADOS **)realloc(resultado, sizeof(DADOS *) * (qtd_registros + 1));
                resultado[qtd_registros] = vetor1[i];
                qtd_registros++;
            }
        }
    }

    resultado = (DADOS **)realloc(resultado, sizeof(DADOS *) * (qtd_registros + 1));
    resultado[qtd_registros] = NULL;

    return resultado;
}

// função que inicializa o registro
DADOS *inicializa_registro(){
    DADOS *registro = (DADOS *)malloc(sizeof(DADOS));

    registro->removido = '1';
    registro->tamanhoRegistro = 0;
    registro->prox = -1;
    registro->id = -1;
    registro->idade = -1;

    registro->tamNomeJog = -1;
    registro->nomeJogador = NULL;

    registro->tamNacionalidade = -1;
    registro->nacionalidade = NULL;

    registro->tamNomeClube = -1;
    registro->nomeClube = NULL;

    return registro;
}

// função que atribui os valores passados na função a um registro
DADOS *atribui_valores_registro(char removido, int tamanhoRegistro, long prox, int id, int idade, int tamNomeJogador, char *nomeJogador, int tamNacionalidade, char *nacionalidade, int tamNomeClube, char *nomeClube) {
    DADOS *registro = (DADOS *)malloc(sizeof(DADOS));
    
    registro->removido = removido;
    registro->tamanhoRegistro = tamanhoRegistro;
    registro->prox = prox;
    registro->id = id;
    registro->idade = idade;

    registro->tamNomeJog = tamNomeJogador;
    registro->nomeJogador = (nomeJogador != NULL) ? strdup(nomeJogador) : NULL;

    registro->tamNacionalidade = tamNacionalidade;
    registro->nacionalidade = (nacionalidade != NULL) ? strdup(nacionalidade) : NULL;

    registro->tamNomeClube = tamNomeClube;
    registro->nomeClube = (nomeClube != NULL) ? strdup(nomeClube) : NULL;

    return registro;
}

// desaloca memória do registro
void free_registro(DADOS *registro) {
    if (registro->nomeJogador)   free(registro->nomeJogador);
    if (registro->nacionalidade) free(registro->nacionalidade);
    if (registro->nomeClube)     free(registro->nomeClube);
    
    free(registro);
}

// função que lê um retorna um registro a partir de uma posição do arquivo binário
DADOS *leitura_registro_arquivoBin(int offset, FILE *binFile) {
    // inicializa um registro
    DADOS *registro = inicializa_registro();

    // vai para a posição indicada do arquivo binário 
    fseek(binFile, offset, SEEK_SET);

    // faz a leitura do campo "removido" do registro no arquivo, e salva na variável registro inicializada no início
    char removido;
    fread(&removido, sizeof(char), 1, binFile); 
    registro->removido = removido;

    // faz a leitura do campo "tamanhoRegistro" do registro no arquivo, e salva na variável registro inicializada no início
    int tamanhoRegistro;
    fread(&tamanhoRegistro, sizeof(int), 1, binFile);
    registro->tamanhoRegistro = tamanhoRegistro;

    // faz a leitura do campo "prox" do registro no arquivo, e salva na variável registro inicializada no início
    int64_t prox;
    fread(&prox, sizeof(int64_t), 1, binFile);
    registro->prox = prox;

    // faz a leitura do campo "id" do registro no arquivo, e salva na variável registro inicializada no início
    int id;
    fread(&id, sizeof(int), 1, binFile);
    registro->id = id;

    // faz a leitura do campo "idade" do registro no arquivo, e salva na variável registro inicializada no início
    int idade;
    fread(&idade, sizeof(int), 1, binFile);

    if(idade == 0) 
        registro->idade = -1;
    else 
        registro->idade = idade;

    // faz a leitura do campo "tamNomeJogador" do registro no arquivo, e salva na variável registro inicializada no início
    int tamNomeJogador;
    fread(&tamNomeJogador, sizeof(int), 1, binFile);
    registro->tamNomeJog = tamNomeJogador;

    // faz a leitura do campo "nomeJogador" do registro no arquivo, e salva na variável registro inicializada no início
    if (tamNomeJogador > 0) {
        char *nomeJogador = (char *)malloc(tamNomeJogador + 1);
        fread(nomeJogador, sizeof(char), tamNomeJogador, binFile);
        nomeJogador[tamNomeJogador] = '\0';
        registro->nomeJogador = nomeJogador;
    }
    else registro->nomeJogador = NULL;

    // faz a leitura do campo "tamNacionalidade" do registro no arquivo, e salva na variável registro inicializada no início
    int tamNacionalidade;
    fread(&tamNacionalidade, sizeof(int), 1, binFile);
    registro->tamNacionalidade = tamNacionalidade;

    // faz a leitura do campo "nacionalidade" do registro no arquivo, e salva na variável registro inicializada no início
    if (tamNacionalidade > 0) {
        char *nacionalidade = (char *)malloc(tamNacionalidade + 1);
        fread(nacionalidade, sizeof(char), tamNacionalidade, binFile);
        nacionalidade[tamNacionalidade] = '\0'; 
        registro->nacionalidade = nacionalidade;
    }
    else registro->nacionalidade = NULL;

    // faz a leitura do campo "tamNomeClube" do registro no arquivo, e salva na variável registro inicializada no início
    int tamNomeClube;
    fread(&tamNomeClube, sizeof(int), 1, binFile);
    registro->tamNomeClube = tamNomeClube;

    // faz a leitura do campo "nomeClube" do registro no arquivo, e salva na variável registro inicializada no início
    if (tamNomeClube > 0) {
        char *nomeClube = (char *)malloc(tamNomeClube + 1);
        fread(nomeClube, sizeof(char), tamNomeClube, binFile);
        nomeClube[tamNomeClube] = '\0';
        registro->nomeClube = nomeClube;
    }
    else registro->nomeClube = NULL;

    return registro;
}