#include "cabecalho.h"
#include "definicoesTipos.h"

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
void apagarCabecalho(CABECALHO *cabecalho)
{
    free(cabecalho);
}

// Função que escreve o status do cabeçalho no arquivo binário
void writeStatusCabecalho(CABECALHO *cabecalho, FILE *arquivoBin)
{
    const int statusByte = 0;
    fseek(arquivoBin, statusByte, SEEK_SET);
    char status = getStatus(cabecalho);
    fwrite(&status, sizeof(char), 1, arquivoBin);
}

// Função que escreve o topo do cabeçalho no arquivo binário
void writeTopoCabecalho(CABECALHO *cabecalho, FILE *arquivoBin)
{
    const int topoByte = 1;
    fseek(arquivoBin, topoByte, SEEK_SET);
    long long int topo = getTopo(cabecalho);
    fwrite(&topo, sizeof(long long int), 1, arquivoBin);
}

// Função que escreve o próximo byte offset do cabeçalho no arquivo binário
void writeProxByteOffsetCabecalho(CABECALHO *cabecalho, FILE *arquivoBin)
{
    const int proxByteOffsetByte = 9;
    fseek(arquivoBin, proxByteOffsetByte, SEEK_SET);
    long long int proxByteOffset = getProxByteOffset(cabecalho);
    fwrite(&proxByteOffset, sizeof(long long int), 1, arquivoBin);
}

void writeNroRegArqCabecalho(CABECALHO *cabecalho, FILE *arquivoBin)
{
    const int nroRegArqByte = 17;
    fseek(arquivoBin, nroRegArqByte, SEEK_SET);
    int nroRegArq = getNroRegArq(cabecalho);
    fwrite(&nroRegArq, sizeof(int), 1, arquivoBin);
}

// Função que escreve o número de registros removidos no cabeçalho do arquivo binário
void writeNroRegRemCabecalho(CABECALHO *cabecalho, FILE *arquivoBin)
{
    const int nroRegRemByte = 21;
    fseek(arquivoBin, nroRegRemByte, SEEK_SET);
    int nroRem = getNroRem(cabecalho);
    fwrite(&nroRem, sizeof(int), 1, arquivoBin);
}

void imprimirCabecalho(CABECALHO *cabecalho)
{
    printf("Status: %c\n", getStatus(cabecalho));
    printf("Topo: %lld\n", getTopo(cabecalho));
    printf("ProxByteOffset: %lld\n", getProxByteOffset(cabecalho));
    printf("NroRegArq: %d\n", getNroRegArq(cabecalho));
    printf("NroRegRem: %d\n\n", getNroRem(cabecalho));
}