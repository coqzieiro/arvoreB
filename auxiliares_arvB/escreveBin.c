/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/definicoesTipos.h"
#include "../bibliotecas/funcoes_fornecidas.h"
#include "../bibliotecas/funcoesAuxiliares.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Função recursiva para buscar um registro por ID na árvore B
int64_t buscarRegistroIdRec(FILE *fileArvoreB, int id, int rrnAtual) {
    REGISTRO_ARVORE_B *registroAtual = lerRegistroArvoreB(fileArvoreB, rrnAtual); // Lê o registro da árvore B no RRN atual
    
    int numerosChaves = getNroChavesRegistroArvoreB(registroAtual); // Obtém o número de chaves no registro
    int chave;
    int descendente;

    for(int i = 0; i < numerosChaves; i++) {
        chave = getChave(registroAtual, i); // Obtém a chave na posição i
        descendente = getDescendente(registroAtual, i); // Obtém o descendente na posição i

        if(id == chave) {
            int64_t byteoffsetRegistro = getByteOffsetRegistroArvoreB(registroAtual, i); // Obtém o byte offset do registro
            apagarRegistroArvoreB(registroAtual); // Libera a memória do registro
            return byteoffsetRegistro; // Retorna o byte offset do registro
        } else if(id < chave) {
            if(descendente != -1) {
                apagarRegistroArvoreB(registroAtual); // Libera a memória do registro
                return buscarRegistroIdRec(fileArvoreB, id, descendente); // Busca recursivamente no descendente
            }
            apagarRegistroArvoreB(registroAtual); // Libera a memória do registro
            return -1; // Registro não encontrado
        }
    }

    descendente = getDescendente(registroAtual, numerosChaves); // Obtém o descendente na última posição
    if(descendente != -1) {
        apagarRegistroArvoreB(registroAtual); // Libera a memória do registro
        return buscarRegistroIdRec(fileArvoreB, id, descendente); // Busca recursivamente no descendente
    }
    apagarRegistroArvoreB(registroAtual); // Libera a memória do registro
    return -1; // Registro não encontrado
}

// Função para escrever um registro no arquivo binário
bool escreverRegistro(DADOS *registro, int byteOffset, int tamRegistroAtual, FILE *arquivoBin) {
    if(registro == NULL || byteOffset < 25 || arquivoBin == NULL) {
        return false;
    }

    char removido = retornaRemovido(registro);
    int tamRegistroOriginal = retornaTamanhoRegistro(registro);
    int tamanhoRegistro = tamRegistroOriginal;
    int64_t prox = retornaProx(registro);
    int id = retornaId(registro);
    int idade = retornaIdade(registro);
    int tamNomeJogador = RetornaTamNomeJogador(registro);
    char *nomeJogador = retornaNomeJogador(registro);
    int tamNacionalidade = retornaTamNacionalidade(registro);
    char *nacionalidade = retornaNacionalidade(registro);
    int tamNomeClube = retornaTamNomeClube(registro);
    char *nomeClube = retornaNomeClube(registro);

    fseek(arquivoBin, byteOffset, SEEK_SET); // Move o ponteiro do arquivo para a posição do registro

    if(tamRegistroAtual != 0) {
        setaTamanhoRegistro(registro, tamRegistroAtual);
        tamanhoRegistro = tamRegistroAtual;
    }

    fwrite(&removido, sizeof(char), 1, arquivoBin); // Escreve o campo removido no arquivo
    fwrite(&tamanhoRegistro, sizeof(int), 1, arquivoBin); // Escreve o campo tamanhoRegistro no arquivo
    fwrite(&prox, sizeof(int64_t), 1, arquivoBin); // Escreve o campo prox no arquivo
    fwrite(&id, sizeof(int), 1, arquivoBin); // Escreve o campo id no arquivo
    fwrite(&idade, sizeof(int), 1, arquivoBin); // Escreve o campo idade no arquivo
    fwrite(&tamNomeJogador, sizeof(int), 1, arquivoBin); // Escreve o campo tamNomeJogador no arquivo
    fwrite(nomeJogador, sizeof(char), tamNomeJogador, arquivoBin); // Escreve o campo nomeJogador no arquivo
    fwrite(&tamNacionalidade, sizeof(int), 1, arquivoBin); // Escreve o campo tamNacionalidade no arquivo
    fwrite(nacionalidade, sizeof(char), tamNacionalidade, arquivoBin); // Escreve o campo nacionalidade no arquivo
    fwrite(&tamNomeClube, sizeof(int), 1, arquivoBin); // Escreve o campo tamNomeClube no arquivo
    fwrite(nomeClube, sizeof(char), tamNomeClube, arquivoBin); // Escreve o campo nomeClube no arquivo

    for (int i = 0; i < tamRegistroAtual - tamRegistroOriginal; i++) {
        fwrite("$", sizeof(char), 1, arquivoBin); // Preenche o registro com '$'
    }

    return true;
}

// Função para imprimir um registro
int imprimeRegistro(DADOS *registro) {
    if (retornaRemovido(registro) == '0') { // Se o registro não foi removido, imprime seus dados na tela

        // Recebe o valor dos atributos do registro
        char *nomeClube = retornaNomeClube(registro);
        char *nacionalidade = retornaNacionalidade(registro);
        char *nomeJogador = retornaNomeJogador(registro);

        printf("Nome do Jogador: ");
        if (strcmp(nomeJogador, "SEM DADO") == 0) { // Se o nome do jogador for "SEM DADO", imprime "SEM DADO"
            printf("SEM DADO\n");
        } else { // Se não, imprime cada caractere do nome do jogador
            for (int j = 0; j < RetornaTamNomeJogador(registro); j++) {
                printf("%c", nomeJogador[j]);
            }
            printf("\n");
        }

        printf("Nacionalidade do Jogador: ");
        if (strcmp(nacionalidade, "SEM DADO") == 0) { // Se a nacionalidade for "SEM DADO", imprime "SEM DADO"
            printf("SEM DADO\n");
        } else { // Se não, imprime cada caractere da nacionalidade
            for (int j = 0; j < retornaTamNacionalidade(registro); j++) {
                printf("%c", nacionalidade[j]);
            }
            printf("\n");
        }

        printf("Clube do Jogador: ");
        if (strcmp(nomeClube, "SEM DADO") == 0) { // Se o nome do clube for "SEM DADO", imprime "SEM DADO"
            printf("SEM DADO\n");
        } else { // Se não, imprime cada caractere do nome do clube
            for (int j = 0; j < retornaTamNomeClube(registro); j++) {
                printf("%c", nomeClube[j]);
            }
            printf("\n");
        }
        printf("\n");
        return 1;
    } else {
        return 0;
    }
}

// Função para imprimir registros por campos de busca
void imprimirRegistrosPorCampos(FILE *file, CABECALHO *cabecalho, int buscaId, char *nomeArquivoArvoreB, int i) {
    int64_t byteOffset = getProxByteOffset(cabecalho);
    int numRegistros = getNroRegArq(cabecalho) + getNroRem(cabecalho); // Número total de registros (incluindo removidos)
    byteOffset = 25; // Posição inicial do byteOffset

    if (numRegistros == 0) { // Verifica se o arquivo não possui registros
        printf("Registro inexistente.\n\n");
        fclose(file); // Fecha o arquivo
        return;
    }

    char campos[5][50]; // Armazena os nomes dos campos a serem buscados
    char parametros[5][50]; // Armazena os valores dos parâmetros a serem buscados
    int id = -1, idade;
    char nome[50], nomeClube[50], nacionalidade[50]; // Variáveis para armazenar os valores dos campos

    int impressoes = 0; // Contador de impressões de registros
    int m;
    scanf("%i", &m); // Lê o número de parâmetros da busca

    for (int j = 0; j < m; j++) {
        scanf("%s", campos[j]); // Lê um parâmetro da busca
        if (strcmp(campos[j], "id") == 0) {
            scanf("%i", &id); // Lê o id da busca
            snprintf(parametros[j], 50, "%i", id);
            if(buscaId == 1 && id != -1) { // Verifica se a busca pelo índice vai ser feita com o arquivo da árvore B
                buscaIdArvore(id, file, nomeArquivoArvoreB, i, 1); // Chama a função para buscar o id na árvore B
                return;
            }
        } else if (strcmp(campos[j], "nomeJogador") == 0) {
            scan_quote_string(nome); // Lê o nome do jogador
            strcpy(parametros[j], nome);
        } else if (strcmp(campos[j], "idade") == 0) {
            scanf("%i", &idade); // Lê a idade
            snprintf(parametros[j], 50, "%i", idade);
        } else if (strcmp(campos[j], "nomeClube") == 0) {
            scan_quote_string(nomeClube); // Lê o nome do clube
            strcpy(parametros[j], nomeClube);
        } else if (strcmp(campos[j], "nacionalidade") == 0) {
            scan_quote_string(nacionalidade); // Lê a nacionalidade
            strcpy(parametros[j], nacionalidade);
        } else {
            printf("Campo invalido\n"); // Parâmetro inválido
        }
    }

    printf("Busca %d\n\n", i+1); // Imprime o número da busca

    for (int j = 0; j < numRegistros; j++) {
        DADOS *registro = lerRegistroFromBin(byteOffset, file); // Lê um registro do arquivo binário
        byteOffset += retornaTamanhoRegistro(registro); // Atualiza o byteOffset para a posição do próximo registro

        int imprimir = 1; // Flag para determinar se o registro deve ser impresso
        if (retornaRemovido(registro) == '1') { // Verifica se o registro está marcado como removido
            imprimir = 0;
        } else {
            for (int k = 0; k < m; k++) {
                if (strcmp(campos[k], "id") == 0) { // Verifica se o parâmetro da busca é o id
                    if (id != retornaId(registro)) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "nomeJogador") == 0) {
                    if (strcmp(nome, retornaNomeJogador(registro)) != 0) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "idade") == 0) {
                    if (idade != retornaIdade(registro)) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "nomeClube") == 0) {
                    if (strcmp(nomeClube, retornaNomeClube(registro)) != 0) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "nacionalidade") == 0) {
                    if (strcmp(nacionalidade, retornaNacionalidade(registro)) != 0) {
                        imprimir = 0;
                    }
                }
            }
        }
        
        if (imprimir == 1) { // Se todos os parâmetros coincidem, imprime o registro
            imprimeRegistro(registro); // Chama a função para imprimir o registro
            impressoes++;
        }
        liberarRegistro(registro); // Libera a memória do registro
    }
    if(impressoes == 0) { // Se nenhum registro foi impresso
        printf("Registro inexistente.\n\n");
    }
}

// Função para criar uma lista de registros removidos a partir de um arquivo binário
REMOVIDOS *criarListaRemovidos(FILE *file) {
    CABECALHO *cabecalho = retornaCabecalhoBinario(file); // Obtém o cabeçalho do arquivo
    REMOVIDOS *removidos = criarListaRemovidosVazia(); // Cria uma lista vazia de registros removidos

    fseek(file, 0, SEEK_END);
    int finalArquivo = ftell(file); // Obtém o final do arquivo

    int proxByteOffset = getTopo(cabecalho); // Obtém o topo da lista de removidos
    
    int count = 0;

    // Percorre todos os registros removidos
    while(proxByteOffset != -1 && proxByteOffset < finalArquivo) {
        DADOS *registro = lerRegistroFromBin(proxByteOffset, file);

        count++;

        if(retornaRemovido(registro) == '1') {
            REGISTRO_INDICE *registroIndice = criarRegistroIndice();
            setIndexRegistroIndice(registroIndice, retornaId(registro));
            setByteOffsetRegistroIndice(registroIndice, proxByteOffset);

            adicionarRegistroRemovido(removidos, registroIndice, retornaTamanhoRegistro(registro));
        }

        proxByteOffset = retornaProx(registro);

        liberarRegistro(registro);

        DADOS *proxRegistro;

        if(proxByteOffset != -1 && proxByteOffset < finalArquivo)
            proxRegistro = lerRegistroFromBin(proxByteOffset, file);

        // Anota o último registro removido
        if(retornaProx(proxRegistro) == -1 && retornaRemovido(proxRegistro) == '1') {
            REGISTRO_INDICE *registroIndice = criarRegistroIndice();
            setIndexRegistroIndice(registroIndice, retornaId(proxRegistro));
            setByteOffsetRegistroIndice(registroIndice, proxByteOffset);

            adicionarRegistroRemovido(removidos, registroIndice, retornaTamanhoRegistro(proxRegistro));
            liberarRegistro(proxRegistro);
            break;
        }

        liberarRegistro(proxRegistro);
    }

    limpaCabecalho(cabecalho); // Libera a memória do cabeçalho

    return removidos;
}

// Função para criar uma lista vazia de registros removidos
REMOVIDOS *criarListaRemovidosVazia() {
    REMOVIDOS *removidos = malloc(sizeof(REMOVIDOS));
    removidos->lista = criarListaIndice();
    removidos->tamanhos = malloc(sizeof(int) * 1000);

    return removidos;
}

// Função para remover um registro da lista de removidos e atualizar o arquivo
void removerRegistroRemovidoEAtualizarArquivo(REMOVIDOS *removidos, int posicao, FILE *file) {
    DADOS *registro = buscaOfsset(getByteOffsetRegistroIndice(getRegistroIndice(removidos->lista, posicao)), file);

    if(posicao == -1) {
        return;
    }

    int tamanhoLista = getTamanhoListaIndice(removidos->lista);
    const int byteProx = 5;
    CABECALHO *cabecalho = retornaCabecalhoBinario(file);

    setStatus(cabecalho, '0');

    setNroRegArq(cabecalho, getNroRegArq(cabecalho) + 1);
    writeNroRegArqCabecalho(cabecalho, file);
    setNroRem(cabecalho, getNroRem(cabecalho) - 1);
    writeNroRegRemCabecalho(cabecalho, file);
    
    if(tamanhoLista == 1) { // Lista só tem um elemento removido
        setTopo(cabecalho, -1);
        writeTopoCabecalho(cabecalho, file);
    } else if(posicao == 0) { // Removendo o primeiro elemento
        REGISTRO_INDICE *registroIndice = getRegistroIndice(removidos->lista, 1);
        int64_t byteOffset = getByteOffsetRegistroIndice(registroIndice);

        setTopo(cabecalho, byteOffset);
        writeTopoCabecalho(cabecalho, file);
    } else if(posicao == tamanhoLista - 1) { // Removendo o último elemento
        REGISTRO_INDICE *registroIndice = getRegistroIndice(removidos->lista, posicao - 1);
        int64_t byteOffset = getByteOffsetRegistroIndice(registroIndice);

        int prox = -1;
        
        byteOffset += byteProx;
        fseek(file, byteOffset, SEEK_SET);
        fwrite(&prox, sizeof(int), 1, file);
    } else {
        REGISTRO_INDICE *registroIndiceAnterior = getRegistroIndice(removidos->lista, posicao - 1);
        REGISTRO_INDICE *registroIndiceProximo = getRegistroIndice(removidos->lista, posicao + 1);

        int64_t byteOffsetAnterior = getByteOffsetRegistroIndice(registroIndiceAnterior);
        int64_t byteOffsetProximo = getByteOffsetRegistroIndice(registroIndiceProximo);

        byteOffsetAnterior += byteProx;
        fseek(file, byteOffsetAnterior, SEEK_SET);
        fwrite(&byteOffsetProximo, sizeof(int), 1, file);
    }

    liberarRegistro(registro);
    limpaCabecalho(cabecalho);

    removerRegistroRemovidoPosicao(removidos, posicao);
}

// Função para apagar a lista de registros removidos
void apagarListaRemovidos(REMOVIDOS *removidos) {
    apagarListaIndice(removidos->lista);
    free(removidos->tamanhos);
    free(removidos);
}

// Função para criar uma lista de índices
LISTA_INDICE *criarListaIndice() {
    LISTA_INDICE *lista = (LISTA_INDICE *)malloc(sizeof(LISTA_INDICE));
    lista->tamanho = 0;
    lista->max_tamanho = 1000;
    lista->registros = (REGISTRO_INDICE **)malloc(sizeof(REGISTRO_INDICE *) * lista->max_tamanho); // Aloca espaço para 1000 endereços de registros

    return lista;
}

// Função que libera a memória da lista de índices e de seus registros
bool apagarListaIndice(LISTA_INDICE *lista) {
    if (lista == NULL) return false;

    for (int i = 0; i < lista->tamanho; i++) {
        apagarRegistroIndice(lista->registros[i]);
    }
    free(lista->registros);

    free(lista);
    return 1;
}

// Função para criar um registro de índice
REGISTRO_INDICE *criarRegistroIndice() {
    REGISTRO_INDICE *registro = (REGISTRO_INDICE *) malloc(sizeof(REGISTRO_INDICE));
    if(!registro) return NULL;
    registro->index = -1;
    registro->byteOffset = -1;
    return registro;
}

// Função para remover um registro da lista de índices
void removerRegistroIndice(LISTA_INDICE *lista, int index) {
    apagarRegistroIndice(lista->registros[index]); // Apaga o registro

    // Desloca todos os registros após o registro removido para a esquerda
    for (int i = index; i < lista->tamanho - 1; i++) {
        lista->registros[i] = lista->registros[i + 1];
    }

    lista->tamanho--;
}

// Função para remover um registro da posição especificada na lista de removidos
void removerRegistroRemovidoPosicao(REMOVIDOS *removidos, int posicao) {
    removerRegistroIndice(removidos->lista, posicao);

    for(int i = posicao; i < getTamanhoListaIndice(removidos->lista); i++) {
        removidos->tamanhos[i] = removidos->tamanhos[i + 1];
    }
}

// Função para apagar um registro de índice
void apagarRegistroIndice(REGISTRO_INDICE *registro) {
    free(registro);
}

// Função para definir o índice de um registro de índice
void setIndexRegistroIndice(REGISTRO_INDICE *registro, int index) {
    registro->index = index;
}

// Função para definir o byte offset de um registro de índice
void setByteOffsetRegistroIndice(REGISTRO_INDICE *registro, int64_t byteOffset) {
    registro->byteOffset = byteOffset;
}

// Função para definir um registro na lista de índices
void setRegistroListaIndice(LISTA_INDICE *lista, int index, REGISTRO_INDICE *registro) {
    lista->registros[index] = registro;
}

// Função para definir o tamanho da lista de índices
void setTamanho(LISTA_INDICE *lista, int tamanho) {
    lista->tamanho = tamanho;
}

// Função para obter um registro de índice da lista
REGISTRO_INDICE *getRegistroIndice(LISTA_INDICE *lista, int index) {
    return lista->registros[index];
}

// Função para buscar a posição de um registro de índice na lista de forma linear
int buscarPosicaoRegistroIndiceLinear(LISTA_INDICE *lista, int id) {
    if (lista->tamanho == 0) return -1; // Verifica se a lista está vazia

    // Percorre a lista de forma linear para encontrar o id
    for (int i = 0; i < lista->tamanho; i++) {
        int idAtual = getIndexRegistroIndice(getRegistroIndice(lista, i));
        if (idAtual == id) return i; // id encontrado
    }

    return -1; // Se o registro não foi encontrado, retorna -1
}

// Função para adicionar um novo registro na lista em ordem de tamanho
void adicionarRegistroRemovido(REMOVIDOS *removidos, REGISTRO_INDICE *registroIndice, int tamanho) {
    int right = getTamanhoListaIndice(removidos->lista);
    int left = 0;
    
    while (left < right) {
        int middle = (left + right) / 2;
        if (removidos->tamanhos[middle] > tamanho) {
            right = middle;
        } else {
            left = middle + 1;
        }
    }

    // Move elementos à direita para abrir espaço para o novo registro
    shiftElementosListaRemovidosRight(removidos, left);

    // Adiciona o novo registro na posição encontrada
    setRegistroListaIndice(removidos->lista, left, registroIndice);
    removidos->tamanhos[left] = tamanho;

    // Atualiza o tamanho da lista
    setTamanho(removidos->lista, getTamanhoListaIndice(removidos->lista) + 1);
}

// Função para deslocar elementos da lista de removidos para a direita
void shiftElementosListaRemovidosRight(REMOVIDOS *removidos, int pos) {
    for(int i = getTamanhoListaIndice(removidos->lista); i > pos; i--) {
        REGISTRO_INDICE *registro = getRegistroIndice(removidos->lista, i - 1);
        setRegistroListaIndice(removidos->lista, i, registro);
        removidos->tamanhos[i] = removidos->tamanhos[i - 1];
    }
}

// Função para obter o maior byte offset menor que um ID específico
int64_t getMaiorByteOffsetMenorQue(REMOVIDOS *removidos, int id) {
    int posicao = buscarPosicaoRegistroIndiceLinear(removidos->lista, id); // Busca a posição do registro com o ID fornecido
    if(posicao <= 0) {
        return -1;
    } else {
        REGISTRO_INDICE *registroIndice = getRegistroIndice(removidos->lista, posicao - 1);
        return getByteOffsetRegistroIndice(registroIndice);
    }
}

// Função para obter o tamanho da lista de índices
int getTamanhoListaIndice(LISTA_INDICE *lista) {
    return lista->tamanho;
}

// Função para obter o índice de um registro de índice
int getIndexRegistroIndice(REGISTRO_INDICE *registro) {
    return registro->index;
}

// Função para obter o tamanho da lista de removidos
int getTamanhoListaRemovidos(REMOVIDOS *removidos) {
    return getTamanhoListaIndice(removidos->lista);
}

// Função para obter o byte offset de um registro de índice
int64_t getByteOffsetRegistroIndice(REGISTRO_INDICE *registro) {
    return registro->byteOffset;
}

// Função para obter o byte offset do best fit para um array de registros
int64_t *getBestFitArrayRegistros(REMOVIDOS *removidos, DADOS **registros, int quantidade, FILE *file) {
    int *tamanhos = malloc(sizeof(int) * quantidade);
    int64_t *byteOffsets = malloc(sizeof(int64_t) * quantidade);

    if(getTamanhoListaRemovidos(removidos) == 0) { // Se não há registros removidos
        CABECALHO *cabecalho = retornaCabecalhoBinario(file);

        setNroRegArq(cabecalho, getNroRegArq(cabecalho) + quantidade);
        writeNroRegArqCabecalho(cabecalho, file);

        for(int i = 0; i < quantidade; i++) {
            byteOffsets[i] = -1;
        }

        limpaCabecalho(cabecalho);

        free(tamanhos);

        return byteOffsets;
    }

    for(int i = 0; i < quantidade; i++) {
        if(retornaTamanhoRegistro(registros[i]) == 0) {
            tamanhos[i] = -1;
            byteOffsets[i] = 0;
            continue;
        }
        tamanhos[i] = retornaTamanhoRegistro(registros[i]);
    }

    // Obtém o best fit na ordem do maior para o menor
    for(int i = 0; i < quantidade; i++) {
        int maior = -1;
        int posicao = -1;

        for(int j = 0; j < quantidade; j++) {
            if(tamanhos[j] > maior) {
                maior = tamanhos[j];
                posicao = j;
            }
        }

        if(posicao == -1) continue;

        byteOffsets[posicao] = getBestFitAndFreeSpace(removidos, tamanhos[posicao], registros[posicao], file);
        tamanhos[posicao] = -1;
    }

    free(tamanhos);

    return byteOffsets;
}

// Função para obter o melhor ajuste (best fit) e liberar o espaço correspondente
int64_t getBestFitAndFreeSpace(REMOVIDOS *removidos, int tamanho, DADOS *registro, FILE *file) {
    int left = 0;
    int right = getTamanhoListaIndice(removidos->lista) - 1;
    int middle = (left + right) / 2;

    // Busca binária do tamanho
    while(left < right) {
        if(removidos->tamanhos[middle] == tamanho) {
            break;
        } else if(removidos->tamanhos[middle] > tamanho) {
            right = middle;
        } else {
            left = middle + 1;
        }
        middle = (left + right) / 2;
    }

    int64_t byteOffset = getByteOffsetRegistroIndice(getRegistroIndice(removidos->lista, middle));

    // Retorna o primeiro elemento da lista com o tamanho desejado
    while(middle - 1 >= 0 && removidos->tamanhos[middle - 1] == tamanho) {
        if(removidos->tamanhos[middle] != removidos->tamanhos[middle - 1]) {
            break;
        }
        int64_t byteOffsetAnterior = getByteOffsetRegistroIndice(getRegistroIndice(removidos->lista, middle - 1));
        byteOffset = byteOffsetAnterior;
        middle--;
    }

    if(middle + 1 < getTamanhoListaIndice(removidos->lista)) { // Tem um registro removido depois
        int64_t proxByteOffset = getByteOffsetRegistroIndice(getRegistroIndice(removidos->lista, middle + 1));
        setaProx(registro, proxByteOffset);
    } else { // Não tem registro removido depois
        setaProx(registro, -1);
    }

    removerRegistroRemovidoEAtualizarArquivo(removidos, middle, file);

    return byteOffset;
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

    int rrnRaiz = getNoRaizCabecalhoArvoreB(cabecalho);
    if (rrnRaiz == -1) {
        // Se a árvore está vazia, cria um novo nó raiz
        REGISTRO_ARVORE_B *novoRegistro = criarRegistroArvoreBVazio();
        setAlturaNoRegistroArvoreB(novoRegistro, 0);
        inserirChaveRegistroArvoreB(novoRegistro, chave, byteOffset);
        setRRNRegistroArvoreB(novoRegistro, 0);

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