/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/funcoesAuxiliaresB.h"

// Inicializa um cabeçalho de árvore B com valores padrão
CABECALHO_ARVORE_B *criarCabecalhoArvB() {
    CABECALHO_ARVORE_B *cabecalho = malloc(sizeof(CABECALHO_ARVORE_B));
    cabecalho->status = '0'; // Status inativo
    cabecalho->noRaiz = 0; // Raiz inicialmente inexistente
    cabecalho->proxRRN = 0; // Primeiro RRN disponível
    cabecalho->nroChaves = 0; // Sem chaves inicialmente

    return cabecalho;
}

// Gera um cabeçalho genérico com valores iniciais pré-definidos
CABECALHO_DADOS *criarCabecalhoDados(){
    CABECALHO_DADOS *cabecalho = (CABECALHO_DADOS*)malloc(sizeof(CABECALHO_DADOS));
    cabecalho->status = '1'; // Status ativo
    cabecalho->topo = -1; // Topo da pilha de registros removidos
    cabecalho->proxByteOffset = 0; // Posição inicial para novos registros
    cabecalho->nroRegArq = 0; // Contador de registros não removidos
    cabecalho->nroRegRem = 0; // Contador de registros removidos

    return cabecalho;
}

// Carrega os dados do cabeçalho da árvore B de um arquivo
CABECALHO_ARVORE_B *lerCabecalhoArvB(FILE *arquivoBinario) {
    if (arquivoBinario == NULL) {
        return NULL;
    }
    CABECALHO_ARVORE_B *cabecalho = malloc(sizeof(CABECALHO_ARVORE_B));

    fseek(arquivoBinario, 0, SEEK_SET);
    fread(&cabecalho->status, sizeof(char), 1, arquivoBinario);
    fread(&cabecalho->noRaiz, sizeof(int), 1, arquivoBinario);
    fread(&cabecalho->proxRRN, sizeof(int), 1, arquivoBinario);
    fread(&cabecalho->nroChaves, sizeof(int), 1, arquivoBinario);

    return cabecalho;
}

// Atualiza o cabeçalho da árvore B no arquivo
void escreverCabecalhoArvB(FILE *arquivoBinario, CABECALHO_ARVORE_B *cabecalho) {
    if (arquivoBinario == NULL || cabecalho == NULL) {
        return;
    }
    fseek(arquivoBinario, 0, SEEK_SET);
    int tamanhoCabecalho = sizeof(char) + sizeof(int) * 3;

    fwrite(&cabecalho->status, sizeof(char), 1, arquivoBinario);
    fwrite(&cabecalho->noRaiz, sizeof(int), 1, arquivoBinario);
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, arquivoBinario);
    fwrite(&cabecalho->nroChaves, sizeof(int), 1, arquivoBinario);

    char lixo = '$';
    for (int i = tamanhoCabecalho; i < TAMANHO_DADOS_ARVORE_B; i++) {
        fwrite(&lixo, sizeof(char), 1, arquivoBinario); // Completa com lixo para uniformidade
    }
    return;
}

// Retorna a estrutura CABECALHO preenchida com dados lidos do arquivo binário
CABECALHO_DADOS *lerCabecalhoDados(FILE* arquivoBinario) {
    CABECALHO_DADOS *cabecalho = criarCabecalhoDados(); // Cria a estrutura de cabeçalho
    fseek(arquivoBinario, 0, SEEK_SET);

    char status;
    fread(&status, sizeof(char), 1, arquivoBinario); // Lê o status atual do cabeçalho
    cabecalho->status = status;

    int64_t topo;
    fread(&topo, sizeof(int64_t), 1, arquivoBinario); // Lê a posição do topo da pilha de registros removidos
    cabecalho->topo = topo;

    int64_t proxByteOffset;
    fread(&proxByteOffset, sizeof(int64_t), 1, arquivoBinario); // Lê o próximo byte offset para escrita
    cabecalho->proxByteOffset = proxByteOffset;

    int nroRegArq;
    fread(&nroRegArq, sizeof(int), 1, arquivoBinario); // Lê o número de registros arquivados
    cabecalho->nroRegArq = nroRegArq;

    int nroRem;
    fread(&nroRem, sizeof(int), 1, arquivoBinario); // Lê o número de registros removidos
    cabecalho->nroRegRem = nroRem;

    return(cabecalho);
}
