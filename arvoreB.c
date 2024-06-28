/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "funcoesAuxiliares.h"

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
        setAlturaNoRegistroArvoreB(registro, 0);
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

    setAlturaNoRegistroArvoreB(registro, maxAltura + 1);
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
        int chaveLida = getChave(caminho[nivel], index);
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
    if (getAlturaNoRegistroArvoreB(caminho[nivel]) == 0) {
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
        int rrnFilhoEsq = getRRNRegistroArvoreB(filhoEsq);
        int rrnFilhoDir = getRRNRegistroArvoreB(filhoDir);

        long long int descendentes[ORDEM_ARVORE_B + 1];

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
            chavesDescendentes[i] = getChave(registroDescendente, 0);
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

    int rrnAtual = getRRNRegistroArvoreB(caminho[nivel]);
    int proxRRN = getProxRRNCabecalhoArvoreB(cabecalho);

    setRRNRegistroArvoreB(registroEsquerdo, rrnAtual);
    setRRNRegistroArvoreB(registroDireito, proxRRN);
    escreverRegistroArvoreB(registroEsquerdo, arquivo, rrnAtual);
    escreverRegistroArvoreB(registroDireito, arquivo, proxRRN);

    maiorChaveEsquerda = getChave(registroEsquerdo, ORDEM_ARVORE_B / 2 - 1);
    menorChaveDireita = getChave(registroDireito, 0);

    if (nivel > 0 && getNroChavesRegistroArvoreB(caminho[nivel - 1]) == ORDEM_ARVORE_B - 1) {
        setProxRRNCabecalhoArvoreB(cabecalho, proxRRN + 1);
        if (!splitNo(arquivo, cabecalho, chavePromovida, byteOffsetPromovido, caminho, registroEsquerdo, registroDireito, nivel - 1)) {
            apagarRegistroArvoreB(registroEsquerdo);
            apagarRegistroArvoreB(registroDireito);
            return false;
        }
    } else {
        escreverRegistroArvoreB(registroEsquerdo, arquivo, rrnAtual);
        escreverRegistroArvoreB(registroDireito, arquivo, proxRRN);

        maiorChaveEsquerda = getChave(registroEsquerdo, ORDEM_ARVORE_B / 2 - 2);
        menorChaveDireita = getChave(registroDireito, 0);

        if (nivel == 0) {
            REGISTRO_ARVORE_B *novaRaiz = criarRegistroArvoreBVazio();

            int rrnRaiz = proxRRN + 1;

            inserirChaveRegistroArvoreB(novaRaiz, chavePromovida, byteOffsetPromovido);
            inserirDescendenteRegistroArvoreB(novaRaiz, rrnAtual, maiorChaveEsquerda);
            inserirDescendenteRegistroArvoreB(novaRaiz, proxRRN, menorChaveDireita);
            setRRNRegistroArvoreB(novaRaiz, rrnRaiz);
            escreverRegistroArvoreB(novaRaiz, arquivo, rrnRaiz);

            CABECALHO_ARVORE_B *cabecalho = lerCabecalhoArvoreB(arquivo);
            setNoRaizCabecalhoArvoreB(cabecalho, rrnRaiz);
            setProxRRNCabecalhoArvoreB(cabecalho, rrnRaiz + 1);
            setNroChavesCabecalhoArvoreB(cabecalho, getNroChavesCabecalhoArvoreB(cabecalho) + 1);
            escreverCabecalhoArvoreB(arquivo, cabecalho);

            aumentarAlturaRecursivamente(arquivo, rrnRaiz);

            apagarRegistroArvoreB(registroEsquerdo);
            apagarRegistroArvoreB(registroDireito);
            apagarRegistroArvoreB(novaRaiz);
            apagarCabecalhoArvoreB(cabecalho);
        } else {
            REGISTRO_ARVORE_B *registroPai = lerRegistroArvoreB(arquivo, getRRNRegistroArvoreB(caminho[nivel - 1]));

            escreverRegistroArvoreB(registroEsquerdo, arquivo, rrnAtual);
            escreverRegistroArvoreB(registroDireito, arquivo, proxRRN);

            inserirChaveRegistroArvoreB(registroPai, chavePromovida, byteOffsetPromovido);
            inserirDescendenteRegistroArvoreB(registroPai, proxRRN, menorChaveDireita);
            escreverRegistroArvoreB(registroPai, arquivo, getRRNRegistroArvoreB(caminho[nivel - 1]));

            setProxRRNCabecalhoArvoreB(cabecalho, proxRRN + 1);
            setNroChavesCabecalhoArvoreB(cabecalho, getNroChavesCabecalhoArvoreB(cabecalho) + 1);
            escreverCabecalhoArvoreB(arquivo, cabecalho);

            aumentarAlturaRecursivamente(arquivo, getRRNRegistroArvoreB(caminho[nivel - 1]));

            apagarRegistroArvoreB(registroEsquerdo);
            apagarRegistroArvoreB(registroDireito);
            apagarRegistroArvoreB(registroPai);
        }
    }

    return true;
}

void insercaoNaoCheio(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, REGISTRO_ARVORE_B *registro) {
    if (!inserirChaveRegistroArvoreB(registro, chave, byteOffset)) {
        return;
    }
    setRRNRegistroArvoreB(registro, rrnAtual);

    escreverRegistroArvoreB(registro, arquivo, rrnAtual);

    setNroChavesCabecalhoArvoreB(cabecalho, getNroChavesCabecalhoArvoreB(cabecalho) + 1);
    escreverCabecalhoArvoreB(arquivo, cabecalho);
}

void insercaoArvoreBRecursiva(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, REGISTRO_ARVORE_B **caminho, int nivel, int *tamCaminho) {
    REGISTRO_ARVORE_B *registro = lerRegistroArvoreB(arquivo, rrnAtual);

    setRRNRegistroArvoreB(registro, rrnAtual);
    caminho = realloc(caminho, sizeof(REGISTRO_ARVORE_B *) * (nivel + 1));
    caminho[nivel] = registro;
    *tamCaminho = nivel + 1;

    if (getAlturaNoRegistroArvoreB(registro) != 0) {
        int chaves[ORDEM_ARVORE_B - 1];

        for (int i = 0; i < getNroChavesRegistroArvoreB(registro); i++) {
            chaves[i] = getChave(registro, i);
        }

        int posicao = 0;
        for (int i = 0; i < getNroChavesRegistroArvoreB(registro); i++) {
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

    if (getNroChavesRegistroArvoreB(registro) < ORDEM_ARVORE_B - 1) {
        insercaoNaoCheio(arquivo, cabecalho, chave, byteOffset, rrnAtual, registro);
    } else {
        splitNo(arquivo, cabecalho, chave, byteOffset, caminho, NULL, NULL, nivel);
    }

    for (int i = 0; i < *tamCaminho; i++) {
        apagarRegistroArvoreB(caminho[i]);
    }
    free(caminho);
}

void inserirArvoreB(FILE *arquivo, int chave, long long int byteOffset) {
    CABECALHO_ARVORE_B *cabecalho = lerCabecalhoArvoreB(arquivo);

    int nroChaves = getNroChavesCabecalhoArvoreB(cabecalho);

    if (nroChaves == 0) {
        REGISTRO_ARVORE_B *registro = criarRegistroArvoreBVazio();
        setAlturaNoRegistroArvoreB(registro, 0);
        inserirChaveRegistroArvoreB(registro, chave, byteOffset);
        setRRNRegistroArvoreB(registro, 0);

        escreverRegistroArvoreB(registro, arquivo, 0);
        apagarRegistroArvoreB(registro);

        setNoRaizCabecalhoArvoreB(cabecalho, 0);
        setProxRRNCabecalhoArvoreB(cabecalho, 1);
        setNroChavesCabecalhoArvoreB(cabecalho, 1);

        escreverCabecalhoArvoreB(arquivo, cabecalho);
        apagarCabecalhoArvoreB(cabecalho);

        return;
    }

    REGISTRO_ARVORE_B **caminho = (REGISTRO_ARVORE_B **) malloc(sizeof(REGISTRO_ARVORE_B *) * 1);
    long long int rrnAtual = getNoRaizCabecalhoArvoreB(cabecalho);

    int tamCaminho = 0;

    insercaoArvoreBRecursiva(arquivo, cabecalho, chave, byteOffset, rrnAtual, caminho, 0, &tamCaminho);

    apagarCabecalhoArvoreB(cabecalho);
}