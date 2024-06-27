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