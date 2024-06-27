#include "escreveBin.h"
#include "arvoreB.h"
#include "interpretaBin.h"
#include "definicoesTipos.h"
#include "cabecalho.h"
#include "cabecalhoArvB.h"
#include "registroArvB.h"
#include <stdio.h>
#include <stdlib.h>

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