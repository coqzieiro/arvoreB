/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/definicoesTipos.h"
#include "../bibliotecas/funcoes_fornecidas.h"
#include "../bibliotecas/funcoesAuxiliaresB.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Função para imprimir registros por campos de busca
void imprimirRegistrosPorCampos(FILE *file, CABECALHO *cabecalho, int buscaId, char *nomeArquivoArvoreB, int i) {
    int64_t byteOffset = cabecalho->proxByteOffset;
    int numRegistros = cabecalho->nroRegArq + cabecalho->nroRegRem; // Número total de registros (incluindo removidos)
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
        DADOS *registro = leitura_registro_arquivoBin(byteOffset, file); // Lê um registro do arquivo binário
        byteOffset += registro->tamanhoRegistro; // Atualiza o byteOffset para a posição do próximo registro

        int imprimir = 1; // Flag para determinar se o registro deve ser impresso
        if (registro->removido == '1') { // Verifica se o registro está marcado como removido
            imprimir = 0;
        } else {
            for (int k = 0; k < m; k++) {
                if (strcmp(campos[k], "id") == 0) { // Verifica se o parâmetro da busca é o id
                    if (id !=  registro->id) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "nomeJogador") == 0) {
                    char* nomeJogador;
                    if (registro->nomeJogador == NULL || strcmp(registro->nomeJogador, "") == 0) // Check for NULL and empty string
                    {
                        nomeJogador = "SEM DADO";
                    }else nomeJogador = registro->nomeJogador;
                    if (strcmp(nome, nomeJogador) != 0) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "idade") == 0) {
                    if (idade != registro->idade) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "nomeClube") == 0) {
                    char* nomeClube;
                    if (registro->nomeClube == NULL || strcmp(registro->nomeClube, "") == 0) // Check for NULL and empty string
                    {
                        nomeClube = "SEM DADO";
                    }else nomeClube = registro->nomeClube;
                    if (strcmp(nomeClube, nomeClube) != 0) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "nacionalidade") == 0) {
                    char* nacionalidade1;
                    if (registro->nacionalidade == NULL || strcmp(registro->nacionalidade, "") == 0) // Check for NULL and empty string
                    {
                        nacionalidade1 = "SEM DADO";
                    }else nacionalidade1 =  registro->nacionalidade;
                    if (strcmp(nacionalidade, nacionalidade1) != 0) {
                        imprimir = 0;
                    }
                }
            }
        }
        
        if (imprimir == 1) { // Se todos os parâmetros coincidem, imprime o registro
            imprimeRegistro(registro); // Chama a função para imprimir o registro
            impressoes++;
        }
        free_registro(registro); // Libera a memória do registro
    }
    if(impressoes == 0) { // Se nenhum registro foi impresso
        printf("Registro inexistente.\n\n");
    }
}

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

// Função recursiva para buscar um registro por ID na árvore B
int64_t buscarRegistroIdRec(FILE *fileArvoreB, int id, int rrnAtual) {
    REGISTRO_ARVORE_B *registroAtual = lerRegistroArvoreB(fileArvoreB, rrnAtual); // Lê o registro da árvore B no RRN atual
    
    int numerosChaves = registroAtual->nroChaves; // Obtém o número de chaves no registro
    int chave;
    int descendente;

    for(int i = 0; i < numerosChaves; i++) {
        if (registroAtual == NULL || i < 0 || i >= ORDEM_ARVORE_B - 1) {
            chave = -1; 
        } else {
            chave = registroAtual->chaves[i];
        }

        if (registroAtual == NULL || i < 0 || i >= ORDEM_ARVORE_B) {
            descendente = -1;
        } else {
            descendente = registroAtual->descendentes[i];
        }

        if(id == chave) {
            int64_t byteoffsetRegistro;

            if (registroAtual == NULL || i < 0 || i >= ORDEM_ARVORE_B - 1) {
                byteoffsetRegistro = -1;
            } else {
                byteoffsetRegistro = registroAtual->byteOffsets[i];
            }

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

    if (registroAtual == NULL || numerosChaves < 0 || numerosChaves >= ORDEM_ARVORE_B) {
        descendente = -1;
    } else {
        descendente = registroAtual->descendentes[numerosChaves];
    }

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

    char removido = registro->removido;
    int tamRegistroOriginal = registro->tamanhoRegistro;
    int tamanhoRegistro = tamRegistroOriginal;
    int64_t prox = registro->prox;
    int id =  registro->id;
    int idade = registro->idade;
    int tamNomeJogador = registro->tamNomeJog;
    char *nomeJogador;

    if (registro->nomeJogador == NULL || strcmp(registro->nomeJogador, "") == 0) // Check for NULL and empty string
    {
        nomeJogador = "SEM DADO";
    }
    else{
        nomeJogador = registro->nomeJogador;
    }

    int tamNacionalidade =registro->tamNacionalidade;
    char *nacionalidade;

    if (registro->nacionalidade == NULL || strcmp(registro->nacionalidade, "") == 0) // Check for NULL and empty string
    {
        nacionalidade = "SEM DADO";
    }else nacionalidade = registro->nacionalidade;

    int tamNomeClube = registro->tamNomeClube;
    char *nomeClube;
    if (registro->nomeClube == NULL || strcmp(registro->nomeClube, "") == 0) // Check for NULL and empty string
    {
        nomeClube = "SEM DADO";
    }else nomeClube = registro->nomeClube;

    fseek(arquivoBin, byteOffset, SEEK_SET); // Move o ponteiro do arquivo para a posição do registro

    if(tamRegistroAtual != 0) {
        registro->tamanhoRegistro = tamRegistroAtual;
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
    if (registro->removido == '0') { // Se o registro não foi removido, imprime seus dados na tela

        // Recebe o valor dos atributos do registro
        char *nomeClube;
        if (registro->nomeClube == NULL || strcmp(registro->nomeClube, "") == 0) // Check for NULL and empty string
        {
            nomeClube =  "SEM DADO";
        }else nomeClube = registro->nomeClube;
        char *nacionalidade;
        if (registro->nacionalidade == NULL || strcmp(registro->nacionalidade, "") == 0) // Check for NULL and empty string
        {
            nacionalidade =  "SEM DADO";
        }else nacionalidade = registro->nacionalidade;

        char *nomeJogador;

        if (registro->nomeJogador == NULL || strcmp(registro->nomeJogador, "") == 0) // Check for NULL and empty string
        {
            nomeJogador = "SEM DADO";
        }else nomeJogador = registro->nomeJogador;

        printf("Nome do Jogador: ");
        if (strcmp(nomeJogador, "SEM DADO") == 0) { // Se o nome do jogador for "SEM DADO", imprime "SEM DADO"
            printf("SEM DADO\n");
        } else { // Se não, imprime cada caractere do nome do jogador
            for (int j = 0; j < registro->tamNomeJog; j++) {
                printf("%c", nomeJogador[j]);
            }
            printf("\n");
        }

        printf("Nacionalidade do Jogador: ");
        if (strcmp(nacionalidade, "SEM DADO") == 0) { // Se a nacionalidade for "SEM DADO", imprime "SEM DADO"
            printf("SEM DADO\n");
        } else { // Se não, imprime cada caractere da nacionalidade
            for (int j = 0; j <registro->tamNacionalidade; j++) {
                printf("%c", nacionalidade[j]);
            }
            printf("\n");
        }

        printf("Clube do Jogador: ");
        if (strcmp(nomeClube, "SEM DADO") == 0) { // Se o nome do clube for "SEM DADO", imprime "SEM DADO"
            printf("SEM DADO\n");
        } else { // Se não, imprime cada caractere do nome do clube
            for (int j = 0; j < registro->tamNomeClube; j++) {
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
        free(lista->registros[i]);
    }
    free(lista->registros);

    free(lista);
    return 1;
}

// Função para criar um registro de índice
REGISTRO_INDICE *inicializa_registro_index() {
    REGISTRO_INDICE *registro = (REGISTRO_INDICE *) malloc(sizeof(REGISTRO_INDICE));
    if(!registro) return NULL;
    registro->index = -1;
    registro->byteOffset = -1;
    return registro;
}

// Função para buscar a posição de um registro de índice na lista de forma linear
int buscarPosicaoRegistroIndiceLinear(LISTA_INDICE *lista, int id) {
    if (lista->tamanho == 0) return -1; // Verifica se a lista está vazia

    // Percorre a lista de forma linear para encontrar o id
    for (int i = 0; i < lista->tamanho; i++) {

        int idAtual = lista->registros[i]->index;
        if (idAtual == id) return i; // id encontrado
    }

    return -1; // Se o registro não foi encontrado, retorna -1
}

// Função para obter o maior byte offset menor que um ID específico
int64_t getMaiorByteOffsetMenorQue(REMOVIDOS *removidos, int id) {
    int posicao = buscarPosicaoRegistroIndiceLinear(removidos->lista, id); // Busca a posição do registro com o ID fornecido
    if(posicao <= 0) {
        return -1;
    } else {
        REGISTRO_INDICE *registroIndice = removidos->lista->registros[posicao - 1];
        return registroIndice->byteOffset;
    }
}

// Função para obter o byte offset do best fit para um array de registros
int64_t *getBestFitArrayRegistros(REMOVIDOS *removidos, DADOS **registros, int quantidade, FILE *file) {
    int *tamanhos = malloc(sizeof(int) * quantidade);
    int64_t *byteOffsets = malloc(sizeof(int64_t) * quantidade);

    if(removidos->lista->tamanho == 0) { // Se não há registros removidos
        CABECALHO *cabecalho = retornaCabecalhoBinario(file);

        cabecalho->nroRegArq = cabecalho->nroRegArq + quantidade;
        writeNroRegArqCabecalho(cabecalho, file);

        for(int i = 0; i < quantidade; i++) {
            byteOffsets[i] = -1;
        }

        free(cabecalho);
        free(tamanhos);

        return byteOffsets;
    }

    for(int i = 0; i < quantidade; i++) {
        if(registros[i]->tamanhoRegistro == 0) {
            tamanhos[i] = -1;
            byteOffsets[i] = 0;
            continue;
        }
        tamanhos[i] = registros[i]->tamanhoRegistro;
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
    int right = removidos->lista->tamanho - 1;
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

    int64_t byteOffset = removidos->lista->registros[middle]->byteOffset;

    // Retorna o primeiro elemento da lista com o tamanho desejado
    while(middle - 1 >= 0 && removidos->tamanhos[middle - 1] == tamanho) {
        if(removidos->tamanhos[middle] != removidos->tamanhos[middle - 1]) {
            break;
        }
        int64_t byteOffsetAnterior = removidos->lista->registros[middle - 1]->byteOffset;
        byteOffset = byteOffsetAnterior;
        middle--;
    }

    if(middle + 1 < removidos->lista->tamanho) { // Tem um registro removido depois
        int64_t proxByteOffset = removidos->lista->registros[middle + 1]->byteOffset;
        registro->prox = proxByteOffset;
    } else { // Não tem registro removido depois
        registro->prox = -1;
    }

    removerRegistroRemovidoEAtualizarArquivo(removidos, middle, file);

    return byteOffset;
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