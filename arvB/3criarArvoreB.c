#include "funcionalidades.h"
#include "definicoesTipos.h"
#include "1definicoesTiposArvoreB.h"
#include "funcoesAuxiliares.h"
#include "funcoes_fornecidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

ARVORE_B* BTreeMetadata_Create(const char* bTreeIndexFileName, const char* indexOpenType, const char* registersFileName, const char* registerOpenType) {
    ARVORE_B* arvoreB = calloc(1, sizeof(ARVORE_B));

    // Initializes the registers File
    if (registersFileName != NULL && registerOpenType != NULL) {
        arvoreB->registersFile = fopen(registersFileName, registerOpenType);
        
        if (arvoreB->registersFile != NULL) {
            char status;
        
            fseek(arvoreB->registersFile, 0, SEEK_SET);
            fread(&status, sizeof(char), 1, arvoreB->registersFile);

            // Validates status
            if (status == '0') {
                fclose(arvoreB->registersFile);
                arvoreB->registersFile = NULL;
            } else {
                status = '0';
                fseek(arvoreB->registersFile, 0, SEEK_SET);
                fwrite(&status, sizeof(char), 1, arvoreB->registersFile);
            }
        }
    }

    // Initializes the index File
    if (bTreeIndexFileName != NULL && indexOpenType != NULL) {
        arvoreB->bTreeIndexFile = fopen(bTreeIndexFileName, indexOpenType);
        
        // Validates file size
        if (arvoreB->bTreeIndexFile != NULL) {
            fseek(arvoreB->bTreeIndexFile, 0, SEEK_SET);
            
            char status;
            fread(&status, sizeof(char), 1, arvoreB->bTreeIndexFile);

            // Validates status
            if (status == '0') {
                fclose(arvoreB->bTreeIndexFile);
                arvoreB->bTreeIndexFile = NULL;
                arvoreB->header = NULL;
            } else {
                status = '0';
                fseek(arvoreB->bTreeIndexFile, 0, SEEK_SET);
                fwrite(&status, sizeof(char), 1, arvoreB->bTreeIndexFile);
                arvoreB->header = BinaryReader_BTreeHeader(arvoreB->bTreeIndexFile);
            }
        }
    }
    
    return arvoreB;
}

void BTreeMetadata_Free(ARVORE_B* arvoreB) {
    if (arvoreB == NULL)   return;

    if (arvoreB->registersFile != NULL) {
        char status = '1';
        fseek(arvoreB->registersFile, 0, SEEK_SET);
        fwrite(&status, sizeof(char), 1, arvoreB->registersFile);
        fclose(arvoreB->registersFile);
    }

    if (arvoreB->bTreeIndexFile != NULL) {
        arvoreB->header->status = '1';
        BinaryWriter_BTreeHeader(arvoreB);
        fclose(arvoreB->bTreeIndexFile);
    }

    free(arvoreB->header);
    free(arvoreB);
}