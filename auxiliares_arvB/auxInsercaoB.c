/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/funcoesAuxiliaresB.h"

// Função que aumenta a altura dos nós recursivamente
int aumentarAlturaRecursivamente(FILE *arquivo, int rrnRaiz) {
    DADOS_ARVORE_B *registro = lerRegistroArvB(arquivo, rrnRaiz); // Lê o registro da árvore B no RRN especificado

    int nroDescendentes = 0; // Inicializa o número de descendentes

    // Conta o número de descendentes do nó
    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        int descendenteTemp = 0; // Variável temporária para armazenar o descendente

        // Verifica se o registro ou índice é inválido, caso contrário obtém o descendente
        if (registro == NULL || i < 0 || i >= ORDEM_ARVORE_B) {
            descendenteTemp = -1;
        } else {
            descendenteTemp = registro->descendentes[i];
        }

        // Incrementa o número de descendentes se for válido
        if (descendenteTemp != -1) {
            nroDescendentes++;
        }
    }

    int maxAltura = -1; // Inicializa a altura máxima

    // Se o nó é folha, define altura 0 e retorna
    if (nroDescendentes == 0) {
        registro->alturaNo = 0; // Define a altura do nó como 0
        escreverRegistroArvB(registro, arquivo, rrnRaiz); // Escreve o registro atualizado de volta no arquivo
        apagarRegistroArvB(registro); // Libera a memória do registro
        return 0; // Retorna 0 indicando que a altura do nó é 0
    }

    // Calcula a altura máxima dos descendentes
    for (int i = 0; i < nroDescendentes; i++) {
        int rrnDescendente = 0; // Variável temporária para armazenar o RRN do descendente

        // Verifica se o registro ou índice é inválido, caso contrário obtém o RRN do descendente
        if (registro == NULL || i < 0 || i >= ORDEM_ARVORE_B) {
            rrnDescendente = -1;
        } else {
            rrnDescendente = registro->descendentes[i];
        }

        // Chama a função recursiva para aumentar a altura dos descendentes e obtém a altura
        int altura = aumentarAlturaRecursivamente(arquivo, rrnDescendente);
        if (altura > maxAltura) {
            maxAltura = altura; // Atualiza a altura máxima
        }
    }

    // Atualiza a altura do nó atual e escreve no arquivo
    registro->alturaNo = maxAltura + 1; // Define a nova altura do nó
    escreverRegistroArvB(registro, arquivo, rrnRaiz); // Escreve o registro atualizado de volta no arquivo
    apagarRegistroArvB(registro); // Libera a memória do registro

    return maxAltura + 1; // Retorna a nova altura do nó
}

// Função para dividir um nó em uma árvore B e ajustar a árvore se necessário.
bool splitNo(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chavePromovida, int byteOffsetPromovido, DADOS_ARVORE_B **caminho, DADOS_ARVORE_B *filhoEsq, DADOS_ARVORE_B *filhoDir, int nivel) {
    int chaves[ORDEM_ARVORE_B];  // Array para armazenar as chaves temporariamente.
    int byteOffsets[ORDEM_ARVORE_B];  // Array para armazenar os byte offsets temporariamente.

    int index = 0;  // Índice para ajudar na iteração e posicionamento.
    bool inserido = false;  // Flag para verificar se a chave foi inserida.
    int posicao = -1;  // Posição para inserir a chave promovida.
    // Itera sobre as chaves do nó atual para reorganizar e inserir a chave promovida.
    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        int chaveLida;  // Armazena a chave lida do nó atual.
        // Verifica se o nó atual é válido e o índice está dentro dos limites.
        if (caminho[nivel] == NULL || index < 0 || index >= ORDEM_ARVORE_B - 1) {
            chaveLida = -1;  // Caso não seja válido, define como -1.
        } else {
            chaveLida = caminho[nivel]->chaves[index];  // Lê a chave do nó.
        }

        // Checa se a chave promovida já existe no nó.
        if (chavePromovida == chaveLida) {
            return false;  // Retorna falso e não insere duplicatas.
        }
        // Se ainda não inseriu e a chave promovida é menor que a chave lida, insere a chave.
        if (!inserido && chavePromovida < chaveLida) {
            inserido = true;
            chaves[i] = chavePromovida;
            byteOffsets[i] = byteOffsetPromovido;
            posicao = i;
            continue;
        }
        // Se chegar ao fim e a chave ainda não foi inserida, insere no final.
        if (chaveLida == -1 && i == ORDEM_ARVORE_B - 1) {
            inserido = true;
            chaves[i] = chavePromovida;
            byteOffsets[i] = byteOffsetPromovido;
            posicao = i;
            break;
        }
        // Copia a chave e o byte offset para os arrays temporários.
        chaves[i] = chaveLida;
        if (caminho[nivel] == NULL || index < 0 || index >= ORDEM_ARVORE_B - 1) {
            byteOffsets[i] = -1;  // Define byte offset como -1 se o índice for inválido.
        } else {
            byteOffsets[i] = caminho[nivel]->byteOffsets[index];  // Copia o byte offset válido.
        }
        
        index++;  // Incrementa o índice para a próxima iteração.
    }

    // Define a chave e o byte offset que serão promovidos para o nível superior.
    chavePromovida = chaves[ORDEM_ARVORE_B / 2 - 1];
    byteOffsetPromovido = byteOffsets[ORDEM_ARVORE_B / 2 - 1];

    // Verifica se o nó atual é folha.
    bool isFolha = false;
    if (caminho[nivel]->alturaNo == 0) {
        isFolha = true;  // Define como verdadeiro se a altura do nó é 0.
    }

    // Cria dois novos registros de árvore B, um para cada lado da divisão.
    DADOS_ARVORE_B *registroEsquerdo = inicializaRegistroArvB();
    int maiorChaveEsquerda = -1;  // Variável para armazenar a maior chave do lado esquerdo.
    DADOS_ARVORE_B *registroDireito = inicializaRegistroArvB();
    int menorChaveDireita = -1;  // Variável para armazenar a menor chave do lado direito.

    // Se o nó é folha, simplesmente divide as chaves entre dois novos nós.
    if (isFolha) {
        for (int i = 0; i < ORDEM_ARVORE_B / 2 - 1; i++) {
            inserirChaveRegistroArvB(registroEsquerdo, chaves[i], byteOffsets[i]);  // Insere as chaves no nó esquerdo.
        }
        for (int i = ORDEM_ARVORE_B / 2; i < ORDEM_ARVORE_B; i++) {
            inserirChaveRegistroArvB(registroDireito, chaves[i], byteOffsets[i]);  // Insere as chaves no nó direito.
        }
    } else {
        // Para nós internos, além de dividir chaves, também divide os descendentes.
        DADOS_ARVORE_B *registroEsq = inicializaRegistroArvB();
        DADOS_ARVORE_B *registroDir = inicializaRegistroArvB();

        // Define os RRNs para os registros esquerdo e direito com base nos filhos dados.
        if (filhoEsq == NULL) {
            registroEsq->rrn = -1;  // Define como -1 se não há filho esquerdo.
        } else {
            registroEsq->rrn = filhoEsq->rrn;  // Usa o RRN do filho esquerdo existente.
        }

        int rrnFilhoEsq = registroEsq->rrn;  // Armazena o RRN do filho esquerdo.

        if (filhoDir == NULL) {
            registroDir->rrn = -1;  // Define como -1 se não há filho direito.
        } else {
            registroDir->rrn = filhoDir->rrn;  // Usa o RRN do filho direito existente.
        }

        int rrnFilhoDir = registroDir->rrn;  // Armazena o RRN do filho direito.

        int64_t descendentes[ORDEM_ARVORE_B + 1];  // Array para armazenar os RRNs dos descendentes temporariamente.

        index = 0;  // Reseta o índice para a iteração seguinte.
        for (int i = 0; i < ORDEM_ARVORE_B; i++) {
            if (i == posicao) {
                descendentes[i] = rrnFilhoEsq;  // Insere o RRN do filho esquerdo na posição atual.
                descendentes[i + 1] = rrnFilhoDir;  // Insere o RRN do filho direito na posição seguinte.
                index += 2;  // Pula um índice para acomodar o filho direito.
                continue;
            }

            // Verifica validade e copia o RRN do descendente.
            if (caminho[nivel] == NULL || i < 0 || i >= ORDEM_ARVORE_B) {
                descendentes[index] = -1;  // Define como -1 se inválido.
            } else {
                descendentes[index] = caminho[nivel]->descendentes[i];  // Copia o RRN válido.
            }

            index++;  // Incrementa o índice para a próxima iteração.
        }

        // Apaga os registros dos filhos esquerdo e direito para liberar memória.
        apagarRegistroArvB(filhoEsq);
        apagarRegistroArvB(filhoDir);

        int chavesDescendentes[ORDEM_ARVORE_B + 1];  // Array para armazenar as chaves dos descendentes.

        // Lê e armazena as chaves dos descendentes para uso posterior.
        for (int i = 0; i < ORDEM_ARVORE_B + 1; i++) {
            DADOS_ARVORE_B *registroDescendente = lerRegistroArvB(arquivo, descendentes[i]);
            chavesDescendentes[i] = registroDescendente->chaves[0];  // Lê a primeira chave do descendente.

            // Apaga o registro do descendente para liberar memória.
            apagarRegistroArvB(registroDescendente);
        }

        // Divide as chaves e os descendentes entre os dois novos registros.
        for (int i = 0; i < ORDEM_ARVORE_B / 2 - 1; i++) {
            inserirChaveRegistroArvB(registroEsquerdo, chaves[i], byteOffsets[i]);  // Insere as chaves no registro esquerdo.
        }

        for (int i = 0; i <= ORDEM_ARVORE_B / 2 - 1; i++) {
            inserirFilhoRegistroArvB(registroEsquerdo, descendentes[i], chavesDescendentes[i]);  // Insere os descendentes no registro esquerdo.
        }
        
        for (int i = ORDEM_ARVORE_B / 2; i < ORDEM_ARVORE_B; i++) {
            inserirChaveRegistroArvB(registroDireito, chaves[i], byteOffsets[i]);  // Insere as chaves no registro direito.
        }

        for (int i = ORDEM_ARVORE_B / 2; i <= ORDEM_ARVORE_B; i++) {
            inserirFilhoRegistroArvB(registroDireito, descendentes[i], chavesDescendentes[i]);  // Insere os descendentes no registro direito.
        }
    }
    
    DADOS_ARVORE_B *registro = inicializaRegistroArvB();  // Cria um novo registro vazio para operações gerais.

    if (caminho[nivel] == NULL) {
        registro->rrn = -1;  // Define o RRN como -1 se o nível atual é inválido.
    } else {
        registro->rrn = caminho[nivel]->rrn;  // Copia o RRN do nível atual.
    }
    
    int rrnAtual = registro->rrn;  // Armazena o RRN atual para uso posterior.

    int proxRRN;  // Variável para armazenar o próximo RRN disponível.

    if (cabecalho == NULL) {
        proxRRN = -1;  // Define como -1 se o cabeçalho é nulo.
    } else {
        proxRRN = cabecalho->proxRRN;  // Copia o próximo RRN disponível do cabeçalho.
    }

    // Define os RRNs dos registros esquerdo e direito.
    registroEsquerdo->rrn = rrnAtual;
    registroDireito->rrn = proxRRN;

    // Escreve os registros esquerdo e direito no arquivo.
    escreverRegistroArvB(registroEsquerdo, arquivo, rrnAtual);
    escreverRegistroArvB(registroDireito, arquivo, proxRRN);

    // Verifica e armazena as chaves mais significativas dos registros esquerdo e direito para uso posterior.
    if (registroEsquerdo == NULL || (ORDEM_ARVORE_B / 2 - 1) < 0 || (ORDEM_ARVORE_B / 2 - 1) >= ORDEM_ARVORE_B - 1) {
        maiorChaveEsquerda = -1;  // Define como -1 se não há chave significativa à esquerda.
    } else {
        maiorChaveEsquerda = registroEsquerdo->chaves[ORDEM_ARVORE_B / 2 - 1];  // Armazena a chave mais significativa do registro esquerdo.
    }

    if (registroDireito == NULL || 0 >= ORDEM_ARVORE_B - 1) {
        menorChaveDireita = -1;  // Define como -1 se não há chave significativa à direita.
    } else {
        menorChaveDireita = registroDireito->chaves[0];  // Armazena a chave mais significativa do registro direito.
    }

    // Lida com a situação onde o nível anterior também precisa ser dividido.
    if (nivel > 0 && (caminho[nivel - 1]->nroChaves == ORDEM_ARVORE_B - 1)) {
        cabecalho->proxRRN = proxRRN + 1;  // Incrementa o próximo RRN no cabeçalho para acomodar novos registros.

        // Tenta dividir o nível anterior recursivamente.
        if (!splitNo(arquivo, cabecalho, chavePromovida, byteOffsetPromovido, caminho, registroEsquerdo, registroDireito, nivel - 1)) {
            apagarRegistroArvB(registroEsquerdo);  // Libera o registro esquerdo se a divisão falhar.
            apagarRegistroArvB(registroDireito);  // Libera o registro direito se a divisão falhar.
            return false;  // Retorna falso se a divisão falhar.
        }
    } else {
        // Escreve os registros esquerdo e direito no arquivo se não precisar dividir o nível anterior.
        escreverRegistroArvB(registroEsquerdo, arquivo, rrnAtual);
        escreverRegistroArvB(registroDireito, arquivo, proxRRN);
        
        // Verifica novamente e armazena as chaves mais significativas dos registros esquerdo e direito.
        if (registroEsquerdo == NULL || (ORDEM_ARVORE_B / 2 - 1) < 0 || (ORDEM_ARVORE_B / 2 - 1) >= ORDEM_ARVORE_B - 1) {
            maiorChaveEsquerda = -1;  // Define como -1 se não há chave significativa à esquerda.
        } else {
            maiorChaveEsquerda = registroEsquerdo->chaves[ORDEM_ARVORE_B / 2 - 1];  // Armazena a chave mais significativa do registro esquerdo.
        }

        if (registroDireito == NULL || 0 >= ORDEM_ARVORE_B - 1) {
            menorChaveDireita = -1;  // Define como -1 se não há chave significativa à direita.
        } else {
            menorChaveDireita = registroDireito->chaves[0];  // Armazena a chave mais significativa do registro direito.
        }

        // Se estiver no nível raiz, cria uma nova raiz.
        if (nivel == 0) {
            DADOS_ARVORE_B *novaRaiz = inicializaRegistroArvB();  // Cria um novo registro vazio para a nova raiz.

            int rrnRaiz = proxRRN + 1;  // Define o RRN da nova raiz.

            // Insere a chave promovida e os descendentes na nova raiz.
            inserirChaveRegistroArvB(novaRaiz, chavePromovida, byteOffsetPromovido);
            inserirFilhoRegistroArvB(novaRaiz, rrnAtual, maiorChaveEsquerda);
            inserirFilhoRegistroArvB(novaRaiz, proxRRN, menorChaveDireita);
            novaRaiz->rrn = rrnRaiz;  // Define o RRN da nova raiz.

            // Escreve a nova raiz no arquivo.
            escreverRegistroArvB(novaRaiz, arquivo, rrnRaiz);

            // Lê o cabeçalho atualizado da árvore B.
            CABECALHO_ARVORE_B *cabecalhoAtualizado = lerCabecalhoArvB(arquivo);
            cabecalhoAtualizado->noRaiz = rrnRaiz;  // Define o novo nó raiz no cabeçalho.
            cabecalhoAtualizado->proxRRN = rrnRaiz + 1;  // Incrementa o próximo RRN no cabeçalho.

            int chave;  // Variável para armazenar o número de chaves.

            // Verifica se o cabeçalho é válido e obtém o número de chaves.
            if (cabecalhoAtualizado == NULL) {
                chave = -1;  // Define como -1 se o cabeçalho é inválido.
            } else {
                chave = cabecalhoAtualizado->nroChaves;  // Obtém o número de chaves do cabeçalho.
            }

            cabecalhoAtualizado->nroChaves = chave + 1;  // Incrementa o número de chaves no cabeçalho.

            // Escreve o cabeçalho atualizado no arquivo.
            escreverCabecalhoArvB(arquivo, cabecalhoAtualizado);

            // Aumenta a altura da árvore recursivamente começando pela nova raiz.
            aumentarAlturaRecursivamente(arquivo, rrnRaiz);

            // Libera os registros dos nós esquerdo, direito e a nova raiz.
            apagarRegistroArvB(registroEsquerdo);
            apagarRegistroArvB(registroDireito);
            apagarRegistroArvB(novaRaiz);
            
            // Libera a memória do cabeçalho atualizado.
            free(cabecalhoAtualizado);
        } else {
            // Trata de uma situação intermediária onde não é a raiz e o nível anterior não precisa de divisão.
            DADOS_ARVORE_B *registro = inicializaRegistroArvB();  // Cria um novo registro vazio para uso geral.

            // Verifica se o nível anterior é válido e define o RRN do registro.
            if (caminho[nivel - 1] == NULL) {
                registro->rrn = -1;  // Define como -1 se o nível anterior é inválido.
            } else {
                registro->rrn = caminho[nivel - 1]->rrn;  // Copia o RRN do nível anterior.
            }

            // Lê o registro pai baseado no RRN obtido.
            DADOS_ARVORE_B *registroPai = lerRegistroArvB(arquivo, registro->rrn);

            // Escreve os registros esquerdo e direito no arquivo.
            escreverRegistroArvB(registroEsquerdo, arquivo, rrnAtual);
            escreverRegistroArvB(registroDireito, arquivo, proxRRN);

            // Insere a chave promovida e o descendente direito no registro pai.
            inserirChaveRegistroArvB(registroPai, chavePromovida, byteOffsetPromovido);
            inserirFilhoRegistroArvB(registroPai, proxRRN, menorChaveDireita);

            // Escreve o registro pai no arquivo.
            escreverRegistroArvB(registroPai, arquivo, registro->rrn);

            // Incrementa o próximo RRN no cabeçalho.
            cabecalho->proxRRN = proxRRN + 1;

            int chave;

            // Verifica se o cabeçalho é válido e obtém o número de chaves.
            if (cabecalho == NULL) {
                chave = -1;  // Define como -1 se o cabeçalho é inválido.
            } else {
                chave = cabecalho->nroChaves;  // Obtém o número de chaves do cabeçalho.
            }

            // Incrementa o número de chaves no cabeçalho.
            cabecalho->nroChaves = chave + 1;

            // Escreve o cabeçalho atualizado no arquivo.
            escreverCabecalhoArvB(arquivo, cabecalho);

            // Aumenta a altura da árvore recursivamente começando pelo nó pai.
            aumentarAlturaRecursivamente(arquivo, registro->rrn);

            // Libera os registros dos nós esquerdo, direito e o registro pai.
            apagarRegistroArvB(registroEsquerdo);
            apagarRegistroArvB(registroDireito);
            apagarRegistroArvB(registroPai);
        }
    }

    return true;  // Retorna verdadeiro indicando sucesso na operação.
}

// Função para inserir em nós que não estão cheios
void insercaoNaoCheio(FILE *arquivo, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, DADOS_ARVORE_B *registro) {
    // Tenta inserir a chave e o offset no registro. Se falhar, sai da função.
    if (!inserirChaveRegistroArvB(registro, chave, byteOffset)) {
        return;
    }
    // Atualiza o RRN do registro com o valor atual.
    registro->rrn = rrnAtual;

    // Escreve o registro atualizado de volta no arquivo.
    escreverRegistroArvB(registro, arquivo, rrnAtual);

    // Se o cabeçalho for nulo, define a chave como -1. Caso contrário, define como o número de chaves no cabeçalho.
    if (cabecalho == NULL) {
        chave = -1;
    } else {
        chave = cabecalho->nroChaves;
    }

    // Incrementa o número de chaves no cabeçalho.
    cabecalho->nroChaves = chave + 1;

    // Escreve o cabeçalho atualizado de volta no arquivo.
    escreverCabecalhoArvB(arquivo, cabecalho);
}

// Função para inserir recursivamente
void insercaoRecursivaArvB(FILE *arquivoBinario, CABECALHO_ARVORE_B *cabecalho, int chave, int byteOffset, int rrnAtual, DADOS_ARVORE_B **caminho, int nivel, int *tamCaminho) {
    // Lê o registro da árvore B no RRN atual.
    DADOS_ARVORE_B *registro = lerRegistroArvB(arquivoBinario, rrnAtual);

    // Atualiza o RRN do registro.
    registro->rrn = rrnAtual;
    // Realoca o caminho para incluir o novo nível.
    caminho = realloc(caminho, sizeof(DADOS_ARVORE_B *) * (nivel + 1));
    // Adiciona o registro atual ao caminho.
    caminho[nivel] = registro;
    // Atualiza o tamanho do caminho.
    *tamCaminho = nivel + 1;

    // Se o nó não for uma folha, continua a busca recursiva.
    if (registro->alturaNo != 0) {
        int chaves[ORDEM_ARVORE_B - 1];

        // Copia as chaves do registro para um array temporário.
        for (int i = 0; i < registro->nroChaves; i++) {
            if (registro == NULL || i < 0 || i >= ORDEM_ARVORE_B - 1) {
                chaves[i] = -1; 
            } else {
                chaves[i] = registro->chaves[i];
            }
        }

        // Encontra a posição para a nova chave.
        int posicao = 0;
        for (int i = 0; i < registro->nroChaves; i++) {
            if (chaves[i] == chave) {
                // Se a chave já existe, libera a memória e retorna.
                for (int i = 0; i < *tamCaminho; i++) {
                    apagarRegistroArvB(caminho[i]);
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
        
        // Define o RRN do descendente a seguir.
        int rrnDescendente = 0;

        if (registro == NULL || posicao < 0 || posicao >= ORDEM_ARVORE_B) {
            rrnDescendente = -1;
        } else {
            rrnDescendente = registro->descendentes[posicao];
        }

        // Chama recursivamente para o próximo nó.
        return insercaoRecursivaArvB(arquivoBinario, cabecalho, chave, byteOffset, rrnDescendente, caminho, nivel + 1, tamCaminho);
    }

    // Se o nó atual tem espaço, insere a chave sem dividir o nó.
    if (registro->nroChaves < ORDEM_ARVORE_B - 1) {
        insercaoNaoCheio(arquivoBinario, cabecalho, chave, byteOffset, rrnAtual, registro);
    } else {
        // Se o nó está cheio, faz o split do nó.
        splitNo(arquivoBinario, cabecalho, chave, byteOffset, caminho, NULL, NULL, nivel);
    }

    // Libera a memória alocada para o caminho.
    for (int i = 0; i < *tamCaminho; i++) {
        apagarRegistroArvB(caminho[i]);
    }
    free(caminho);
}

// Função para adicionar um nó à árvore B
/*bool adicionarNoArvoreB(int chave, int64_t byteOffset, FILE *arquivoArvoreB) {
    // Lê o cabeçalho da árvore B
    CABECALHO_ARVORE_B *cabecalho = lerCabecalhoArvB(arquivoArvoreB);

    // Verifica se o cabeçalho é nulo
    if (cabecalho == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return false;
    }

    // Se o cabeçalho indica que a árvore está inconsistente, retorna false
    if (cabecalho->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        free(cabecalho);
        return false;
    }

    int rrnRaiz;

    // Define o RRN da raiz baseado na existência do cabeçalho
    if (cabecalho == NULL) {
        rrnRaiz = -1;
    } else {
        rrnRaiz = cabecalho->noRaiz;
    }

    // Se a árvore está vazia, cria um novo nó raiz
    if (rrnRaiz == -1) {
        DADOS_ARVORE_B *novoRegistro = inicializaRegistroArvB();
        novoRegistro->alturaNo = 0;
        inserirChaveRegistroArvB(novoRegistro, chave, byteOffset);
        novoRegistro->rrn = 0;

        escreverRegistroArvB(novoRegistro, arquivoArvoreB, 0);
        apagarRegistroArvB(novoRegistro);

        cabecalho->noRaiz = 0;
        cabecalho->proxRRN = 1;
        cabecalho->nroChaves = 1;
        cabecalho->status = '1';

        escreverCabecalhoArvB(arquivoArvoreB, cabecalho);
        free(cabecalho);
        return true;
    }

    // Inicializa o array de caminho para rastrear o caminho percorrido na árvore
    DADOS_ARVORE_B **caminho = (DADOS_ARVORE_B **)malloc(sizeof(DADOS_ARVORE_B *));
    int tamCaminho = 0;

    // Chama a função recursiva de inserção
    insercaoRecursivaArvB(arquivoArvoreB, cabecalho, chave, byteOffset, rrnRaiz, caminho, 0, &tamCaminho);

    // Atualiza e escreve o cabeçalho de volta
    cabecalho->status = '1';
    escreverCabecalhoArvB(arquivoArvoreB, cabecalho);
    free(cabecalho);

    // Limpa a memória
    for (int i = 0; i < tamCaminho; i++) {
        apagarRegistroArvB(caminho[i]);
    }
    free(caminho);

    return true;
}*/

// Função para inserir uma chave em um registro de árvore B
int inserirChaveRegistroArvB(DADOS_ARVORE_B *registro, int chave, int64_t byteOffset) {
    // Se o registro estiver cheio, não é possível inserir
    if (registro->nroChaves == ORDEM_ARVORE_B - 1) {
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
int inserirFilhoRegistroArvB(DADOS_ARVORE_B *registro, int64_t descendente, int chaveDescendente) {
    // Verifica se o registro é nulo ou se o número de chaves é zero.
    if (!registro || registro->nroChaves == 0) {
        return 0; // Retorna 0 se o registro for inválido.
    }

    int i = 0; // Índice para percorrer as chaves do registro.

    // Encontra a posição correta para inserir o descendente.
    while (i < registro->nroChaves && registro->chaves[i] < chaveDescendente) {
        if (registro->chaves[i] == chaveDescendente) {
        // Se a chave já existe, não insere e retorna 0.
        return 0;
        }
        i++;
    }

    // Desloca os descendentes para abrir espaço para o novo descendente.
    if (registro->descendentes[i] != -1) {
        for (int j = registro->nroChaves; j > i; j--) {
        registro->descendentes[j] = registro->descendentes[j - 1];
        }
    }

    // Insere o novo descendente na posição correta.
    registro->descendentes[i] = descendente;

    return 1; // Retorna 1 se a inserção for bem-sucedida.
}

void inserirArvoreB(FILE *arquivoBinario, int chave, int64_t byteOffset) {
    // Lê o cabeçalho da árvore B do arquivo.
    CABECALHO_ARVORE_B *cabecalho = lerCabecalhoArvB(arquivoBinario);

    int nroChaves; // Número de chaves na árvore.

    // Verifica se o cabeçalho foi lido com sucesso.
    if (cabecalho == NULL) {
        nroChaves = -1; // Se o cabeçalho for nulo, a árvore está vazia.
    } else {
        nroChaves = cabecalho->nroChaves; // Obtém o número de chaves do cabeçalho.
    }

    // Se a árvore estiver vazia...
    if (nroChaves == 0) {
        // Cria um novo registro de árvore B vazio.
        DADOS_ARVORE_B *registro = inicializaRegistroArvB();
        registro->alturaNo = 0; // Define a altura do nó como 0.

        // Insere a chave e o offset de byte no registro.
        inserirChaveRegistroArvB(registro, chave, byteOffset);
        registro->rrn = 0; // Define o RRN do nó como 0.

        // Escreve o registro no arquivo.
        escreverRegistroArvB(registro, arquivoBinario, 0);

        // Apaga o registro da memória (já foi escrito no arquivo).
        apagarRegistroArvB(registro);

        // Atualiza o cabeçalho com as novas informações.
        cabecalho->noRaiz = 0; // Define o RRN da raiz como 0.
        cabecalho->proxRRN = 1; // Define o próximo RRN disponível como 1.
        cabecalho->nroChaves = 1; // Define o número de chaves como 1.

        // Escreve o cabeçalho atualizado no arquivo.
        escreverCabecalhoArvB(arquivoBinario, cabecalho);

        // Libera a memória alocada para o cabeçalho.
        free(cabecalho);

        // Retorna da função, pois a inserção foi concluída.
        return;
    }

    // Se a árvore não estiver vazia...

    // Aloca um array para armazenar o caminho percorrido durante a inserção.
    DADOS_ARVORE_B **caminho = (DADOS_ARVORE_B **) malloc(sizeof(DADOS_ARVORE_B *) * 1);

    int64_t rrnAtual; // RRN do nó atual.

    // Obtém o RRN do nó raiz (ou -1 se a árvore estiver vazia).
    if (cabecalho == NULL) {
        rrnAtual = -1;
    } else {
        rrnAtual = cabecalho->noRaiz;
    }

    int tamCaminho = 0; // Tamanho do array de caminho.

    // Chama a função recursiva para realizar a inserção recursivamente na árvore.
    insercaoRecursivaArvB(arquivoBinario, cabecalho, chave, byteOffset, rrnAtual, caminho, 0, &tamCaminho);

    // Libera a memória alocada para o cabeçalho.
    free(cabecalho);
}