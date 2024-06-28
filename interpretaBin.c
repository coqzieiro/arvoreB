#include "funcoesAuxiliares.h"

// Função que lê os valores do cabeçalho do arquivo binário e salva na struct cabeçalho
void lerCabecalhoFromBin(FILE *file, CABECALHO *cabecalho){
    char status;
    // lê o status do cabeçalho do arquivo e salva na variável status
    fread(&status, sizeof(char), 1, file);
    setStatus(cabecalho, status);

    // lê o topo do cabeçalho do arquivo e salva na variável topo
    long long int topo;
    fread(&topo, sizeof(long long int), 1, file);
    setTopo(cabecalho, topo);

    // lê o próximo byte offset do cabeçalho do arquivo e salva na variável proxyByteOffset
    long long int proxByteOffset;
    fread(&proxByteOffset, sizeof(long long int), 1, file);
    setProxByteOffset(cabecalho, proxByteOffset);

    // lê o número de registros não removidos do cabeçalho do arquivo e salva na variável nroRegArq
    int nroRegArq;
    fread(&nroRegArq, sizeof(int), 1, file);
    setNroRegArq(cabecalho, nroRegArq);

    // lê o número de registros removidos do cabeçalho do arquivo e salva na variável nroRem
    int nroRem;
    fread(&nroRem, sizeof(int), 1, file);
    setNroRem(cabecalho, nroRem);
}

// Função que pega o cabeçalho do arquivo binário e salva em uma struct do tipo CABECALHO
CABECALHO *getCabecalhoFromBin(FILE *file)
{
    // cria um cabeçalho e chama a função lerCabecalhoFromBin para atribuir os valores a ele
    CABECALHO *cabecalho = criarCabecalho();
    fseek(file, 0, SEEK_SET);
    lerCabecalhoFromBin(file, cabecalho);

    return cabecalho;
}