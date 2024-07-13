/*
INTEGRANTES DO GRUPO:
                    Felipe da Costa Coqueiro,   NºUSP: 11781361
                    Fernando Alee Suaiden,      NºUSP: 12680836
*/

#include "../bibliotecas/definicoesTipos.h"
#include "../bibliotecas/funcoesAuxiliaresB.h"

// Funções de set para atribuir um valor aos campos do cabeçalho de acordo com os valores passados como parâmetro nas funções

void setStatus(CABECALHO *cabecalho, char status)
{
    cabecalho->status = status;
}

void setTopo(CABECALHO *cabecalho, int64_t topo)
{
    cabecalho->topo = topo;
}

void setProxByteOffset(CABECALHO *cabecalho, int64_t proxByteOffset)
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