/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "funcoesAuxiliares.h"

// Função para criar um registro vazio de árvore B com valores padrão
REGISTRO_ARVORE_B *criarRegistroArvoreBVazio() {
    REGISTRO_ARVORE_B *registro = malloc(sizeof(REGISTRO_ARVORE_B));
    registro->rrn = -1;
    registro->alturaNo = 0;
    registro->nroChaves = 0;

    // Inicializa os arrays de chaves, byte offsets e descendentes com -1
    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        registro->chaves[i] = -1;
        registro->byteOffsets[i] = -1;
    }
    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        registro->descendentes[i] = -1;
    }

    return registro;
}

// Função para inserir uma chave em um registro de árvore B
int inserirChaveRegistroArvoreB(REGISTRO_ARVORE_B *registro, int chave, int64_t byteOffset) {
    if (registro->nroChaves == ORDEM_ARVORE_B - 1) {
        // Se o registro estiver cheio, não é possível inserir
        return 0;
    } else if (registro->nroChaves == 0) {
        // Se o registro estiver vazio, insere a chave na primeira posição
        registro->chaves[0] = chave;
        registro->byteOffsets[0] = byteOffset;
        registro->nroChaves++;
    } else {
        // Encontra a posição correta para inserir a chave
        int posicao = 0;
        for (int i = 0; i < registro->nroChaves; i++) {
            if (registro->chaves[i] == chave) {
                // Se a chave já existe, não insere
                return 0;
            }
            if (registro->chaves[i] > chave) {
                posicao = i;
                break;
            }
            posicao++;
        }

        // Desloca as chaves e byte offsets para abrir espaço para a nova chave
        if (registro->nroChaves > posicao) {
            for (int i = registro->nroChaves; i > posicao; i--) {
                registro->chaves[i] = registro->chaves[i - 1];
                registro->byteOffsets[i] = registro->byteOffsets[i - 1];
            }
        }

        // Insere a nova chave e byte offset na posição correta
        registro->chaves[posicao] = chave;
        registro->byteOffsets[posicao] = byteOffset;
        registro->nroChaves++;
    }

    return 1;
}

// Função para remover uma chave de um registro de árvore B
int removerChaveRegistroArvoreB(REGISTRO_ARVORE_B *registro, int chave) {
    if (registro->nroChaves == 0) {
        // Se o registro estiver vazio, não há chave para remover
        return 0;
    } else {
        // Encontra a posição da chave a ser removida
        int i = 0;
        while (i < registro->nroChaves && registro->chaves[i] != chave) {
            i++;
        }
        if (i == registro->nroChaves) {
            // Se não encontrou a chave, retorna falha
            return 0;
        } else {
            // Desloca as chaves e byte offsets para preencher o espaço da chave removida
            for (int j = i; j < registro->nroChaves - 1; j++) {
                registro->chaves[j] = registro->chaves[j + 1];
                registro->byteOffsets[j] = registro->byteOffsets[j + 1];
            }
            registro->nroChaves--;
        }
    }

    return 1;
}

// Função para inserir um descendente em um registro de árvore B
int inserirDescendenteRegistroArvoreB(REGISTRO_ARVORE_B *registro, int64_t descendente, int chaveDescendente) {
    if (!registro || registro->nroChaves == 0) {
        return 0;
    }

    int i = 0;
    // Encontra a posição correta para inserir o descendente
    while (i < registro->nroChaves && registro->chaves[i] < chaveDescendente) {
        if (registro->chaves[i] == chaveDescendente) {
            // Se a chave já existe, não insere
            return 0;
        }
        i++;
    }

    // Desloca os descendentes para abrir espaço para o novo descendente
    if (registro->descendentes[i] != -1) {
        for (int j = registro->nroChaves; j > i; j--) {
            registro->descendentes[j] = registro->descendentes[j - 1];
        }
    }

    // Insere o novo descendente na posição correta
    registro->descendentes[i] = descendente;

    return 1;
}

// Função para remover um descendente de um registro de árvore B
int removerDescendenteRegistroArvoreB(REGISTRO_ARVORE_B *registro, int64_t descendente) {
    if (registro->nroChaves == 0) {
        return 0;
    }

    int i = 0;
    // Encontra a posição do descendente a ser removido
    while (i < registro->nroChaves && registro->descendentes[i] != descendente) {
        i++;
    }

    if (i == ORDEM_ARVORE_B) {
        // Se não encontrou o descendente, retorna falha
        return 0;
    }

    // Remove o descendente
    registro->descendentes[i] = -1;

    return 1;
}

// Função para obter o RRN de um registro de árvore B
int getRRNRegistroArvoreB(REGISTRO_ARVORE_B *registro) {
    if (registro == NULL) {
        return -1;
    }
    
    return registro->rrn;
}

// Função para obter a altura de um nó em um registro de árvore B
int getAlturaNoRegistroArvoreB(REGISTRO_ARVORE_B *registro) {
    if (registro == NULL) {
        return -1;
    }
    
    return registro->alturaNo;
}

// Função para definir o RRN de um registro de árvore B
int setRRNRegistroArvoreB(REGISTRO_ARVORE_B *registro, int rrn) {
    if (registro == NULL) {
        return 0;
    }
    
    registro->rrn = rrn;
    return 1;
}

// Função para definir a altura de um nó em um registro de árvore B
int setAlturaNoRegistroArvoreB(REGISTRO_ARVORE_B *registro, int alturaNo) {
    if (registro == NULL) {
        return 0;
    }
    
    registro->alturaNo = alturaNo;
    return 1;
}

// Função para obter o número de chaves em um registro de árvore B
int getNroChavesRegistroArvoreB(REGISTRO_ARVORE_B *registro) {
    if (registro == NULL) {
        return -1;
    }
    
    return registro->nroChaves;
}

// Função para obter uma chave em uma posição específica de um registro de árvore B
int getChave(REGISTRO_ARVORE_B *registro, int posicao) {
    if (registro == NULL || posicao < 0 || posicao >= ORDEM_ARVORE_B - 1) {
        return -1;
    }

    return registro->chaves[posicao];
}

// Função para obter o byte offset de uma chave em uma posição específica de um registro de árvore B
int64_t getByteOffsetRegistroArvoreB(REGISTRO_ARVORE_B *registro, int posicao) {
    if (registro == NULL || posicao < 0 || posicao >= ORDEM_ARVORE_B - 1) {
        return -1;
    }

    return registro->byteOffsets[posicao];
}

// Função para obter um descendente em uma posição específica de um registro de árvore B
int getDescendente(REGISTRO_ARVORE_B *registro, int posicao) {
    if (registro == NULL || posicao < 0 || posicao >= ORDEM_ARVORE_B) {
        return -1;
    }

    return registro->descendentes[posicao];
}

// Função para liberar a memória de um registro de árvore B
int apagarRegistroArvoreB(REGISTRO_ARVORE_B *registro) {
    if (registro == NULL) {
        return 0;
    }
    
    free(registro);
    return 1;
}

// Função para ler um registro de árvore B de um arquivo
REGISTRO_ARVORE_B *lerRegistroArvoreB(FILE *arquivo, int rrn) {
    REGISTRO_ARVORE_B *registro = criarRegistroArvoreBVazio();
    if (registro == NULL || arquivo == NULL) {
        return NULL;
    }
    
    int64_t byteOffset = (rrn + 1) * TAMANHO_REGISTRO_ARVORE_B;

    fseek(arquivo, byteOffset, SEEK_SET);

    fread(&registro->alturaNo, sizeof(int), 1, arquivo);
    fread(&registro->nroChaves, sizeof(int), 1, arquivo);
    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        fread(&registro->chaves[i], sizeof(int), 1, arquivo);
        fread(&registro->byteOffsets[i], sizeof(int64_t), 1, arquivo);
    }
    fread(registro->descendentes, sizeof(int), ORDEM_ARVORE_B, arquivo);

    return registro;
}

// Função para escrever um registro de árvore B em um arquivo
int escreverRegistroArvoreB(REGISTRO_ARVORE_B *registro, FILE *arquivo, int rrn) {
    if (registro == NULL || arquivo == NULL) {
        return 0;
    }
    
    int64_t byteOffset = (rrn + 1) * TAMANHO_REGISTRO_ARVORE_B;

    fseek(arquivo, byteOffset, SEEK_SET);

    fwrite(&registro->alturaNo, sizeof(int), 1, arquivo);
    fwrite(&registro->nroChaves, sizeof(int), 1, arquivo);
    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        fwrite(&registro->chaves[i], sizeof(int), 1, arquivo);
        fwrite(&registro->byteOffsets[i], sizeof(int64_t), 1, arquivo);
    }
    fwrite(registro->descendentes, sizeof(int), ORDEM_ARVORE_B, arquivo);

    return 1;
}