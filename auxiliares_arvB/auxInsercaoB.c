/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/funcoesAuxiliaresB.h"

// Função que aumenta a altura dos nós recursivamente
int aumentarAlturaRecursivamente(FILE *arquivo, int rrnRaiz) {
    REGISTRO_ARVORE_B *registro = lerRegistroArvoreB(arquivo, rrnRaiz);

    int nroDescendentes = 0;

    // Conta o número de descendentes do nó
    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        if (getDescendente(registro, i) != -1) {
            nroDescendentes++;
        }
    }

    int maxAltura = -1;

    // Se o nó é folha, define altura 0 e retorna
    if (nroDescendentes == 0) {
        registro->alturaNo = 0;
        escreverRegistroArvoreB(registro, arquivo, rrnRaiz);
        apagarRegistroArvoreB(registro);
        return 0;
    }

    // Calcula a altura máxima dos descendentes
    for (int i = 0; i < nroDescendentes; i++) {
        int rrnDescendente = getDescendente(registro, i);
        int altura = aumentarAlturaRecursivamente(arquivo, rrnDescendente);
        if (altura > maxAltura) {
            maxAltura = altura;
        }
    }

    registro->alturaNo = maxAltura + 1;
    escreverRegistroArvoreB(registro, arquivo, rrnRaiz);
    apagarRegistroArvoreB(registro);

    return maxAltura + 1;
}

// Função para dividir o nó e retornar a nova raiz
bool splitNo(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chavePromovida, int byteOffsetPromovido, REGISTRO_ARVORE_B **caminho, REGISTRO_ARVORE_B *filhoEsq, REGISTRO_ARVORE_B *filhoDir, int nivel) { 
    int chaves[ORDEM_ARVORE_B];
    int byteOffsets[ORDEM_ARVORE_B];

    int index = 0;
    bool inserido = false;
    int posicao = -1;
    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        int chaveLida;
        if (caminho[nivel] == NULL || index < 0 || index >= ORDEM_ARVORE_B - 1) {
            chaveLida = -1; // ou qualquer valor indicando erro
        } else {
            chaveLida = caminho[nivel]->chaves[index];
        }

        if (chavePromovida == chaveLida) {
            return false;
        }
        if (!inserido && chavePromovida < chaveLida) {
            inserido = true;
            chaves[i] = chavePromovida;
            byteOffsets[i] = byteOffsetPromovido;
            posicao = i;
            continue;
        }
        if (chaveLida == -1 && i == ORDEM_ARVORE_B - 1) {
            inserido = true;
            chaves[i] = chavePromovida;
            byteOffsets[i] = byteOffsetPromovido;
            posicao = i;
            break;
        }
        chaves[i] = chaveLida;
        byteOffsets[i] = getByteOffsetRegistroArvoreB(caminho[nivel], index);
        index++;
    }

    chavePromovida = chaves[ORDEM_ARVORE_B / 2 - 1];
    byteOffsetPromovido = byteOffsets[ORDEM_ARVORE_B / 2 - 1];

    bool isFolha = false;

    if (caminho[nivel]->alturaNo == 0) {
        isFolha = true;
    }

    REGISTRO_ARVORE_B *registroEsquerdo = criarRegistroArvoreBVazio();
    int maiorChaveEsquerda = -1;
    REGISTRO_ARVORE_B *registroDireito = criarRegistroArvoreBVazio();
    int menorChaveDireita = -1;

    if (isFolha) {
        for (int i = 0; i < ORDEM_ARVORE_B / 2 - 1; i++) {
            inserirChaveRegistroArvoreB(registroEsquerdo, chaves[i], byteOffsets[i]);
        }
        for (int i = ORDEM_ARVORE_B / 2; i < ORDEM_ARVORE_B; i++) {
            inserirChaveRegistroArvoreB(registroDireito, chaves[i], byteOffsets[i]);
        }
    } else {
        REGISTRO_ARVORE_B *registroEsq = criarRegistroArvoreBVazio();
        REGISTRO_ARVORE_B *registroDir = criarRegistroArvoreBVazio();

        if (filhoEsq == NULL) {
            registroEsq->rrn = -1;
        } else {
            registroEsq->rrn = filhoEsq->rrn;
        }

        int rrnFilhoEsq = registroEsq->rrn;

        if (filhoDir == NULL) {
            registroDir->rrn = -1;
        } else {
            registroDir->rrn = filhoDir->rrn;
        }

        int rrnFilhoDir = registroDir->rrn;

        int64_t descendentes[ORDEM_ARVORE_B + 1];

        index = 0;
        for (int i = 0; i < ORDEM_ARVORE_B; i++) {
            if (i == posicao) {
                descendentes[i] = rrnFilhoEsq;
                descendentes[i + 1] = rrnFilhoDir;
                index += 2;
                continue;
            }
            descendentes[index] = getDescendente(caminho[nivel], i);
            index++;
        }

        apagarRegistroArvoreB(filhoEsq);
        apagarRegistroArvoreB(filhoDir);

        int chavesDescendentes[ORDEM_ARVORE_B + 1];

        for (int i = 0; i < ORDEM_ARVORE_B + 1; i++) {
            REGISTRO_ARVORE_B *registroDescendente = lerRegistroArvoreB(arquivo, descendentes[i]);
            chavesDescendentes[i] = registroDescendente->chaves[0]; // Obtém a chave na posição i

            apagarRegistroArvoreB(registroDescendente);
        }

        for (int i = 0; i < ORDEM_ARVORE_B / 2 - 1; i++) {
            inserirChaveRegistroArvoreB(registroEsquerdo, chaves[i], byteOffsets[i]);
        }

        for (int i = 0; i <= ORDEM_ARVORE_B / 2 - 1; i++) {
            inserirDescendenteRegistroArvoreB(registroEsquerdo, descendentes[i], chavesDescendentes[i]);
        }
        
        for (int i = ORDEM_ARVORE_B / 2; i < ORDEM_ARVORE_B; i++) {
            inserirChaveRegistroArvoreB(registroDireito, chaves[i], byteOffsets[i]);
        }

        for (int i = ORDEM_ARVORE_B / 2; i <= ORDEM_ARVORE_B; i++) {
            inserirDescendenteRegistroArvoreB(registroDireito, descendentes[i], chavesDescendentes[i]);
        }

    }
    
    REGISTRO_ARVORE_B *registro = criarRegistroArvoreBVazio();

    if (caminho[nivel] == NULL) {
        registro->rrn = -1;
    } else {
        registro->rrn = caminho[nivel]->rrn;
    }
    
    int rrnAtual = registro->rrn;

    int proxRRN;

    if (cabecalho == NULL) {
        proxRRN = -1;
    } else {
        proxRRN = cabecalho->proxRRN;
    }

    registroEsquerdo->rrn = rrnAtual;
    registroDireito->rrn = proxRRN;

    escreverRegistroArvoreB(registroEsquerdo, arquivo, rrnAtual);
    escreverRegistroArvoreB(registroDireito, arquivo, proxRRN);

    if (registroEsquerdo == NULL || (ORDEM_ARVORE_B / 2 - 1) < 0 || (ORDEM_ARVORE_B / 2 - 1) >= ORDEM_ARVORE_B - 1) {
        maiorChaveEsquerda = -1; 
    } else {
        maiorChaveEsquerda = registroEsquerdo->chaves[ORDEM_ARVORE_B / 2 - 1];
    }

    if (registroDireito == NULL || 0 >= ORDEM_ARVORE_B - 1) {
        menorChaveDireita = -1; 
    } else {
        menorChaveDireita = registroDireito->chaves[0];
    }

    if (nivel > 0 && (caminho[nivel - 1]->nroChaves == ORDEM_ARVORE_B - 1)) {
        setProxRRNCabecalhoArvoreB(cabecalho, proxRRN + 1);
        if (!splitNo(arquivo, cabecalho, chavePromovida, byteOffsetPromovido, caminho, registroEsquerdo, registroDireito, nivel - 1)) {
            apagarRegistroArvoreB(registroEsquerdo);
            apagarRegistroArvoreB(registroDireito);
            return false;
        }
    } else {
        escreverRegistroArvoreB(registroEsquerdo, arquivo, rrnAtual);
        escreverRegistroArvoreB(registroDireito, arquivo, proxRRN);
        
        if (registroEsquerdo == NULL || (ORDEM_ARVORE_B / 2 - 1) < 0 || (ORDEM_ARVORE_B / 2 - 1) >= ORDEM_ARVORE_B - 1) {
            maiorChaveEsquerda = -1; 
        } else {
            maiorChaveEsquerda = registroEsquerdo->chaves[ORDEM_ARVORE_B / 2 - 1];
        }

        if (registroDireito == NULL || 0 >= ORDEM_ARVORE_B - 1) {
            menorChaveDireita = -1; 
        } else {
            menorChaveDireita = registroDireito->chaves[0];
        }

        if (nivel == 0) {
            REGISTRO_ARVORE_B *novaRaiz = criarRegistroArvoreBVazio();

            int rrnRaiz = proxRRN + 1;

            inserirChaveRegistroArvoreB(novaRaiz, chavePromovida, byteOffsetPromovido);
            inserirDescendenteRegistroArvoreB(novaRaiz, rrnAtual, maiorChaveEsquerda);
            inserirDescendenteRegistroArvoreB(novaRaiz, proxRRN, menorChaveDireita);
            novaRaiz->rrn = rrnRaiz;

            escreverRegistroArvoreB(novaRaiz, arquivo, rrnRaiz);

            CABECALHO_ARVORE_B *cabecalho = lerCabecalhoArvoreB(arquivo);
            setNoRaizCabecalhoArvoreB(cabecalho, rrnRaiz);
            setProxRRNCabecalhoArvoreB(cabecalho, rrnRaiz + 1);

            int chave;

            if (cabecalho == NULL) {
                chave = -1;
            } else {
                chave = cabecalho->nroChaves;
            }

            setNroChavesCabecalhoArvoreB(cabecalho, chave + 1);

            escreverCabecalhoArvoreB(arquivo, cabecalho);

            aumentarAlturaRecursivamente(arquivo, rrnRaiz);

            apagarRegistroArvoreB(registroEsquerdo);
            apagarRegistroArvoreB(registroDireito);
            apagarRegistroArvoreB(novaRaiz);
            limpaCabecalhoArvoreB(cabecalho);
        } else {
            REGISTRO_ARVORE_B *registro = criarRegistroArvoreBVazio();

            if (caminho[nivel - 1] == NULL) {
                registro->rrn = -1;
            } else {
                registro->rrn = caminho[nivel - 1]->rrn;
            }

            REGISTRO_ARVORE_B *registroPai = lerRegistroArvoreB(arquivo, registro->rrn);

            escreverRegistroArvoreB(registroEsquerdo, arquivo, rrnAtual);
            escreverRegistroArvoreB(registroDireito, arquivo, proxRRN);

            inserirChaveRegistroArvoreB(registroPai, chavePromovida, byteOffsetPromovido);
            inserirDescendenteRegistroArvoreB(registroPai, proxRRN, menorChaveDireita);

            escreverRegistroArvoreB(registroPai, arquivo, registro->rrn);

            setProxRRNCabecalhoArvoreB(cabecalho, proxRRN + 1);

            int chave;

            if (cabecalho == NULL) {
                chave = -1;
            } else {
                chave = cabecalho->nroChaves;
            }

            setNroChavesCabecalhoArvoreB(cabecalho, chave + 1);

            escreverCabecalhoArvoreB(arquivo, cabecalho);

            /*REGISTRO_ARVORE_B *registro;

            if (caminho[nivel - 1] == NULL) {
                registro->rrn = -1;
            } else {
                registro->rrn = caminho[nivel - 1]->rrn;
            }*/

            aumentarAlturaRecursivamente(arquivo, registro->rrn);
            apagarRegistroArvoreB(registroEsquerdo);
            apagarRegistroArvoreB(registroDireito);
            apagarRegistroArvoreB(registroPai);
        }
    }

    return true;
}

// Função para inserir em nós que não estão cheios
void insercaoNaoCheio(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, REGISTRO_ARVORE_B *registro) {
    if (!inserirChaveRegistroArvoreB(registro, chave, byteOffset)) {
        return;
    }
    registro->rrn = rrnAtual;

    escreverRegistroArvoreB(registro, arquivo, rrnAtual);

    if (cabecalho == NULL) {
        chave = -1;
    } else {
        chave = cabecalho->nroChaves;
    }

    setNroChavesCabecalhoArvoreB(cabecalho, chave + 1);

    escreverCabecalhoArvoreB(arquivo, cabecalho);
}

// Função para inserir recursivamente
void insercaoArvoreBRecursiva(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, REGISTRO_ARVORE_B **caminho, int nivel, int *tamCaminho) {
    REGISTRO_ARVORE_B *registro = lerRegistroArvoreB(arquivo, rrnAtual);

    registro->rrn = rrnAtual;
    caminho = realloc(caminho, sizeof(REGISTRO_ARVORE_B *) * (nivel + 1));
    caminho[nivel] = registro;
    *tamCaminho = nivel + 1;

    if (registro->alturaNo != 0) {
        int chaves[ORDEM_ARVORE_B - 1];

        for (int i = 0; i < registro->nroChaves; i++) {
            if (registro == NULL || i < 0 || i >= ORDEM_ARVORE_B - 1) {
                chaves[i] = -1; 
            } else {
                chaves[i] = registro->chaves[i];
            }
        }

        int posicao = 0;
        for (int i = 0; i < registro->nroChaves; i++) {
            if (chaves[i] == chave) {
                for (int i = 0; i < *tamCaminho; i++) {
                    apagarRegistroArvoreB(caminho[i]);
                }
                free(caminho);
                return;
            }

            if (chaves[i] > chave) {
                posicao = i;
                break;
            }

            posicao++;
        }

        int rrnDescendente = getDescendente(registro, posicao);
        return insercaoArvoreBRecursiva(arquivo, cabecalho, chave, byteOffset, rrnDescendente, caminho, nivel + 1, tamCaminho);
    }

    if (registro->nroChaves < ORDEM_ARVORE_B - 1) {
        insercaoNaoCheio(arquivo, cabecalho, chave, byteOffset, rrnAtual, registro);
    } else {
        splitNo(arquivo, cabecalho, chave, byteOffset, caminho, NULL, NULL, nivel);
    }

    for (int i = 0; i < *tamCaminho; i++) {
        apagarRegistroArvoreB(caminho[i]);
    }
    free(caminho);
}

// Função para adicionar um nó à árvore B
bool adicionarNoArvoreB(int chave, int64_t byteOffset, FILE *arquivoArvoreB) {
    // Lê o cabeçalho da árvore B
    CABECALHO_ARVORE_B *cabecalho = lerCabecalhoArvoreB(arquivoArvoreB);

    // Verifica se cabeçalho é nulo
    if (cabecalho == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return false;
    }

    // Se o cabeçalho indica que a árvore está inconsistente, retorna false
    if (cabecalho->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        limpaCabecalhoArvoreB(cabecalho);
        return false;
    }

    int rrnRaiz;

    if(cabecalho == NULL) {
        rrnRaiz = -1;
    } else {
        rrnRaiz = cabecalho->noRaiz;
    }

    if (rrnRaiz == -1) {
        // Se a árvore está vazia, cria um novo nó raiz
        REGISTRO_ARVORE_B *novoRegistro = criarRegistroArvoreBVazio();
        novoRegistro->alturaNo = 0;
        inserirChaveRegistroArvoreB(novoRegistro, chave, byteOffset);
        novoRegistro->rrn = 0;

        escreverRegistroArvoreB(novoRegistro, arquivoArvoreB, 0);
        apagarRegistroArvoreB(novoRegistro);

        setNoRaizCabecalhoArvoreB(cabecalho, 0);
        setProxRRNCabecalhoArvoreB(cabecalho, 1);
        setNroChavesCabecalhoArvoreB(cabecalho, 1);
        setStatusCabecalhoArvoreB(cabecalho, '1');
        escreverCabecalhoArvoreB(arquivoArvoreB, cabecalho);
        limpaCabecalhoArvoreB(cabecalho);
        return true;
    }

    // Inicializa o array de caminho para rastrear o caminho percorrido na árvore
    REGISTRO_ARVORE_B **caminho = (REGISTRO_ARVORE_B **)malloc(sizeof(REGISTRO_ARVORE_B *));
    int tamCaminho = 0;

    // Chama a função recursiva de inserção
    insercaoArvoreBRecursiva(arquivoArvoreB, cabecalho, chave, byteOffset, rrnRaiz, caminho, 0, &tamCaminho);

    // Atualiza e escreve o cabeçalho de volta
    setStatusCabecalhoArvoreB(cabecalho, '1');
    escreverCabecalhoArvoreB(arquivoArvoreB, cabecalho);
    limpaCabecalhoArvoreB(cabecalho);

    // Limpa a memória
    for (int i = 0; i < tamCaminho; i++) {
        apagarRegistroArvoreB(caminho[i]);
    }
    free(caminho);

    return true;
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

// Função para inserir no index árvoreB
void inserirArvoreB(FILE *arquivo, int chave, int64_t byteOffset) {
    CABECALHO_ARVORE_B *cabecalho = lerCabecalhoArvoreB(arquivo);

    int nroChaves;

    if (cabecalho == NULL) {
        nroChaves = -1;
    } else {
        nroChaves = cabecalho->nroChaves;
    }

    if (nroChaves == 0) {
        REGISTRO_ARVORE_B *registro = criarRegistroArvoreBVazio();
        registro->alturaNo = 0;
        inserirChaveRegistroArvoreB(registro, chave, byteOffset);
        registro->rrn = 0;

        escreverRegistroArvoreB(registro, arquivo, 0);
        apagarRegistroArvoreB(registro);

        setNoRaizCabecalhoArvoreB(cabecalho, 0);
        setProxRRNCabecalhoArvoreB(cabecalho, 1);
        setNroChavesCabecalhoArvoreB(cabecalho, 1);

        escreverCabecalhoArvoreB(arquivo, cabecalho);
        limpaCabecalhoArvoreB(cabecalho);

        return;
    }

    REGISTRO_ARVORE_B **caminho = (REGISTRO_ARVORE_B **) malloc(sizeof(REGISTRO_ARVORE_B *) * 1);

    int64_t rrnAtual;

    if(cabecalho == NULL){
        rrnAtual = -1;
    } else {
        rrnAtual = cabecalho->noRaiz;
    }

    int tamCaminho = 0;

    insercaoArvoreBRecursiva(arquivo, cabecalho, chave, byteOffset, rrnAtual, caminho, 0, &tamCaminho);

    limpaCabecalhoArvoreB(cabecalho);
}