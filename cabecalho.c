/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "definicoesTipos.h"
#include "funcoesAuxiliares.h"

// Função que cria o cabeçalho e define os valores iniciais de seus campos
CABECALHO *criarCabecalho(void)
{
    CABECALHO *cabecalho = (CABECALHO *)malloc(sizeof(CABECALHO));
    cabecalho->status = '1';
    cabecalho->topo = -1;
    cabecalho->proxByteOffset = 0;
    cabecalho->nroRegArq = 0;
    cabecalho->nroRegRem = 0;

    return cabecalho;
}

// retorna o valor do status do cabeçalho
char getStatus(CABECALHO *cabecalho)
{
    return cabecalho->status;
}

// retorna o valor do topo do cabeçalho
long long int getTopo(CABECALHO *cabecalho)
{
    return cabecalho->topo;
}

// retorna o valor do proxByteOffset do cabeçalho
long long int getProxByteOffset(CABECALHO *cabecalho)
{
    return cabecalho->proxByteOffset;
}

// retorna o valor do número de registros não removidos do cabeçalho
int getNroRegArq(CABECALHO *cabecalho)
{
    return cabecalho->nroRegArq;
}

// retorna o valor do número de registros removidos do cabeçalho
int getNroRem(CABECALHO *cabecalho)
{
    return cabecalho->nroRegRem;
}

// Funções de set para atribuir um valor aos campos do cabeçalho de acordo com os valores passados como parâmetro nas funções

void setStatus(CABECALHO *cabecalho, char status)
{
    cabecalho->status = status;
}

void setTopo(CABECALHO *cabecalho, long long int topo)
{
    cabecalho->topo = topo;
}

void setProxByteOffset(CABECALHO *cabecalho, long long int proxByteOffset)
{
    cabecalho->proxByteOffset = proxByteOffset;
}

void setNroRegArq(CABECALHO *cabecalho, int nroRegArq)
{
    cabecalho->nroRegArq = nroRegArq;
}

void setNroRem(CABECALHO *cabecalho, int nroRem)
{
    cabecalho->nroRegRem = nroRem;
}

// Libera a memória do cabeçalho
void limpaCabecalho(CABECALHO *cabecalho)
{
    free(cabecalho);
}