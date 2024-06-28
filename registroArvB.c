#include "funcoesAuxiliares.h"

// cria um registro de árvore B com os valores padrao
REGISTRO_ARVORE_B *criarRegistroArvoreBVazio()
{
    REGISTRO_ARVORE_B *registro = malloc(sizeof(REGISTRO_ARVORE_B));
    registro->rrn = -1;
    registro->alturaNo = 0;
    registro->nroChaves = 0;
    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++)
    {
        registro->chaves[i] = -1;
    }
    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++)
    {
        registro->byteOffsets[i] = -1;
    }
    for (int i = 0; i < ORDEM_ARVORE_B; i++)
    {
        registro->descendentes[i] = -1;
    }

    return registro;
}

// insere uma chave no registro de árvore B
int inserirChaveRegistroArvoreB(REGISTRO_ARVORE_B *registro, int chave, long long int byteOffset)
{
    if (registro->nroChaves == ORDEM_ARVORE_B - 1) // se o registro estiver cheio
    {
        return 0;
    }
    else if (registro->nroChaves == 0) // se o registro estiver vazio
    {
        registro->chaves[0] = chave;
        registro->byteOffsets[0] = byteOffset;
        registro->nroChaves++;
    }
    else // se o registro nao estiver vazio nem cheio
    {
        int posicao = 0;
        for(int i = 0; i < registro->nroChaves; i++)
        {
            if(registro->chaves[i] == chave) // se a chave ja existe
            {
                return 0;
            }

            if(registro->chaves[i] > chave)
            {
                posicao = i;
                break;
            }

            posicao++;
        }


        if(registro->nroChaves > posicao)
        {
            for(int i = registro->nroChaves; i > posicao; i--)
            {
                registro->chaves[i] = registro->chaves[i - 1];
                registro->byteOffsets[i] = registro->byteOffsets[i - 1];
            }
        }

        /*for(int i = posicao; i < registro->nroChaves; i++)
        {
            registro->chaves[i + 1] = registro->chaves[i];
            registro->byteOffsets[i + 1] = registro->byteOffsets[i];
        }*/
        registro->chaves[posicao] = chave;
        registro->byteOffsets[posicao] = byteOffset;
        registro->nroChaves++;
    }

    return 1;
}

// remove uma chave do registro de árvore B
int removerChaveRegistroArvoreB(REGISTRO_ARVORE_B *registro, int chave)
{
    if (registro->nroChaves == 0) // se o registro estiver vazio
    {
        return 0;
    }
    else
    {
        int i = 0;
        while (i < registro->nroChaves && registro->chaves[i] != chave) // procura a chave
        {
            i++;
        }
        if (i == registro->nroChaves) // nao achou a chave
        {
            return 0;
        }
        else
        {
            for (int j = i; j < registro->nroChaves - 1; j++)
            {
                registro->chaves[j] = registro->chaves[j + 1]; // reposiciona as chaves
                registro->byteOffsets[j] = registro->byteOffsets[j + 1]; // reposiciona os byte offsets
            }
            registro->nroChaves--;
        }
    }

    return 1;
}

// insere um descendente no registro de árvore B
int inserirDescendenteRegistroArvoreB(REGISTRO_ARVORE_B *registro, long long int descendente, int chaveDescendente)
{
    if (!registro || registro->nroChaves == 0)
    {
        return 0;
    }

    int i = 0;

    //printf("primeira chave: %d\n", registro->chaves[i]);
    //printf("chave descendente: %d\n", chaveDescendente);

    while (i < registro->nroChaves && registro->chaves[i] < chaveDescendente) // acha a posicao do descendente
    {
        if(registro->chaves[i] == chaveDescendente) // se a chave ja existe
        {
            return 0;
        }
        i++;
    }

    if(registro->descendentes[i] != -1) // se a posicao ja esta ocupada
    {
        for (int j = registro->nroChaves; j > i; j--) // reposiciona os descendentes
        {
            registro->descendentes[j] = registro->descendentes[j - 1];
        }
    }

    registro->descendentes[i] = descendente; // insere o descendente na posicao em que Ci < chaveDescendente < Ci+1

    return 1;
}

int removerDescendenteRegistroArvoreB(REGISTRO_ARVORE_B *registro, long long int descendente)
{
    if (registro->nroChaves == 0)
    {
        return 0;
    }
    
    int i = 0;

    while (i < registro->nroChaves && registro->descendentes[i] != descendente) // acha a posicao do descendente
    {
        i++;
    }

    if(i == ORDEM_ARVORE_B) // nao achou o descendente
    {
        return 0;
    }

    registro->descendentes[i] = -1; // remove o descendente

    return 1;
}

int getRRNRegistroArvoreB(REGISTRO_ARVORE_B *registro)
{
    if (registro == NULL)
    {
        return -1;
    }
    
    int rrn = registro->rrn;

    return rrn;
}

int getAlturaNoRegistroArvoreB(REGISTRO_ARVORE_B *registro)
{
    if (registro == NULL)
    {
        return -1;
    }
    
    int alturaNo = registro->alturaNo;

    return alturaNo;
}

int setRRNRegistroArvoreB(REGISTRO_ARVORE_B *registro, int rrn)
{
    if (registro == NULL)
    {
        return 0;
    }
    else
    {
        registro->rrn = rrn;
    }

    return 1;
}

int setAlturaNoRegistroArvoreB(REGISTRO_ARVORE_B *registro, int alturaNo)
{
    if (registro == NULL)
    {
        return 0;
    }
    else
    {
        registro->alturaNo = alturaNo;
    }

    return 1;
}

int getNroChavesRegistroArvoreB(REGISTRO_ARVORE_B *registro)
{
    if (registro == NULL)
    {
        return -1;
    }
    
    int nroChaves = registro->nroChaves;

    return nroChaves;
}

int getChave(REGISTRO_ARVORE_B *registro, int posicao)
{
    if (registro == NULL || posicao < 0 || posicao >= ORDEM_ARVORE_B - 1)
    {
        return -1;
    }

    return registro->chaves[posicao];
}

long long int getByteOffsetRegistroArvoreB(REGISTRO_ARVORE_B *registro, int posicao)
{
    if (registro == NULL || posicao < 0 || posicao >= ORDEM_ARVORE_B - 1)
    {
        return -1;
    }

    return registro->byteOffsets[posicao];
}

int getDescendente(REGISTRO_ARVORE_B *registro, int posicao)
{
    if (registro == NULL || posicao < 0 || posicao >= ORDEM_ARVORE_B)
    {
        return -1;
    }

    return registro->descendentes[posicao];
}

int apagarRegistroArvoreB(REGISTRO_ARVORE_B *registro)
{
    if (registro == NULL)
    {
        return 0;
    }
    else
    {
        free(registro);
    }

    return 1;
}

REGISTRO_ARVORE_B *lerRegistroArvoreB(FILE *arquivo, int rrn)
{
    REGISTRO_ARVORE_B *registro = criarRegistroArvoreBVazio();
    if (registro == NULL || arquivo == NULL)
    {
        return NULL;
    }
    
    long long int byteOffset = (rrn + 1) * TAMANHO_REGISTRO_ARVORE_B;

    fseek(arquivo, byteOffset, SEEK_SET);

    fread(&registro->alturaNo, sizeof(int), 1, arquivo);
    fread(&registro->nroChaves, sizeof(int), 1, arquivo);
    for(int i = 0; i < ORDEM_ARVORE_B -1; i++)
    {
        fread(&registro->chaves[i], sizeof(int), 1, arquivo);
        fread(&registro->byteOffsets[i], sizeof(long long int), 1, arquivo);
    }
    fread(registro->descendentes, sizeof(int), ORDEM_ARVORE_B, arquivo);

    return registro;
}

int escreverRegistroArvoreB(REGISTRO_ARVORE_B *registro, FILE *arquivo, int rrn)
{
    if (registro == NULL || arquivo == NULL)
    {
        return 0;
    }
    
    long long int byteOffset = (rrn + 1) * TAMANHO_REGISTRO_ARVORE_B;

    fseek(arquivo, byteOffset, SEEK_SET);

    fwrite(&registro->alturaNo, sizeof(int), 1, arquivo);
    fwrite(&registro->nroChaves, sizeof(int), 1, arquivo);
    for(int i = 0; i < ORDEM_ARVORE_B - 1; i++)
    {
        fwrite(&registro->chaves[i], sizeof(int), 1, arquivo);
        fwrite(&registro->byteOffsets[i], sizeof(long long int), 1, arquivo);
    }
    fwrite(registro->descendentes, sizeof(int), ORDEM_ARVORE_B, arquivo);

    return 1;
}