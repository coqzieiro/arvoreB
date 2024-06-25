#include "bTreeDataModel.h"
#include "1definicoesTiposArvoreB.h"
#include "bTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

CABECALHO_ARVORE_B* BHeader_Create(char status, int rootNode, int rrnNextNode) {
    // Creates a new header and returns it
    CABECALHO_ARVORE_B* header = (CABECALHO_ARVORE_B*) malloc(sizeof(CABECALHO_ARVORE_B));

    header->status = status;
    header->noRaiz = rootNode;
    header->proxRRN = rrnNextNode;
    
    memset(header->lixo, '$', sizeof(header->lixo));
    
    return header;
}

void BHeader_Free(BHeader_t* header) {
    free(header);
}

BNode_t* BNode_Create(char isLeaf, int32_t indexedKeysCount, RRN_t rrn, ByteOffset_t* regOffsets, RegKey_t* regKeys, RRN_t* childrenRRNs) {
    // Creates a new node and returns it
    BNode_t* node = (BNode_t*) calloc(1, sizeof(BNode_t));
    node->isLeaf = isLeaf;
    node->indexedKeysCount = indexedKeysCount;
    node->rrn = rrn;

    for (int i = 0; i < BTREE_ORDER-1; i++) {
        node->childrenRRNs[i] = childrenRRNs[i];
        node->keys[i] = regKeys[i];
        node->offsets[i] = regOffsets[i];
    }
    node->childrenRRNs[BTREE_ORDER-1] = childrenRRNs[BTREE_ORDER-1];

    return node;
}

BNode_t* BNode_CreateWithRRN(BTreeMetadata_t* meta, char isLeaf) {
    BNode_t* node = BNode_CreateNull();
    node->isLeaf = isLeaf;
    node->rrn = meta->header->rrnNextNode;
    meta->header->rrnNextNode += 1;
    return node;
}

BNode_t* BNode_CreateNull() {
    ByteOffset_t regOffsets[BTREE_ORDER-1] = { -1, -1, -1, -1 };
    RegKey_t regKeys[BTREE_ORDER-1] = { -1, -1, -1, -1 };
    RRN_t childrenRRNs[BTREE_ORDER] = { -1, -1, -1, -1, -1 };
    return BNode_Create(-1, 0, -1, &regOffsets[0], &regKeys[0], &childrenRRNs[0]);
}

int BNode_GetKeyIndex(BNode_t* node, RegKey_t key) {
    if (node == NULL) return -1;

    for (int i = 0; i < node->indexedKeysCount; ++i) {
        if (node->keys[i] == key) {
            return i;
        }
    }
    return -1;
}

void BNode_Free(BNode_t* node) {
    if (node != NULL)   free(node);
}

ByteOffset_t RRNToOffset(RRN_t rrn) {
    return (1 + (ByteOffset_t) rrn) * ((ByteOffset_t) BTREE_RECORD_SIZE);
}

void BinaryWriter_BTreeHeader(ARVORE_B* arvoreB) {    
    fseek(arvoreB->bTreeIndexFile, 0, SEEK_SET);

    fwrite(&(arvoreB->header->status), sizeof(char), 1, arvoreB->bTreeIndexFile);

    fwrite(&(arvoreB->header->noRaiz), sizeof(int), 1, arvoreB->bTreeIndexFile);
    fwrite(&(arvoreB->header->proxRRN), sizeof(int), 1, arvoreB->bTreeIndexFile);
    fwrite(&(arvoreB->header->nroChaves), sizeof(int), 1, arvoreB->bTreeIndexFile);
    
    fwrite(&(arvoreB->header->lixo[0]), sizeof(char), 47, arvoreB->bTreeIndexFile);
}

CABECALHO_ARVORE_B* BinaryReader_BTreeHeader(FILE* pt) {
    fseek(pt, 0, SEEK_SET);

    CABECALHO_ARVORE_B* header = BHeader_Create(0, -1, -1);

    fread(&(header->status), sizeof(char), 1, pt);
    fread(&(header->noRaiz), sizeof(int), 1, pt);
    fread(&(header->proxRRN), sizeof(int), 1, pt);
    fread(&(header->nroChaves), sizeof(int), 1, pt);
    fread(&(header->lixo[0]), sizeof(char), 47, pt);

    return header;
}