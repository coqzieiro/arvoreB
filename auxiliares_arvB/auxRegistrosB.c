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
void imprimirRegistrosCampos(FILE *arquivoBinario, CABECALHO_DADOS *cabecalho, int buscaId, char *nomeArquivoArvoreB, int i) {
    // Variável para armazenar o offset do byte no arquivo
    int64_t byteOffset = cabecalho->proxByteOffset;
    // Número total de registros (incluindo removidos)
    int numRegistros = cabecalho->nroRegArq + cabecalho->nroRegRem;
    // Posição inicial do byteOffset
    byteOffset = 25;

    // Verifica se o arquivo não possui registros
    if (numRegistros == 0) {
        printf("Registro inexistente.\n\n");
        fclose(arquivoBinario); // Fecha o arquivo
        return;
    }

    // Arrays para armazenar os nomes dos campos e valores dos parâmetros a serem buscados
    char campos[5][50];
    char parametros[5][50];

    // Variáveis para armazenar os valores dos campos
    int id = -1, idade;
    char nome[50], nomeClube[50], nacionalidade[50];

    // Contador de impressões de registros
    int impressoes = 0;
    int m;

    // Lê o número de parâmetros da busca
    scanf("%i", &m);

    // Loop para ler os parâmetros de busca
    for (int j = 0; j < m; j++) {
        scanf("%s", campos[j]); // Lê um parâmetro da busca

        // Verifica se o parâmetro é "id"
        if (strcmp(campos[j], "id") == 0) {
            scanf("%i", &id); // Lê o id da busca
            snprintf(parametros[j], 50, "%i", id);
            // Verifica se a busca pelo índice vai ser feita com o arquivo da árvore B
            if(buscaId == 1 && id != -1) {
                buscaIdArvore(id, arquivoBinario, nomeArquivoArvoreB, i, 1); // Chama a função para buscar o id na árvore B
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

    // Imprime o número da busca
    printf("Busca %d\n\n", i+1);

    // Loop para ler e verificar cada registro do arquivo
    for (int j = 0; j < numRegistros; j++) {
        // Lê um registro do arquivo binário
        DADOS *registro = lerRegistroBinario(byteOffset, arquivoBinario);
        // Atualiza o byteOffset para a posição do próximo registro
        byteOffset += registro->tamanhoRegistro;

        // Flag para determinar se o registro deve ser impresso
        int imprimir = 1;

        // Verifica se o registro está marcado como removido
        if (registro->removido == '1') {
            imprimir = 0;
        } else {
            // Loop para verificar se os parâmetros coincidem com o registro
            for (int k = 0; k < m; k++) {
                if (strcmp(campos[k], "id") == 0) { // Verifica se o parâmetro da busca é o id
                    if (id != registro->id) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "nomeJogador") == 0) {
                    // Verifica se o nome do jogador é NULL ou string vazia
                    char* nomeJogador;
                    if (registro->nomeJogador == NULL || strcmp(registro->nomeJogador, "") == 0) {
                        nomeJogador = "SEM DADO";
                    } else {
                        nomeJogador = registro->nomeJogador;
                    }
                    if (strcmp(nome, nomeJogador) != 0) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "idade") == 0) {
                    if (idade != registro->idade) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "nomeClube") == 0) {
                    // Verifica se o nome do clube é NULL ou string vazia
                    char* nomeClube;
                    if (registro->nomeClube == NULL || strcmp(registro->nomeClube, "") == 0) {
                        nomeClube = "SEM DADO";
                    } else {
                        nomeClube = registro->nomeClube;
                    }
                    if (strcmp(nomeClube, nomeClube) != 0) {
                        imprimir = 0;
                    }
                } else if (strcmp(campos[k], "nacionalidade") == 0) {
                    // Verifica se a nacionalidade é NULL ou string vazia
                    char* nacionalidade1;
                    if (registro->nacionalidade == NULL || strcmp(registro->nacionalidade, "") == 0) {
                        nacionalidade1 = "SEM DADO";
                    } else {
                        nacionalidade1 = registro->nacionalidade;
                    }
                    if (strcmp(nacionalidade, nacionalidade1) != 0) {
                        imprimir = 0;
                    }
                }
            }
        }
        
        // Se todos os parâmetros coincidem, imprime o registro
        if (imprimir == 1) {
            imprimirRegistrosDados(registro); // Chama a função para imprimir o registro
            impressoes++;
        }
        // Libera a memória do registro
        liberaRegistroDados(registro);
    }

    // Se nenhum registro foi impresso
    if (impressoes == 0) {
        printf("Registro inexistente.\n\n");
    }
}

// Função para criar um registro vazio de árvore B com valores padrão
DADOS_ARVORE_B *inicializaRegistroArvB() {
    // Aloca memória para um novo registro da árvore B
    DADOS_ARVORE_B *registro = malloc(sizeof(DADOS_ARVORE_B));
    
    // Inicializa os campos do registro com valores padrão
    registro->rrn = -1;          // RRN (Relative Record Number) do registro é -1
    registro->alturaNo = 0;      // Altura do nó é 0
    registro->nroChaves = 0;     // Número de chaves no nó é 0

    // Inicializa os arrays de chaves e byte offsets com -1
    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        registro->chaves[i] = -1;         // Inicializa cada chave com -1
        registro->byteOffsets[i] = -1;    // Inicializa cada byte offset com -1
    }

    // Inicializa o array de descendentes com -1
    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        registro->descendentes[i] = -1;   // Inicializa cada descendente com -1
    }

    // Retorna o registro inicializado
    return registro;
}

// Função recursiva para buscar um registro por ID na árvore B
int64_t buscarRegistroRecursivo(FILE *arquivoBinario, int id, int rrnAtual) {
    // Lê o registro da árvore B no RRN atual
    DADOS_ARVORE_B *registroAtual = lerRegistroArvB(arquivoBinario, rrnAtual);

    // Obtém o número de chaves no registro
    int numerosChaves = registroAtual->nroChaves;
    int chave;
    int descendente;

    // Loop para percorrer todas as chaves do registro
    for(int i = 0; i < numerosChaves; i++) {
        // Verifica se o índice está fora dos limites e inicializa a chave
        if (registroAtual == NULL || i < 0 || i >= ORDEM_ARVORE_B - 1) {
            chave = -1; 
        } else {
            chave = registroAtual->chaves[i];
        }

        // Verifica se o índice está fora dos limites e inicializa o descendente
        if (registroAtual == NULL || i < 0 || i >= ORDEM_ARVORE_B) {
            descendente = -1;
        } else {
            descendente = registroAtual->descendentes[i];
        }

        // Verifica se a chave é igual ao ID buscado
        if(id == chave) {
            int64_t byteoffsetRegistro;

            // Verifica se o índice está fora dos limites e inicializa o byte offset
            if (registroAtual == NULL || i < 0 || i >= ORDEM_ARVORE_B - 1) {
                byteoffsetRegistro = -1;
            } else {
                byteoffsetRegistro = registroAtual->byteOffsets[i];
            }

            // Libera a memória do registro
            apagarRegistroArvB(registroAtual);
            // Retorna o byte offset do registro
            return byteoffsetRegistro;
        // Verifica se o ID é menor que a chave atual
        } else if(id < chave) {
            // Se houver descendente, busca recursivamente
            if(descendente != -1) {
                // Libera a memória do registro
                apagarRegistroArvB(registroAtual);
                // Busca recursivamente no descendente
                return buscarRegistroRecursivo(arquivoBinario, id, descendente);
            }
            // Libera a memória do registro
            apagarRegistroArvB(registroAtual);
            // Registro não encontrado
            return -1;
        }
    }

    // Verifica se o índice do número de chaves está fora dos limites e inicializa o descendente
    if (registroAtual == NULL || numerosChaves < 0 || numerosChaves >= ORDEM_ARVORE_B) {
        descendente = -1;
    } else {
        descendente = registroAtual->descendentes[numerosChaves];
    }

    // Se houver descendente, busca recursivamente
    if(descendente != -1) {
        // Libera a memória do registro
        apagarRegistroArvB(registroAtual);
        // Busca recursivamente no descendente
        return buscarRegistroRecursivo(arquivoBinario, id, descendente);
    }
    // Libera a memória do registro
    apagarRegistroArvB(registroAtual);
    // Registro não encontrado
    return -1;
}

// Função para escrever um registro no arquivo binário
int escreverRegistroDados(DADOS *registro, int byteOffset, int tamRegistroAtual, FILE *arquivoBinario) {
    // Verifica se o registro, o byteOffset ou o arquivo binário são inválidos
    if(registro == NULL || byteOffset < 25 || arquivoBinario == NULL) {
        return 0; // Retorna 0 se alguma condição for inválida
    }

    // Inicializa variáveis locais com os valores do registro
    char removido = registro->removido; // Campo 'removido' do registro
    int tamRegistroOriginal = registro->tamanhoRegistro; // Tamanho original do registro
    int tamanhoRegistro = tamRegistroOriginal; // Inicializa o tamanho do registro com o tamanho original
    int64_t prox = registro->prox; // Próximo registro
    int id = registro->id; // ID do jogador
    int idade = registro->idade; // Idade do jogador
    int tamNomeJogador = registro->tamNomeJog; // Tamanho do nome do jogador
    char *nomeJogador;

    // Verifica se o nome do jogador é NULL ou vazio e inicializa a variável correspondente
    if (registro->nomeJogador == NULL || strcmp(registro->nomeJogador, "") == 0) {
        nomeJogador = "SEM DADO"; // Se for NULL ou vazio, atribui "SEM DADO"
    } else {
        nomeJogador = registro->nomeJogador; // Caso contrário, atribui o valor do campo nomeJogador
    }

    int tamNacionalidade = registro->tamNacionalidade; // Tamanho da nacionalidade do jogador
    char *nacionalidade;

    // Verifica se a nacionalidade é NULL ou vazia e inicializa a variável correspondente
    if (registro->nacionalidade == NULL || strcmp(registro->nacionalidade, "") == 0) {
        nacionalidade = "SEM DADO"; // Se for NULL ou vazio, atribui "SEM DADO"
    } else {
        nacionalidade = registro->nacionalidade; // Caso contrário, atribui o valor do campo nacionalidade
    }

    int tamNomeClube = registro->tamNomeClube; // Tamanho do nome do clube
    char *nomeClube;

    // Verifica se o nome do clube é NULL ou vazio e inicializa a variável correspondente
    if (registro->nomeClube == NULL || strcmp(registro->nomeClube, "") == 0) {
        nomeClube = "SEM DADO"; // Se for NULL ou vazio, atribui "SEM DADO"
    } else {
        nomeClube = registro->nomeClube; // Caso contrário, atribui o valor do campo nomeClube
    }

    // Move o ponteiro do arquivo para a posição do registro
    fseek(arquivoBinario, byteOffset, SEEK_SET);

    // Verifica se há um tamanho de registro atual diferente de 0
    if(tamRegistroAtual != 0) {
        registro->tamanhoRegistro = tamRegistroAtual; // Atualiza o tamanho do registro
        tamanhoRegistro = tamRegistroAtual; // Define o tamanho do registro para o tamanho atual
    }

    // Escreve os campos do registro no arquivo binário
    fwrite(&removido, sizeof(char), 1, arquivoBinario); // Campo 'removido'
    fwrite(&tamanhoRegistro, sizeof(int), 1, arquivoBinario); // Tamanho do registro
    fwrite(&prox, sizeof(int64_t), 1, arquivoBinario); // Próximo registro
    fwrite(&id, sizeof(int), 1, arquivoBinario); // ID do jogador
    fwrite(&idade, sizeof(int), 1, arquivoBinario); // Idade do jogador
    fwrite(&tamNomeJogador, sizeof(int), 1, arquivoBinario); // Tamanho do nome do jogador
    fwrite(nomeJogador, sizeof(char), tamNomeJogador, arquivoBinario); // Nome do jogador
    fwrite(&tamNacionalidade, sizeof(int), 1, arquivoBinario); // Tamanho da nacionalidade
    fwrite(nacionalidade, sizeof(char), tamNacionalidade, arquivoBinario); // Nacionalidade
    fwrite(&tamNomeClube, sizeof(int), 1, arquivoBinario); // Tamanho do nome do clube
    fwrite(nomeClube, sizeof(char), tamNomeClube, arquivoBinario); // Nome do clube

    // Preenche o restante do registro com '$'
    for (int i = 0; i < tamRegistroAtual - tamRegistroOriginal; i++) {
        fwrite("$", sizeof(char), 1, arquivoBinario); // Escreve '$' no arquivo
    }

    return 1; // Retorna 1 se a escrita foi bem-sucedida
}

// Função para imprimir um registro
void imprimirRegistrosDados(DADOS *registro) {
    // Verifica se o registro não foi removido
    if (registro->removido == '0') {

        // Inicializa as variáveis com os valores dos campos do registro
        char *nomeClube;
        if (registro->nomeClube == NULL || strcmp(registro->nomeClube, "") == 0) {
            nomeClube = "SEM DADO"; // Se o nome do clube for NULL ou vazio, atribui "SEM DADO"
        } else {
            nomeClube = registro->nomeClube; // Caso contrário, atribui o valor do campo nomeClube
        }

        char *nacionalidade;
        if (registro->nacionalidade == NULL || strcmp(registro->nacionalidade, "") == 0) {
            nacionalidade = "SEM DADO"; // Se a nacionalidade for NULL ou vazia, atribui "SEM DADO"
        } else {
            nacionalidade = registro->nacionalidade; // Caso contrário, atribui o valor do campo nacionalidade
        }

        char *nomeJogador;
        if (registro->nomeJogador == NULL || strcmp(registro->nomeJogador, "") == 0) {
            nomeJogador = "SEM DADO"; // Se o nome do jogador for NULL ou vazio, atribui "SEM DADO"
        } else {
            nomeJogador = registro->nomeJogador; // Caso contrário, atribui o valor do campo nomeJogador
        }

        // Imprime o nome do jogador
        printf("Nome do Jogador: ");
        if (strcmp(nomeJogador, "SEM DADO") == 0) {
            printf("SEM DADO\n"); // Se o nome do jogador for "SEM DADO", imprime "SEM DADO"
        } else {
            for (int j = 0; j < registro->tamNomeJog; j++) {
                printf("%c", nomeJogador[j]); // Imprime cada caractere do nome do jogador
            }
            printf("\n");
        }

        // Imprime a nacionalidade do jogador
        printf("Nacionalidade do Jogador: ");
        if (strcmp(nacionalidade, "SEM DADO") == 0) {
            printf("SEM DADO\n"); // Se a nacionalidade for "SEM DADO", imprime "SEM DADO"
        } else {
            for (int j = 0; j < registro->tamNacionalidade; j++) {
                printf("%c", nacionalidade[j]); // Imprime cada caractere da nacionalidade
            }
            printf("\n");
        }

        // Imprime o nome do clube do jogador
        printf("Clube do Jogador: ");
        if (strcmp(nomeClube, "SEM DADO") == 0) {
            printf("SEM DADO\n"); // Se o nome do clube for "SEM DADO", imprime "SEM DADO"
        } else {
            for (int j = 0; j < registro->tamNomeClube; j++) {
                printf("%c", nomeClube[j]); // Imprime cada caractere do nome do clube
            }
            printf("\n");
        }

        printf("\n");
        return; // Retorna 1 indicando que o registro foi impresso
    } else {
        return; // Retorna 0 indicando que o registro foi removido e não foi impresso
    }
}

// Função para criar uma lista de índices
LISTA_INDICE *criarListaIndex() {
    // Aloca memória para uma nova lista de índices
    LISTA_INDICE *lista = (LISTA_INDICE *)malloc(sizeof(LISTA_INDICE));
    lista->tamanho = 0; // Inicializa o tamanho da lista como 0
    lista->max_tamanho = 1000; // Define o tamanho máximo da lista como 1000
    // Aloca espaço para 1000 endereços de registros
    lista->registros = (DADOS_INDICE **)malloc(sizeof(DADOS_INDICE *) * lista->max_tamanho);

    return lista; // Retorna a lista de índices criada
}

// Função que libera a memória da lista de índices e de seus registros
bool apagarListaIndice(LISTA_INDICE *lista) {
    // Verifica se a lista é nula
    if (lista == NULL) return false;

    // Libera a memória de cada registro na lista
    for (int i = 0; i < lista->tamanho; i++) {
        free(lista->registros[i]);
    }
    free(lista->registros); // Libera a memória do array de registros

    free(lista); // Libera a memória da lista de índices
    return true; // Retorna true para indicar que a lista foi apagada com sucesso
}

// Função para criar um registro de índice
DADOS_INDICE *inicializaRegistroIndex() {
    // Aloca memória para um novo registro de índice
    DADOS_INDICE *registro = (DADOS_INDICE *) malloc(sizeof(DADOS_INDICE));
    // Verifica se a alocação de memória falhou
    if(!registro) return NULL;
    // Inicializa os campos do registro de índice
    registro->index = -1;
    registro->byteOffset = -1;
    return registro; // Retorna o registro de índice inicializado
}

// Função para buscar a posição de um registro de índice na lista de forma linear
int buscarPosicaoRegistro(LISTA_INDICE *lista, int id) {
    // Verifica se a lista está vazia
    if (lista->tamanho == 0) return -1;

    // Percorre a lista de forma linear para encontrar o id
    for (int i = 0; i < lista->tamanho; i++) {
        int idAtual = lista->registros[i]->index; // Obtém o id atual
        if (idAtual == id) return i; // Retorna a posição se o id for encontrado
    }

    return -1; // Se o registro não foi encontrado, retorna -1
}

// Função para obter o byte offset do best fit para um array de registros
int64_t *bestFitRegistros(REMOVIDOS *removidos, DADOS **registros, int quantidade, FILE *arquivoBinario) {
    // Aloca memória para os tamanhos e byte offsets
    int *tamanhos = malloc(sizeof(int) * quantidade);
    int64_t *byteOffsets = malloc(sizeof(int64_t) * quantidade);

    // Verifica se não há registros removidos
    if(removidos->lista->tamanho == 0) {
        // Lê o cabeçalho dos dados do arquivo
        CABECALHO_DADOS *cabecalho = lerCabecalhoDados(arquivoBinario);

        // Atualiza o número de registros no cabeçalho
        cabecalho->nroRegArq = cabecalho->nroRegArq + quantidade;

        // Define a posição do ponteiro do arquivo e escreve o número de registros
        const int gapNroRegArqByte = 17;
        fseek(arquivoBinario, gapNroRegArqByte, SEEK_SET);
        int nroRegArq = cabecalho->nroRegArq;
        fwrite(&nroRegArq, sizeof(int), 1, arquivoBinario);

        // Inicializa os byte offsets como -1
        for(int i = 0; i < quantidade; i++) {
            byteOffsets[i] = -1;
        }

        free(cabecalho); // Libera a memória do cabeçalho
        free(tamanhos); // Libera a memória dos tamanhos

        return byteOffsets; // Retorna os byte offsets
    }

    // Inicializa os tamanhos e byte offsets dos registros
    for(int i = 0; i < quantidade; i++) {
        if(registros[i]->tamanhoRegistro == 0) {
            tamanhos[i] = -1; // Define o tamanho como -1 se for inválido
            byteOffsets[i] = 0; // Define o byte offset como 0
            continue;
        }
        tamanhos[i] = registros[i]->tamanhoRegistro; // Define o tamanho do registro
    }

    // Obtém o best fit na ordem do maior para o menor
    for(int i = 0; i < quantidade; i++) {
        int maior = -1;
        int posicao = -1;

        // Encontra o maior tamanho de registro
        for(int j = 0; j < quantidade; j++) {
            if(tamanhos[j] > maior) {
                maior = tamanhos[j];
                posicao = j;
            }
        }

        if(posicao == -1) continue;

        // Obtém o byte offset do best fit e libera o espaço
        byteOffsets[posicao] = bestFitLiberarMemoria(removidos, tamanhos[posicao], registros[posicao], arquivoBinario);
        tamanhos[posicao] = -1; // Marca o tamanho como processado
    }

    free(tamanhos); // Libera a memória dos tamanhos

    return byteOffsets; // Retorna os byte offsets
}

// Função para obter o melhor ajuste (best fit) e liberar o espaço correspondente
int64_t bestFitLiberarMemoria(REMOVIDOS *removidos, int tamanho, DADOS *registro, FILE *arquivoBinario) {
    int left = 0;
    int right = removidos->lista->tamanho - 1;
    int middle = (left + right) / 2;

    // Realiza uma busca binária para encontrar o melhor ajuste
    while(left < right) {
        if(removidos->tamanhos[middle] == tamanho) {
            break; // Encontrou um tamanho igual ao desejado
        } else if(removidos->tamanhos[middle] > tamanho) {
            right = middle; // Ajusta o limite direito
        } else {
            left = middle + 1; // Ajusta o limite esquerdo
        }
        middle = (left + right) / 2; // Calcula o novo valor do meio
    }

    // Obtém o byte offset do registro removido no melhor ajuste encontrado
    int64_t byteOffset = removidos->lista->registros[middle]->byteOffset;

    // Ajusta para o primeiro elemento da lista com o tamanho desejado
    while(middle - 1 >= 0 && removidos->tamanhos[middle - 1] == tamanho) {
        if(removidos->tamanhos[middle] != removidos->tamanhos[middle - 1]) {
            break; // Encontra o primeiro tamanho igual ao desejado
        }
        int64_t byteOffsetAnterior = removidos->lista->registros[middle - 1]->byteOffset;
        byteOffset = byteOffsetAnterior; // Atualiza o byte offset
        middle--;
    }

    // Ajusta o ponteiro para o próximo registro removido, se houver
    if(middle + 1 < removidos->lista->tamanho) {
        int64_t proxByteOffset = removidos->lista->registros[middle + 1]->byteOffset;
        registro->prox = proxByteOffset; // Define o próximo byte offset
    } else {
        registro->prox = -1; // Não há registro removido depois
    }

    // Remove o registro removido e atualiza o arquivo
    atualizarRegistroRemovido(removidos, middle, arquivoBinario);

    return byteOffset; // Retorna o byte offset do melhor ajuste encontrado
}

// Função para ler um registro de árvore B de um arquivo
DADOS_ARVORE_B *lerRegistroArvB(FILE *arquivoBinario, int rrn) {
    // Cria um registro vazio de árvore B
    DADOS_ARVORE_B *registro = inicializaRegistroArvB();
    if (registro == NULL || arquivoBinario == NULL) {
        return NULL; // Verifica se a criação do registro falhou
    }

    // Calcula o byte offset do registro no arquivo
    int64_t byteOffset = (rrn + 1) * TAMANHO_DADOS_ARVORE_B;

    fseek(arquivoBinario, byteOffset, SEEK_SET); // Move o ponteiro do arquivo para a posição do registro

    // Lê os campos do registro a partir do arquivo
    fread(&registro->alturaNo, sizeof(int), 1, arquivoBinario);
    fread(&registro->nroChaves, sizeof(int), 1, arquivoBinario);
    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        fread(&registro->chaves[i], sizeof(int), 1, arquivoBinario);
        fread(&registro->byteOffsets[i], sizeof(int64_t), 1, arquivoBinario);
    }
    fread(registro->descendentes, sizeof(int), ORDEM_ARVORE_B, arquivoBinario);

    return registro; // Retorna o registro lido
}

// Função para escrever um registro de árvore B em um arquivo
int escreverRegistroArvB(DADOS_ARVORE_B *registro, FILE *arquivoBinario, int rrn) {
    if (registro == NULL || arquivoBinario == NULL) {
        return 0; // Verifica se o registro ou arquivo são nulos
    }

    // Calcula o byte offset do registro no arquivo
    int64_t byteOffset = (rrn + 1) * TAMANHO_DADOS_ARVORE_B;

    fseek(arquivoBinario, byteOffset, SEEK_SET); // Move o ponteiro do arquivo para a posição do registro

    // Escreve os campos do registro no arquivo
    fwrite(&registro->alturaNo, sizeof(int), 1, arquivoBinario);
    fwrite(&registro->nroChaves, sizeof(int), 1, arquivoBinario);
    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        fwrite(&registro->chaves[i], sizeof(int), 1, arquivoBinario);
        fwrite(&registro->byteOffsets[i], sizeof(int64_t), 1, arquivoBinario);
    }
    fwrite(registro->descendentes, sizeof(int), ORDEM_ARVORE_B, arquivoBinario);

    return 1; // Retorna 1 para indicar sucesso na escrita
}