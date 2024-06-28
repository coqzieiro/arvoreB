#include "escreveBin.h"
#include "arvoreB.h"
#include "interpretaBin.h"
#include "definicoesTipos.h"
#include "cabecalho.h"
#include "cabecalhoArvB.h"
#include "registroArvB.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void imprimirIdArvoreB(int id, FILE *file, char *nomeArquivoArvoreB, int i, int buscaMinuscula) {
    FILE *fileArvoreB = fopen(nomeArquivoArvoreB, "rb"); // Abre o arquivo da árvore B no modo leitura
    if(fileArvoreB == NULL) {
        printf("Falha no processamento do arquivo.\n"); // Verifica se ocorreu um erro ao abrir o arquivo
        return;
    }

    CABECALHO_ARVORE_B *cabecalhoArvoreB = lerCabecalhoArvoreB(fileArvoreB); // Lê o cabeçalho da árvore B
    if(getStatusCabecalhoArvoreB(cabecalhoArvoreB) == '0') {
        printf("Falha no processamento do arquivo.\n"); // Verifica se o status do cabeçalho é inválido
        apagarCabecalhoArvoreB(cabecalhoArvoreB); // Libera a memória do cabeçalho
        fclose(fileArvoreB); // Fecha o arquivo
        return;
    }

    int rrnAtual = getNoRaizCabecalhoArvoreB(cabecalhoArvoreB); // Obtém o RRN da raiz da árvore B
    apagarCabecalhoArvoreB(cabecalhoArvoreB); // Libera a memória do cabeçalho

    if(buscaMinuscula) {
        printf("Busca %d\n\n", i+1); // Imprime o número da busca
    } else {
        printf("BUSCA %d\n\n", i+1); // Imprime o número da busca em maiúsculas
    }

    if(rrnAtual != -1) {
        long long int byteOffsetRegistroBuscado = buscarRegistroIdRec(fileArvoreB, id, rrnAtual); // Busca o byte offset do registro na árvore B

        if(byteOffsetRegistroBuscado != -1) { // Se o registro foi encontrado
            DADOS *registro = buscarRegistroOffset(byteOffsetRegistroBuscado, file); // Busca o registro no arquivo binário
            imprimeRegistro(registro); // Imprime o registro
            liberarRegistro(registro); // Libera a memória do registro
        } else {
            printf("Registro inexistente.\n\n"); // Se o registro não foi encontrado
        }
    }
    else {
        printf("Registro inexistente.\n\n"); // Se a árvore B está vazia
    }

    fclose(fileArvoreB); // Fecha o arquivo da árvore B
}

long long int buscarRegistroIdRec(FILE *fileArvoreB, int id, int rrnAtual) {
    REGISTRO_ARVORE_B *registroAtual = lerRegistroArvoreB(fileArvoreB, rrnAtual); // Lê o registro da árvore B no RRN atual
    
    int numerosChaves = getNroChavesRegistroArvoreB(registroAtual); // Obtém o número de chaves no registro
    int chave;
    int descendente;

    for(int i=0; i<numerosChaves; i++) {
        chave = getChave(registroAtual, i); // Obtém a chave na posição i
        descendente = getDescendente(registroAtual, i); // Obtém o descendente na posição i

        if(id == chave) {
            long long int byteoffsetRegistro = getByteOffsetRegistroArvoreB(registroAtual, i); // Obtém o byte offset do registro
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

bool escreverRegistro(DADOS *registro, int byteOffset, int tamRegistroAtual, FILE *arquivoBin) {
    if(registro == NULL || byteOffset < 25 || arquivoBin == NULL)
    {
        return false;
    }

    char removido = get_removido(registro);
    int tamRegistroOriginal = get_tamanhoRegistro(registro);
    int tamanhoRegistro = tamRegistroOriginal;
    long long int prox = get_prox(registro);
    int id = get_id(registro);
    int idade = get_idade(registro);
    int tamNomeJogador = get_tamNomeJogador(registro);
    char *nomeJogador = get_nomeJogador(registro);
    int tamNacionalidade = get_tamNacionalidade(registro);
    char *nacionalidade = get_nacionalidade(registro);
    int tamNomeClube = get_tamNomeClube(registro);
    char *nomeClube = get_nomeClube(registro);

    fseek(arquivoBin, byteOffset, SEEK_SET); // muda o ponteiro do arquivo para a posição do registro

    if(tamRegistroAtual != 0)
    {
        set_tamanhoRegistro(registro, tamRegistroAtual);
        tamanhoRegistro = tamRegistroAtual;
    }

    fwrite(&removido, sizeof(char), 1, arquivoBin); // escreve o campo removido no arquivo
    fwrite(&tamanhoRegistro, sizeof(int), 1, arquivoBin); // escreve o campo tamanhoRegistro no arquivo
    fwrite(&prox, sizeof(long long int), 1, arquivoBin); // escreve o campo prox no arquivo
    fwrite(&id, sizeof(int), 1, arquivoBin); // escreve o campo id no arquivo
    fwrite(&idade, sizeof(int), 1, arquivoBin); // escreve o campo idade no arquivo
    fwrite(&tamNomeJogador, sizeof(int), 1, arquivoBin); // escreve o campo tamNomeJogador no arquivo
    fwrite(nomeJogador, sizeof(char), tamNomeJogador, arquivoBin); // escreve o campo nomeJogador no arquivo
    fwrite(&tamNacionalidade, sizeof(int), 1, arquivoBin); // escreve o campo tamNacionalidade no arquivo
    fwrite(nacionalidade, sizeof(char), tamNacionalidade, arquivoBin); // escreve o campo nacionalidade no arquivo
    fwrite(&tamNomeClube, sizeof(int), 1, arquivoBin); // escreve o campo tamNomeClube no arquivo
    fwrite(nomeClube, sizeof(char), tamNomeClube, arquivoBin); // escreve o campo nomeClube no arquivo

    for (int i = 0; i < tamRegistroAtual - tamRegistroOriginal; i++)
    {
        fwrite("$", sizeof(char), 1, arquivoBin); // preenche o registro com '$'
    }

    return true;
}

int imprimeRegistro(DADOS *registro)
{
    if (get_removido(registro) == '0')
    { // se o registro não foi removido, imprime seus dados na tela

        // recebe o valor dos atributos do registro
        char *nomeClube = get_nomeClube(registro);
        char *nacionalidade = get_nacionalidade(registro);
        char *nomeJogador = get_nomeJogador(registro);

        printf("Nome do Jogador: ");
        if (strcmp(nomeJogador, "SEM DADO") == 0) // se o nome do jogador for "SEM DADO", imprime "SEM DADO"
        {
            printf("SEM DADO\n");
        }
        else // se não, imprime cada caractere do nome do jogador
        {
            for (int j = 0; j < get_tamNomeJogador(registro); j++)
            {
                printf("%c", nomeJogador[j]);
            }
            printf("\n");
        }

        printf("Nacionalidade do Jogador: ");
        if (strcmp(nacionalidade, "SEM DADO") == 0) // se a nacionalidade for "SEM DADO", imprime "SEM DADO"
        {
            printf("SEM DADO\n");
        }
        else // se não, imprime cada caractere da nacionalidade
        {
            for (int j = 0; j < get_tamNacionalidade(registro); j++)
            {
                printf("%c", nacionalidade[j]);
            }
            printf("\n");
        }

        printf("Clube do Jogador: ");
        if (strcmp(nomeClube, "SEM DADO") == 0) // se o nome do clube for "SEM DADO", imprime "SEM DADO"
        {
            printf("SEM DADO\n");
        }
        else // se não, imprime cada caractere do nome do clube
        {
            for (int j = 0; j < get_tamNomeClube(registro); j++)
            {
                printf("%c", nomeClube[j]);
            }
            printf("\n");
        }
        printf("\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

int criarArquivoArvoreB(char *arquivoBin, char *arquivoArvB){
    FILE *arquivoBinario = fopen(arquivoBin, "rb"); // abre o arquivo binário no modo escrita
    FILE *arquivoArvoreB = fopen(arquivoArvB, "wb+"); // abre o arquivo de índices no modo escrita

    if(!arquivoBinario || !arquivoArvoreB)
    {
        printf("Falha no processamento do arquivo.\n");
        if(arquivoBinario) fclose(arquivoBinario);
        if(arquivoArvoreB) fclose(arquivoArvoreB);
        return 0;
    }

    CABECALHO *cabecalho = getCabecalhoFromBin(arquivoBinario); // lê o cabeçalho do arquivo binário

    if(getStatus(cabecalho) == '0')
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBinario);
        fclose(arquivoArvoreB);
        apagarCabecalho(cabecalho);
        return 0;
    }

    CABECALHO_ARVORE_B *cabecalhoArvoreB = criarCabecalhoArvoreBVazio(); // cria um cabeçalhos
    setStatusCabecalhoArvoreB(cabecalhoArvoreB, '0');
    escreverCabecalhoArvoreB(arquivoArvoreB, cabecalhoArvoreB); // escreve o cabeçalho no arquivo

    // pula o cabecalho
    long long int posicao = 25;

    //REGISTRO_INDICE *registroIndice = criarRegistroIndice();

    // quantidade de registros no arquivo
    int quantidade = getNroRegArq(cabecalho) + getNroRem(cabecalho);

    apagarCabecalho(cabecalho);

    for(int i = 0; i < quantidade; i++)
    {
        DADOS *registro = lerRegistroFromBin(posicao, arquivoBinario); // lê um registro do arquivo binário

        if(get_removido(registro) == '1') //pula o registro removido
        {
            posicao += get_tamanhoRegistro(registro);
            continue;
        }

        int chave = get_id(registro); // obtém a chave do registro
        long long int byteOffset = posicao; // obtém o byteOffset do registro
        
        inserirArvoreB(arquivoArvoreB, chave, byteOffset); // insere o registro na árvore B

        posicao += get_tamanhoRegistro(registro); // muda o byteOffset para a posição do próximo registro
        liberarRegistro(registro); // libera a memória do registro
    }

    //imprimirArvoreBGraphviz(arquivoArvoreB); // imprime a árvore B

    apagarCabecalhoArvoreB(cabecalhoArvoreB); // libera a memória do cabeçalho da árvore B
    cabecalhoArvoreB = lerCabecalhoArvoreB(arquivoArvoreB); // lê o cabeçalho da árvore B
    setStatusCabecalhoArvoreB(cabecalhoArvoreB, '1'); // define o status do cabeçalho como consistente
    escreverCabecalhoArvoreB(arquivoArvoreB, cabecalhoArvoreB); // escreve o cabeçalho no arquivo
    apagarCabecalhoArvoreB(cabecalhoArvoreB); // libera a memória do cabeçalho da árvore B

    fclose(arquivoBinario); // fecha o arquivo binário
    fclose(arquivoArvoreB); // fecha o arquivo de índices

    return 1;
}

void imprimeRegistrosBuscados(char *arquivo, int buscaId, char *nomeArquivoArvoreB) {
    int numOperacoes;
    scanf("%d", &numOperacoes); // Lê o número de buscas a serem feitas

    for (int i = 0; i < numOperacoes; i++)
    {
        FILE *file = fopen(arquivo, "rb"); // Abre o arquivo binário no modo leitura
        if (file == NULL)
        {
            printf("Falha no processamento do arquivo."); // Verifica se ocorreu um erro ao abrir o arquivo
            return;
        }

        // Cria um cabeçalho e chama a função getCabecalhoFromBin para atribuir os valores a ele
        CABECALHO *cabecalho = getCabecalhoFromBin(file);

        imprimirRegistrosPorCampos(file, cabecalho, buscaId, nomeArquivoArvoreB, i); // Chama a função para imprimir registros
        
        fclose(file); // Fecha o arquivo
        apagarCabecalho(cabecalho); // Libera a memória do cabeçalho
    }
}

void imprimirRegistrosPorCampos(FILE *file, CABECALHO *cabecalho, int buscaId, char *nomeArquivoArvoreB, int i) {
    long long int byteOffset = getProxByteOffset(cabecalho);
    int numRegistros = getNroRegArq(cabecalho) + getNroRem(cabecalho); // Número total de registros (incluindo removidos)
    byteOffset = 25; // Posição inicial do byteOffset

    if (numRegistros == 0) // Verifica se o arquivo não possui registros
    {
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

    for (int j = 0; j < m; j++)
    {
        scanf("%s", campos[j]); // Lê um parâmetro da busca
        if (strcmp(campos[j], "id") == 0)
        {
            scanf("%i", &id); // Lê o id da busca
            snprintf(parametros[j], 50, "%i", id);
            if(buscaId == 1 && id != -1) { // Verifica se a busca pelo índice vai ser feita com o arquivo da árvore B
                imprimirIdArvoreB(id, file, nomeArquivoArvoreB, i, 1); // Chama a função para buscar o id na árvore B
                return;
            }
        }
        else if (strcmp(campos[j], "nomeJogador") == 0)
        {
            scan_quote_string(nome); // Lê o nome do jogador
            strcpy(parametros[j], nome);
        }
        else if (strcmp(campos[j], "idade") == 0)
        {
            scanf("%i", &idade); // Lê a idade
            snprintf(parametros[j], 50, "%i", idade);
        }
        else if (strcmp(campos[j], "nomeClube") == 0)
        {
            scan_quote_string(nomeClube); // Lê o nome do clube
            strcpy(parametros[j], nomeClube);
        }
        else if (strcmp(campos[j], "nacionalidade") == 0)
        {
            scan_quote_string(nacionalidade); // Lê a nacionalidade
            strcpy(parametros[j], nacionalidade);
        }
        else
        {
            printf("Campo invalido\n"); // Parâmetro inválido
        }
    }

    printf("Busca %d\n\n", i+1); // Imprime o número da busca

    for (int j = 0; j < numRegistros; j++)
    {
        DADOS *registro = lerRegistroFromBin(byteOffset, file); // Lê um registro do arquivo binário
        byteOffset += get_tamanhoRegistro(registro); // Atualiza o byteOffset para a posição do próximo registro

        int imprimir = 1; // Flag para determinar se o registro deve ser impresso
        if (get_removido(registro) == '1') // Verifica se o registro está marcado como removido
        {
            imprimir = 0;
        }
        else
        {
            for (int k = 0; k < m; k++)
            {
                if (strcmp(campos[k], "id") == 0)
                { // Verifica se o parâmetro da busca é o id
                    if (id != get_id(registro))
                    {
                        imprimir = 0;
                    }
                }
                else if (strcmp(campos[k], "nomeJogador") == 0)
                {
                    if (strcmp(nome, get_nomeJogador(registro)) != 0)
                    {
                        imprimir = 0;
                    }
                }
                else if (strcmp(campos[k], "idade") == 0)
                {
                    if (idade != get_idade(registro))
                    {
                        imprimir = 0;
                    }
                }
                else if (strcmp(campos[k], "nomeClube") == 0)
                {
                    if (strcmp(nomeClube, get_nomeClube(registro)) != 0)
                    {
                        imprimir = 0;
                    }
                }
                else if (strcmp(campos[k], "nacionalidade") == 0)
                {
                    if (strcmp(nacionalidade, get_nacionalidade(registro)) != 0)
                    {
                        imprimir = 0;
                    }
                }
            }
        }
        
        if (imprimir == 1) // Se todos os parâmetros coincidem, imprime o registro
        {
            imprimeRegistro(registro); // Chama a função para imprimir o registro
            impressoes++;
        }
        liberarRegistro(registro); // Libera a memória do registro
    }
    if(impressoes == 0) { // Se nenhum registro foi impresso
        printf("Registro inexistente.\n\n");
    }
}

// cria uma lista de registros removidos a partir de um arquivo binario
REMOVIDOS *criarListaRemovidos(FILE *file) {
  CABECALHO *cabecalho = getCabecalhoFromBin(file);
  REMOVIDOS *removidos = criarListaRemovidosVazia();

  fseek(file, 0, SEEK_END);
  int finalArquivo = ftell(file);

  int proxByteOffset = getTopo(cabecalho);
  
  int count = 0;

  // percorre todos os registros removidos
  while(proxByteOffset != -1 && proxByteOffset < finalArquivo) {
    DADOS *registro = lerRegistroFromBin(proxByteOffset, file);

    count++;

    if(get_removido(registro) == '1') {
      REGISTRO_INDICE *registroIndice = criarRegistroIndice();
      setIndexRegistroIndice(registroIndice, get_id(registro));
      setByteOffsetRegistroIndice(registroIndice, proxByteOffset);

      adicionarRegistroRemovido(removidos, registroIndice, get_tamanhoRegistro(registro));
    }

    proxByteOffset = get_prox(registro);

    liberarRegistro(registro);

    DADOS *proxRegistro;

    if(proxByteOffset != -1 && proxByteOffset < finalArquivo)
      proxRegistro = lerRegistroFromBin(proxByteOffset, file);

    // anotar ultimo reg removido
    if(get_prox(proxRegistro) == -1 && get_removido(proxRegistro) == '1') {
      REGISTRO_INDICE *registroIndice = criarRegistroIndice();
      setIndexRegistroIndice(registroIndice, get_id(proxRegistro));
      setByteOffsetRegistroIndice(registroIndice, proxByteOffset);

      adicionarRegistroRemovido(removidos, registroIndice, get_tamanhoRegistro(proxRegistro));
      liberarRegistro(proxRegistro);
      break;
    }

    liberarRegistro(proxRegistro);
  }

  apagarCabecalho(cabecalho);

  return removidos;
}

REMOVIDOS *criarListaRemovidosVazia() {
  REMOVIDOS *removidos = malloc(sizeof(REMOVIDOS));
  removidos->lista = criarListaIndice();
  removidos->tamanhos = malloc(sizeof(int) * 1000);

  return removidos;
}

void removerRegistroRemovidoEAtualizarArquivo(REMOVIDOS *removidos, int posicao, FILE *file) {
  DADOS *registro = buscarRegistroOffset(getByteOffsetRegistroIndice(getRegistroIndice(removidos->lista, posicao)) ,file);

  if(posicao == -1) {
    return;
  }

  int tamanhoLista = getTamanhoListaIndice(removidos->lista);
  const int byteProx = 5;
  CABECALHO *cabecalho = getCabecalhoFromBin(file);

  setStatus(cabecalho, '0');

  setNroRegArq(cabecalho, getNroRegArq(cabecalho) + 1);
  writeNroRegArqCabecalho(cabecalho, file);
  setNroRem(cabecalho, getNroRem(cabecalho) - 1);
  writeNroRegRemCabecalho(cabecalho, file);
  
  if(tamanhoLista == 1) { // lista so tem um elemento removido
    setTopo(cabecalho, -1);
    writeTopoCabecalho(cabecalho, file);
  }
  else if(posicao == 0) // removendo o primeiro elemento
  {
    REGISTRO_INDICE *registroIndice = getRegistroIndice(removidos->lista, 1);
    long long int byteOffset = getByteOffsetRegistroIndice(registroIndice);

    setTopo(cabecalho, byteOffset);
    writeTopoCabecalho(cabecalho, file);
  }
  else if(posicao == tamanhoLista - 1) // removendo o ultimo elemento
  {
    REGISTRO_INDICE *registroIndice = getRegistroIndice(removidos->lista, posicao - 1);
    long long int byteOffset = getByteOffsetRegistroIndice(registroIndice);

    int prox = -1;
    
    byteOffset += byteProx;
    fseek(file, byteOffset, SEEK_SET);
    fwrite(&prox, sizeof(int), 1, file);
  }
  else
  {
    REGISTRO_INDICE *registroIndiceAnterior = getRegistroIndice(removidos->lista, posicao - 1);
    REGISTRO_INDICE *registroIndiceProximo = getRegistroIndice(removidos->lista, posicao + 1);

    long long int byteOffsetAnterior = getByteOffsetRegistroIndice(registroIndiceAnterior);
    long long int byteOffsetProximo = getByteOffsetRegistroIndice(registroIndiceProximo);

    byteOffsetAnterior += byteProx;
    fseek(file, byteOffsetAnterior, SEEK_SET);
    fwrite(&byteOffsetProximo, sizeof(int), 1, file);
  }

  liberarRegistro(registro);
  apagarCabecalho(cabecalho);

  removerRegistroRemovidoPosicao(removidos, posicao);
}

void apagarListaRemovidos(REMOVIDOS *removidos) {
  apagarListaIndice(removidos->lista);
  free(removidos->tamanhos);
  free(removidos);
}

LISTA_INDICE *criarListaIndice() {
    LISTA_INDICE *lista = (LISTA_INDICE *)malloc(sizeof(LISTA_INDICE));
    lista->tamanho = 0;
    lista->max_tamanho = 1000;
    lista->registros = (REGISTRO_INDICE **)malloc(sizeof(REGISTRO_INDICE *) * lista->max_tamanho); // aloca espaço para 1000 endereços de registros

    return lista;
}

// Função que libera a memória da lista e de seus registros
bool apagarListaIndice(LISTA_INDICE *lista) {
    if (lista == NULL)
        return false;

    for (int i = 0; i < lista->tamanho; i++)
    {
        apagarRegistroIndice(lista->registros[i]);
    }
    free(lista->registros);

    free(lista);
    return 1;
}

REGISTRO_INDICE *criarRegistroIndice() {
    REGISTRO_INDICE *registro = (REGISTRO_INDICE *) malloc(sizeof(REGISTRO_INDICE));
    if(!registro)
        return NULL;
    registro->index = -1;
    registro->byteOffset = -1;
    return registro;
}

void removerRegistroIndice(LISTA_INDICE *lista, int index) {
    apagarRegistroIndice(lista->registros[index]); // apaga o registro

    // desloca todos os registros depois do registro a ser removido para a esquerda
    for (int i = index; i < lista->tamanho - 1; i++)
    {
        lista->registros[i] = lista->registros[i + 1];
    }

    lista->tamanho--;
}

void removerRegistroRemovidoPosicao(REMOVIDOS *removidos, int posicao) {
  removerRegistroIndice(removidos->lista, posicao);

  for(int i = posicao; i < getTamanhoListaIndice(removidos->lista); i++) {
    removidos->tamanhos[i] = removidos->tamanhos[i + 1];
  }
}

void apagarRegistroIndice(REGISTRO_INDICE *registro) {
    free(registro);
}

void setIndexRegistroIndice(REGISTRO_INDICE *registro, int index) {
    registro->index = index;
}

void setByteOffsetRegistroIndice(REGISTRO_INDICE *registro, long long int byteOffset) {
    registro->byteOffset = byteOffset;
}

void setRegistroListaIndice(LISTA_INDICE *lista, int index, REGISTRO_INDICE *registro) {
    lista->registros[index] = registro;
}

void setTamanho(LISTA_INDICE *lista, int tamanho) {
    lista->tamanho = tamanho;
}

REGISTRO_INDICE *getRegistroIndice(LISTA_INDICE *lista, int index) {
    return lista->registros[index]; // retorna o registro de determinado index da lista
}

int buscarPosicaoRegistroIndiceLinear(LISTA_INDICE *lista, int id) {
    // Verifica se a lista está vazia
    if (lista->tamanho == 0) {
        return -1;
    }

    // Percorre a lista de forma linear para encontrar o id
    for (int i = 0; i < lista->tamanho; i++) {
        int idAtual = getIndexRegistroIndice(getRegistroIndice(lista, i));
        if (idAtual == id) {
            return i; // id encontrado
        }
    }

    return -1; // Se o registro não foi encontrado, retorna -1
}

// adiciona um novo registro na lista em ordem de tamanho
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

void shiftElementosListaRemovidosRight(REMOVIDOS *removidos, int pos) {
  for(int i = getTamanhoListaIndice(removidos->lista); i > pos; i--) {
    REGISTRO_INDICE *registro = getRegistroIndice(removidos->lista, i - 1);
    setRegistroListaIndice(removidos->lista, i, registro);
    removidos->tamanhos[i] = removidos->tamanhos[i - 1];
  }
}

// oq isso faz
long long int getMaiorByteOffsetMenorQue(REMOVIDOS *removidos, int id) {
    // Busca a posição do registro com o id fornecido
    int posicao = buscarPosicaoRegistroIndiceLinear(removidos->lista, id);
    if(posicao <= 0) {
      return -1;
    } else {
      REGISTRO_INDICE *registroIndice = getRegistroIndice(removidos->lista, posicao-1);
      long long int byteOffset = getByteOffsetRegistroIndice(registroIndice);
      return byteOffset;
    }
}

int getTamanhoListaIndice(LISTA_INDICE *lista) {
    return lista->tamanho;
}

int getIndexRegistroIndice(REGISTRO_INDICE *registro) {
    return registro->index;
}

int getTamanhoListaRemovidos(REMOVIDOS *removidos) {
  return getTamanhoListaIndice(removidos->lista);
}

long long int getByteOffsetRegistroIndice(REGISTRO_INDICE *registro) {
    return registro->byteOffset;
}

// retorna o byteOffset do best fit de cada registro de um vetor
long long int *getBestFitArrayRegistros(REMOVIDOS *removidos, DADOS **registros, int quantidade, FILE *file)
{
  int *tamanhos = malloc(sizeof(int) * quantidade);
  long long int *byteOffsets = malloc(sizeof(long long int) * quantidade);

  // se nao tem nenhum registro removido
  if(getTamanhoListaRemovidos(removidos) == 0)
  {
    CABECALHO *cabecalho = getCabecalhoFromBin(file);

    setNroRegArq(cabecalho, getNroRegArq(cabecalho) + quantidade);
    writeNroRegArqCabecalho(cabecalho, file);

    for(int i = 0; i < quantidade; i++)
    {
      byteOffsets[i] = -1;
    }

    apagarCabecalho(cabecalho);

    free(tamanhos);

    return byteOffsets;
  }

  for(int i = 0; i < quantidade; i++) {
    if(get_tamanhoRegistro(registros[i]) == 0) {
      tamanhos[i] = -1;
      byteOffsets[i] = 0;
      continue;
    }
    tamanhos[i] = get_tamanhoRegistro(registros[i]);
  }

  // pegue o bestFit na ordem do maior para o menor
  for(int i = 0; i < quantidade; i++) {
    int maior = -1;
    int posicao = -1;

    for(int j = 0; j < quantidade; j++) {
      if(tamanhos[j] > maior) {
        maior = tamanhos[j];
        posicao = j;
      }
    }

    if(posicao == -1)
    {
      continue;
    }
    byteOffsets[posicao] = getBestFitAndFreeSpace(removidos, tamanhos[posicao], registros[posicao], file);
    tamanhos[posicao] = -1;
  }

  free(tamanhos);

  return byteOffsets;
}

bool inserirNovoDadoArvoreB(char *arquivoBinario, char *arquivoArvoreB, int numOperacoes) {
    FILE *arquivoBin = fopen(arquivoBinario, "rb+");
    FILE *fileArvoreB = fopen(arquivoArvoreB, "rb+");

    if (arquivoBin == NULL || fileArvoreB == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        if(arquivoBin != NULL)
            fclose(arquivoBin);
        if(fileArvoreB != NULL)
            fclose(fileArvoreB);
        return false;
    }
    CABECALHO *cabecalho = getCabecalhoFromBin(arquivoBin);
    CABECALHO_ARVORE_B *cabecalhoArvoreB = lerCabecalhoArvoreB(fileArvoreB); // Lê o cabeçalho da árvore B

    if(getStatus(cabecalho) == '0' || getStatusCabecalhoArvoreB(cabecalhoArvoreB) == '0')
    {
        printf("Falha no processamento do arquivo.\n");
        apagarCabecalho(cabecalho);
        apagarCabecalhoArvoreB(cabecalhoArvoreB);
        fclose(arquivoBin);
        fclose(fileArvoreB);
        return false;
    }

    REMOVIDOS *removidos = criarListaRemovidos(arquivoBin);
    DADOS **registros = malloc(sizeof(DADOS*) * numOperacoes);

    int id;
    char idadeStr[10];
    char **nomeJogador = malloc(sizeof(char*) * numOperacoes);
    char **nacionalidade = malloc(sizeof(char*) * numOperacoes);
    char **nomeClube = malloc(sizeof(char*) * numOperacoes);

    for(int i = 0; i < numOperacoes; i++)
    {
        nomeJogador[i] = malloc(sizeof(char) * 50);
        nacionalidade[i] = malloc(sizeof(char) * 50);
        nomeClube[i] = malloc(sizeof(char) * 50);
    }

    for(int i = 0; i < numOperacoes; i++)
    {
        // le o input do usuario
        scanf("%i", &id);
        scan_quote_string(idadeStr);
        scan_quote_string(nomeJogador[i]);
        scan_quote_string(nacionalidade[i]);
        scan_quote_string(nomeClube[i]);

        int rrnAtual = getNoRaizCabecalhoArvoreB(cabecalhoArvoreB); // Obtém o RRN da raiz da árvore B
        long long int byteoffsetRegistro = buscarRegistroIdRec(fileArvoreB, id, rrnAtual);

        if(byteoffsetRegistro != -1) // registro ja existe
        {
            nomeJogador[i] = '\0';
            nacionalidade[i] = '\0';
            nomeClube[i] = '\0';
            registros[i] = criarRegistro('1',
                                         0,
                                         0,
                                         0,
                                         0,
                                         0,
                                         nomeJogador[i],
                                         0,
                                         nomeClube[i],
                                         0,
                                         nacionalidade[i]);
            continue;
        }

        int idade = -1;

        if(strcmp(idadeStr, "NULO") == 0 || strcmp(idadeStr, "") == 0)
        {
            idade = -1;
        }
        else
        {
            idade = atoi(idadeStr);
        }

        if(strcmp(nomeJogador[i], "NULO") == 0 || strcmp(nomeJogador[i], "") == 0)
        {
            strcpy(nomeJogador[i], "");
        }

        if(strcmp(nacionalidade[i], "NULO") == 0 || strcmp(nacionalidade[i], "") == 0)
        {
            strcpy(nacionalidade[i], "");
        }

        if(strcmp(nomeClube[i], "NULO") == 0 || strcmp(nomeClube[i], "") == 0)
        {
            strcpy(nomeClube[i], "");
        }

        registros[i] = criarRegistro('0', 
                                     33 + strlen(nomeJogador[i]) + strlen(nomeClube[i]) + strlen(nacionalidade[i]), 
                                     -1,
                                     id,
                                     idade,
                                     strlen(nomeJogador[i]),
                                     nomeJogador[i],
                                     strlen(nacionalidade[i]),
                                     nacionalidade[i], 
                                     strlen(nomeClube[i]),
                                     nomeClube[i]);
    }

    apagarCabecalhoArvoreB(cabecalhoArvoreB); // Libera a memória do cabeçalho

    // pega o byteOffset do best fit de cada registro
    long long int *byteOffsets = getBestFitArrayRegistros(removidos, registros, numOperacoes, arquivoBin);
    int tamanhoRegistroAtual = 0;

    for(int i = 0; i < numOperacoes; i++)
    {
        if(byteOffsets[i] == 0) // registro ja existe
        {
            continue;
        }
        if(byteOffsets[i] == -1) // registro insere no fim
        {
            tamanhoRegistroAtual = 0;
            fseek(arquivoBin, 0, SEEK_END);
            byteOffsets[i] = ftell(arquivoBin);
            setProxByteOffset(cabecalho, byteOffsets[i] + get_tamanhoRegistro(registros[i]));
            writeProxByteOffsetCabecalho(cabecalho, arquivoBin);
        }
        else
        {
            DADOS *registro = lerRegistroFromBin(byteOffsets[i], arquivoBin);
            tamanhoRegistroAtual = get_tamanhoRegistro(registro);
            liberarRegistro(registro);
        }

        set_prox(registros[i], -1);

        // escrevendo arquivo
        setStatus(cabecalho, '0');
        writeStatusCabecalho(cabecalho, arquivoBin);

        // escreve os dados do registro no arquivoBin
        escreverRegistro(registros[i], byteOffsets[i], tamanhoRegistroAtual, arquivoBin);

        // terminou de escrever o arquivo
        setStatus(cabecalho, '1');
        writeStatusCabecalho(cabecalho, arquivoBin);

        // atualiza o status do arquivo da arvore b para '0'
        fseek(fileArvoreB, 0, SEEK_SET);
        char statusArquivoArvoreB = '0';
        fwrite(&statusArquivoArvoreB, sizeof(char), 1, fileArvoreB);

        // insere a chave e o byteoffset no arquivo da arvore b
        inserirArvoreB(fileArvoreB, get_id(registros[i]), byteOffsets[i]);

        // atualiza o status do arquivo da arvore b para '1'
        fseek(fileArvoreB, 0, SEEK_SET);
        statusArquivoArvoreB = '1';
        fwrite(&statusArquivoArvoreB, sizeof(char), 1, fileArvoreB);
    }

    for(int i = 0; i < numOperacoes; i++)
    {
        liberarRegistro(registros[i]);
    }

    free(registros);
    free(byteOffsets);

    free(nomeJogador);
    free(nacionalidade);
    free(nomeClube);

    apagarCabecalho(cabecalho);
    apagarListaRemovidos(removidos);

    fclose(arquivoBin);
    fclose(fileArvoreB);

    return true;
}

long long int getBestFitAndFreeSpace(REMOVIDOS *removidos, int tamanho, DADOS *registro, FILE *file) {
  int left = 0;
  int right = getTamanhoListaIndice(removidos->lista) - 1;

  int middle = (left + right) / 2;

  // busca binaria do tamanho
  while(left < right) {
    if(removidos->tamanhos[middle] == tamanho)
    {
      break;
    }
    else if(removidos->tamanhos[middle] > tamanho) {
      right = middle;
    } else {
      left = middle + 1;
    }

    middle = (left + right) / 2;
  }

  long long int byteOffset = getByteOffsetRegistroIndice(getRegistroIndice(removidos->lista, middle));

  // retorna o primeiro elemento da lista com o tamanho desejado
  while(middle - 1 >= 0 && removidos->tamanhos[middle - 1] == tamanho) {
    if(removidos->tamanhos[middle] != removidos->tamanhos[middle - 1]) {
      break;
    }

    long long int byteOffsetAnterior = getByteOffsetRegistroIndice(getRegistroIndice(removidos->lista, middle - 1));
    byteOffset = byteOffsetAnterior;

    middle--;
  }

  if(middle + 1 < getTamanhoListaIndice(removidos->lista)) { // tem um registro removido depois
    long long int proxByteOffset = getByteOffsetRegistroIndice(getRegistroIndice(removidos->lista, middle + 1));
    set_prox(registro, proxByteOffset);
  }
  else { // nao tem registro removido depois
    set_prox(registro, -1);
  }

  removerRegistroRemovidoEAtualizarArquivo(removidos, middle, file);

  return byteOffset;
}

bool adicionarNoArvoreB(int chave, long long int byteOffset, FILE *arquivoArvoreB) {
    // Read the B-tree header
    CABECALHO_ARVORE_B *cabecalho = lerCabecalhoArvoreB(arquivoArvoreB);
    if (getStatusCabecalhoArvoreB(cabecalho) == '0') {
        // If the header indicates the tree is inconsistent, return false
        printf("Falha no processamento do arquivo.\n");
        apagarCabecalhoArvoreB(cabecalho);
        return false;
    }

    int rrnRaiz = getNoRaizCabecalhoArvoreB(cabecalho);
    if (rrnRaiz == -1) {
        // If the tree is empty, create a new root node
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
        apagarCabecalhoArvoreB(cabecalho);
        return true;
    }

    // Initialize path array to track the path taken in the tree
    REGISTRO_ARVORE_B **caminho = (REGISTRO_ARVORE_B **)malloc(sizeof(REGISTRO_ARVORE_B *));
    int tamCaminho = 0;

    // Call the recursive insertion function
    insercaoArvoreBRecursiva(arquivoArvoreB, cabecalho, chave, byteOffset, rrnRaiz, caminho, 0, &tamCaminho);

    // Update and write the header back
    setStatusCabecalhoArvoreB(cabecalho, '1');
    escreverCabecalhoArvoreB(arquivoArvoreB, cabecalho);
    apagarCabecalhoArvoreB(cabecalho);

    // Clean up
    for (int i = 0; i < tamCaminho; i++) {
        apagarRegistroArvoreB(caminho[i]);
    }
    free(caminho);

    return true;
}