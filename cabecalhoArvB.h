#ifndef CABECALHOARVOREB_H
    #define CABECALHOARVOREB_H
    #include "definicoesTipos.h"

    typedef struct _cabecalhoArvoreB CABECALHO_ARVORE_B;

    CABECALHO_ARVORE_B *criarCabecalhoArvoreBVazio();
    char getStatusCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);
    int getNoRaizCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);
    int getProxRRNCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);
    int getNroChavesCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);
    int setStatusCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, char status);
    int setNoRaizCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int noRaiz);
    int setProxRRNCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int proxRRN);
    int setNroChavesCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho, int nroChaves);
    CABECALHO_ARVORE_B *lerCabecalhoArvoreB(FILE *file);
    int escreverCabecalhoArvoreB(FILE *file, CABECALHO_ARVORE_B *cabecalho);
    int apagarCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);
    int imprimirCabecalhoArvoreB(CABECALHO_ARVORE_B *cabecalho);
    
#endif