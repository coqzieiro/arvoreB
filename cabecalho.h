#ifndef CABECALHO_H
    #define CABECALHO_H

    #include <stdlib.h>
    #include <stdio.h>
    #include "registro.h"

    CABECALHO *criarCabecalho(void);

    char getStatus(CABECALHO *cabecalho);

    long long int getTopo(CABECALHO *cabecalho);

    long long int getProxByteOffset(CABECALHO *cabecalho);

    int getNroRegArq(CABECALHO *cabecalho);

    int getNroRem(CABECALHO *cabecalho);

    void setStatus(CABECALHO *cabecalho, char status);

    void setTopo(CABECALHO *cabecalho, long long int topo);

    void setProxByteOffset(CABECALHO *cabecalho, long long int proxByteOffset);

    void setNroRegArq(CABECALHO *cabecalho, int nroRegArq);

    void setNroRem(CABECALHO *cabecalho, int nroRem);

    void apagarCabecalho(CABECALHO *cabecalho);

    void writeStatusCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);

    void writeTopoCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);

    void writeProxByteOffsetCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);

    void writeNroRegArqCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);

    void writeNroRegRemCabecalho(CABECALHO *cabecalho, FILE *arquivoBin);

    void imprimirCabecalho(CABECALHO *cabecalho);

#endif