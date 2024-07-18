/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/funcoesAuxiliaresB.h"

// Função para inicializar um cabeçalho de árvore B com valores padrão
CABECALHO_ARVORE_B *criarCabecalhoArvB() {
    CABECALHO_ARVORE_B *cabecalho = malloc(sizeof(CABECALHO_ARVORE_B)); // Aloca memória para o cabeçalho
    cabecalho->status = '0'; // Define o status como inativo
    cabecalho->noRaiz = 0; // Inicializa o número do nó raiz como inexistente
    cabecalho->proxRRN = 0; // Define o próximo RRN disponível como 0
    cabecalho->nroChaves = 0; // Inicializa o número de chaves como 0

    return cabecalho; // Retorna o cabeçalho criado
}

// Função para gerar um cabeçalho de dados com valores iniciais pré-definidos
CABECALHO_DADOS *criarCabecalhoDados(){
    CABECALHO_DADOS *cabecalho = (CABECALHO_DADOS*)malloc(sizeof(CABECALHO_DADOS)); // Aloca memória para o cabeçalho de dados
    cabecalho->status = '1'; // Define o status como ativo
    cabecalho->topo = -1; // Inicializa o topo da pilha de registros removidos como -1
    cabecalho->proxByteOffset = 0; // Define o próximo byte offset inicial como 0
    cabecalho->nroRegArq = 0; // Inicializa o número de registros arquivados como 0
    cabecalho->nroRegRem = 0; // Inicializa o número de registros removidos como 0

    return cabecalho; // Retorna o cabeçalho de dados criado
}

// Função para carregar os dados do cabeçalho da árvore B de um arquivo
CABECALHO_ARVORE_B *lerCabecalhoArvB(FILE *arquivoBinario) {
    if (arquivoBinario == NULL) {
        return NULL; // Verifica se o arquivo está aberto corretamente
    }
    CABECALHO_ARVORE_B *cabecalho = malloc(sizeof(CABECALHO_ARVORE_B)); // Aloca memória para o cabeçalho

    fseek(arquivoBinario, 0, SEEK_SET); // Move o cursor para o início do arquivo
    fread(&cabecalho->status, sizeof(char), 1, arquivoBinario); // Lê o status do cabeçalho
    fread(&cabecalho->noRaiz, sizeof(int), 1, arquivoBinario); // Lê o número do nó raiz
    fread(&cabecalho->proxRRN, sizeof(int), 1, arquivoBinario); // Lê o próximo RRN disponível
    fread(&cabecalho->nroChaves, sizeof(int), 1, arquivoBinario); // Lê o número de chaves

    return cabecalho; // Retorna o cabeçalho lido
}

// Função para atualizar o cabeçalho da árvore B no arquivo
void escreverCabecalhoArvB(FILE *arquivoBinario, CABECALHO_ARVORE_B *cabecalho) {
    if (arquivoBinario == NULL || cabecalho == NULL) {
        return; // Verifica se o arquivo e o cabeçalho não são nulos
    }
    fseek(arquivoBinario, 0, SEEK_SET); // Move o cursor para o início do arquivo
    int tamanhoCabecalho = sizeof(char) + sizeof(int) * 3; // Calcula o tamanho do cabeçalho

    fwrite(&cabecalho->status, sizeof(char), 1, arquivoBinario); // Escreve o status no arquivo
    fwrite(&cabecalho->noRaiz, sizeof(int), 1, arquivoBinario); // Escreve o número do nó raiz
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, arquivoBinario); // Escreve o próximo RRN disponível
    fwrite(&cabecalho->nroChaves, sizeof(int), 1, arquivoBinario); // Escreve o número de chaves

    char lixo = '$'; // Caractere usado para preencher o espaço restante do cabeçalho
    for (int i = tamanhoCabecalho; i < TAMANHO_DADOS_ARVORE_B; i++) {
        fwrite(&lixo, sizeof(char), 1, arquivoBinario); // Completa com lixo para manter a uniformidade
    }
    return;
}

// Função para retornar a estrutura CABECALHO preenchida com dados lidos do arquivo binário
CABECALHO_DADOS *lerCabecalhoDados(FILE* arquivoBinario) {
    CABECALHO_DADOS *cabecalho = criarCabecalhoDados(); // Cria a estrutura de cabeçalho com valores iniciais
    fseek(arquivoBinario, 0, SEEK_SET); // Move o cursor para o início do arquivo

    char status;
    fread(&status, sizeof(char), 1, arquivoBinario); // Lê o status do cabeçalho
    cabecalho->status = status; // Atualiza o status no cabeçalho

    int64_t topo;
    fread(&topo, sizeof(int64_t), 1, arquivoBinario); // Lê a posição do topo da pilha de registros removidos
    cabecalho->topo = topo; // Atualiza o topo no cabeçalho

    int64_t proxByteOffset;
    fread(&proxByteOffset, sizeof(int64_t), 1, arquivoBinario); // Lê o próximo byte offset para escrita
    cabecalho->proxByteOffset = proxByteOffset; // Atualiza o próximo byte offset no cabeçalho

    int nroRegArq;
    fread(&nroRegArq, sizeof(int), 1, arquivoBinario); // Lê o número de registros arquivados
    cabecalho->nroRegArq = nroRegArq; // Atualiza o número de registros arquivados no cabeçalho

    int nroRem;
    fread(&nroRem, sizeof(int), 1, arquivoBinario); // Lê o número de registros removidos
    cabecalho->nroRegRem = nroRem; // Atualiza o número de registros removidos no cabeçalho

    return(cabecalho); // Retorna o cabeçalho preenchido
}
