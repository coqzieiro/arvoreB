#ifndef _DATA_MODEL_H_
    #define _DATA_MODEL_H_
    #define ORDEM_DA_ARVORE 4 // Número máximo de filhos

    #include <stdint.h>
    #include <stdio.h>

    typedef struct CABECALHO_ARVORE_B{
        char status;
        int noRaiz;
        int proxRRN;
        int nroChaves;
        char lixo[47];
    } CABECALHO_ARVORE_B;
    
    typedef struct ARVORE_B {
        FILE* bTreeIndexFile; // File with the nodes from the B-Tree.
        FILE* registersFile; // File with the registers from the B-Tree.
        CABECALHO_ARVORE_B* header;
    } ARVORE_B;

    typedef struct NO_ARVORE_B {
        char isLeaf;
        int rrn;
        int indexedKeysCount;

        int64_t offsets[ORDEM_DA_ARVORE-1];
        int keys[ORDEM_DA_ARVORE-1];
        int childrenRRNs[ORDEM_DA_ARVORE];
    } NO_ARVORE_B;



#endif
