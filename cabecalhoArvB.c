#include "cabecalhoArvB.h"
#include "registroArvB.h"

// cria um cabecalho da arvore B com os valores padrao
CABECALHO_ARVORE_B *criarCabecalhoArvoreBVazio()
{
    CABECALHO_ARVORE_B *cabecalho = malloc(sizeof(CABECALHO_ARVORE_B));
    cabecalho->status = '0';
    cabecalho->noRaiz = 0;
    cabecalho->proxRRN = 0;
    cabecalho->nroChaves = 0;

    return cabecalho;
}

char getStatusCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho)
{
    if (cabecalho == NULL)
    {
        return '$';
    }

    return cabecalho->status;
}

int getNoRaizCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho)
{
    if (cabecalho == NULL)
    {
        return -1;
    }

    return cabecalho->noRaiz;
}

int getProxRRNCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho)
{
    if (cabecalho == NULL)
    {
        return -1;
    }

    return cabecalho->proxRRN;
}

int getNroChavesCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho)
{
    if (cabecalho == NULL)
    {
        return -1;
    }

    return cabecalho->nroChaves;
}

int setStatusCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, char status)
{
    if (cabecalho == NULL)
    {
        return 0;
    }

    cabecalho->status = status;

    return 1;
}

int setNoRaizCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int noRaiz)
{
    if (cabecalho == NULL)
    {
        return 0;
    }

    cabecalho->noRaiz = noRaiz;

    return 1;
}

int setProxRRNCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int proxRRN)
{
    if (cabecalho == NULL)
    {
        return 0;
    }

    cabecalho->proxRRN = proxRRN;

    return 1;
}

int setNroChavesCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int nroChaves)
{
    if (cabecalho == NULL)
    {
        return 0;
    }

    cabecalho->nroChaves = nroChaves;

    return 1;
}

// cria um cabecalho da arbore B a partir de um arquivo
CABECALHO_ARVORE_B *lerCabecalhoArvoreB(FILE *file)
{
    if (file == NULL)
    {
        return NULL;
    }

    CABECALHO_ARVORE_B *cabecalho = malloc(sizeof(CABECALHO_ARVORE_B));

    fseek(file, 0, SEEK_SET);

    fread(&cabecalho->status, sizeof(char), 1, file);
    fread(&cabecalho->noRaiz, sizeof(int), 1, file);
    fread(&cabecalho->proxRRN, sizeof(int), 1, file);
    fread(&cabecalho->nroChaves, sizeof(int), 1, file);

    return cabecalho;
}

// escreve em um arquivo o cabecalho da Arvore B
int escreverCabecalhoArvoreB(FILE *file, CABECALHO_ARVORE_B *cabecalho)
{
    if (file == NULL || cabecalho == NULL)
    {
        return 0;
    }

    fseek(file, 0, SEEK_SET);

    int tamanhoCabecalho = sizeof(char) + sizeof(int) * 3;

    fwrite(&cabecalho->status, sizeof(char), 1, file);
    fwrite(&cabecalho->noRaiz, sizeof(int), 1, file);
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, file);
    fwrite(&cabecalho->nroChaves, sizeof(int), 1, file);

    char lixo = '$';
    for(int i = tamanhoCabecalho; i < TAMANHO_REGISTRO_ARVORE_B; i++)
    {
        fwrite(&lixo, sizeof(char), 1, file); // lixo para o cabecalho ter o mesmo tamanho do registro
    }

    return 1;
}

int apagarCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho)
{
    if (cabecalho == NULL)
    {
        return 0;
    }

    free(cabecalho);

    return 1;
}

int imprimirCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho)
{
    if (cabecalho == NULL)
    {
        return 0;
    }

    printf("Status: %c\n", cabecalho->status);
    printf("No raiz: %d\n", cabecalho->noRaiz);
    printf("Prox RRN: %d\n", cabecalho->proxRRN);
    printf("Nro chaves: %d\n", cabecalho->nroChaves);

    return 1;
}