#include "funcionalidades.h"
#include "definicoesTipos.h"
#include "1definicoesTiposArvoreB.h"
#include "funcoesAuxiliares.h"
#include "funcoes_fornecidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Insertion algorithm

/**
 * @brief Struct to help manipulate registers
 * @param key the key of the register
 * @param fileOffset the offset of the record in the main file (in bytes)
 * @param childRRN the RRN of the child node
 */
typedef struct REGISTRO_ARVORE_B {
    int key;
    int64_t fileOffset;
    int childRRN;
} REGISTRO_ARVORE_B;

/**
 * @brief Create and return an Register object
 */
REGISTRO_ARVORE_B* CreateRegister(int key, int64_t fileOffset, int childRRN) {
    REGISTRO_ARVORE_B* reg = (REGISTRO_ARVORE_B*) malloc(sizeof(REGISTRO_ARVORE_B));
    reg->key = key;
    reg->fileOffset = fileOffset;
    reg->childRRN = childRRN;

    return reg;
}

/**
 * @brief Get the Next Node object
 * 
 * @param meta 
 * @param node 
 * @param key 
 * @return BNode_t* 
 */
int GetNextNode(BTreeMetadata_t* meta, BNode_t* node, int key) {
    int i=0;
    while (key > node->keys[i] && i < node->indexedKeysCount) {
        i++;
    }

    return node->childrenRRNs[i];
}

/**
 * @brief Partitions a node to two.
 * 
 * @param meta The metadata of the tree
 * @param node The node that is full
 * @param newReg The register to be inserted in the node
 * @return The new node created
 */
BRegister_t* PartitionNode(BTreeMetadata_t* meta, BNode_t* node, BRegister_t* newReg) {
    // Criar vetores auxiliares
    int children[BTREE_ORDER+1];
    int64_t offsets[BTREE_ORDER];
    int keys[BTREE_ORDER];

    children[0] = node->childrenRRNs[0];
    for (int i=0; i<BTREE_ORDER-1; i++) {
        children[i+1] = node->childrenRRNs[i+1];
        offsets[i] = node->offsets[i];
        keys[i] = node->keys[i];
    }

    // Inserts newReg ordered at our auxiliary array
    int pos = 0;
    while (pos < (BTREE_ORDER-1) && newReg->key > keys[pos]) {
        pos++;
    }

    for (int i = BTREE_ORDER-1; i >= pos; i--) {
        children[i+1] = children[i];
        offsets[i] = offsets[i-1];
        keys[i] = keys[i-1];
    }
    children[pos+1] = newReg->childRRN;
    keys[pos] = newReg->key;
    offsets[pos] = newReg->fileOffset;

    free(newReg);

    // Copies the first in the old and erases removed registers from it
    node->childrenRRNs[BTREE_ORDER-1] = -1;
    for (int i=0; i<BTREE_ORDER/2; i++) {
        node->childrenRRNs[i+1] = children[i+1];
        node->offsets[i] = offsets[i];
        node->keys[i] = keys[i];
    }
    for (int i=BTREE_ORDER/2; i<BTREE_ORDER-1; i++) {
        node->childrenRRNs[i+1] = -1;
        node->offsets[i] = -1;
        node->keys[i] = -1;
    }
    node->indexedKeysCount = BTREE_ORDER/2;
    BinaryWriter_SeekAndWriteNode(node, meta);
    
    // Creates a new node and copies the last registers
    BNode_t* partitioned = BNode_CreateWithRRN(meta, node->isLeaf);

    partitioned->childrenRRNs[BTREE_ORDER/2] = children[BTREE_ORDER];
    for (int i = 0; i < BTREE_ORDER/2; i++) {
        partitioned->childrenRRNs[i] = children[i + BTREE_ORDER/2 + 1];
        partitioned->keys[i] = keys[i + BTREE_ORDER/2 + 1];
        partitioned->offsets[i] = offsets[i + BTREE_ORDER/2 + 1];
    }

    partitioned->indexedKeysCount = BTREE_ORDER/2;
    BinaryWriter_SeekAndWriteNode(partitioned, meta);

    // Creates the new register
    newReg = CreateRegister(keys[BTREE_ORDER/2], offsets[BTREE_ORDER/2], partitioned->rrn);
    
    BNode_Free(partitioned);
    return newReg;
}

/**
 * @brief Inserts a register into the given node. The node passed must have enough space for the new register.
 * 
 * @param node Node that will be updated
 * @param reg Register to be inserted; its memory will be freed
 */
BRegister_t* InsertRegisterInNode(BTreeMetadata_t* meta, BNode_t* node, BRegister_t* reg) {
    if (reg == NULL || node == NULL)   return NULL;

    // If there is no available space, partition
    if (node->indexedKeysCount >= BTREE_ORDER-1) {
        return PartitionNode(meta, node, reg);
    }   // Else, simple insertion is enough


    // Finds insertion index
    int pos = 0;
    while (pos < node->indexedKeysCount && reg->key > node->keys[pos]) pos++;

    // Shifts registers by 1
    for (int i=node->indexedKeysCount-1; i>=pos; i--) {
        node->childrenRRNs[i+2] = node->childrenRRNs[i+1];
        node->keys[i+1] = node->keys[i];
        node->offsets[i+1] = node->offsets[i];
    }
    node->childrenRRNs[pos+1] = node->childrenRRNs[pos];

    // Saves the content of the insertion
    node->keys[pos] = reg->key;
    node->offsets[pos] = reg->fileOffset;
    node->childrenRRNs[pos+1] = reg->childRRN;

    free(reg);
    
    node->indexedKeysCount++;
    return NULL;
}

/**
 * @brief Insert the register in the tree, takes care of partitions and promotions.
 * 
 * @param meta The metadata
 * @param node The current node of the insertion
 * @param newReg The new register to be included
 * @return In case of promotion, returns the promoted reg. Otherwise, returns NULL.
 */
BRegister_t* InsertNodeRecur(BTreeMetadata_t* meta, int nodeRRN, BRegister_t* newReg) {
    BNode_t* node = BinaryReader_BTreeNode(meta, nodeRRN);

    if (node == NULL) {
        fprintf(stderr, "Passed a NULL node to InsertNodeRecur. This should never happen. Returning...\n");
        return NULL;
    }

    BRegister_t* promoted = NULL;
    if (node->isLeaf == 1) { // Inserts in node
        promoted = InsertRegisterInNode(meta, node, newReg);
    }
    else { // Inserts in a child node
        promoted = InsertNodeRecur(meta, GetNextNode(meta, node, newReg->key), newReg);

        if (promoted != NULL) {
            promoted = InsertRegisterInNode(meta, node, promoted);
        }
    }

    BinaryWriter_SeekAndWriteNode(node, meta);
    BNode_Free(node);

    // If the node exists, tries to insert in it    
    return promoted;
}


void BTreeMetadata_Insert(BTreeMetadata_t* meta, int key, int64_t fileOffset) {
    BRegister_t* newReg = CreateRegister(key, fileOffset, -1);

    // The root is always defined in the metadata (if the tree is not empty)
    if (meta->header->rootRRN < 0) {
        // Create an root
        BNode_t* root = BNode_CreateWithRRN(meta, TRUE);
        meta->header->rootRRN = root->rrn;

        // Add a new register and updates the file
        InsertRegisterInNode(meta, root, newReg);
        BinaryWriter_SeekAndWriteNode(root, meta);

        BNode_Free(root);
    } else {
        // Inserts new register
        BRegister_t* promoted = InsertNodeRecur(meta, meta->header->rootRRN, newReg);

        // Checks if there is a promotion
        if (promoted != NULL) {
            // Create the new root
            BNode_t* newRoot = BNode_CreateWithRRN(meta, FALSE);
            newRoot->childrenRRNs[0] = meta->header->rootRRN;
            meta->header->rootRRN = newRoot->rrn;

            // Add new register and update the file
            InsertRegisterInNode(meta, newRoot, promoted);
            BinaryWriter_SeekAndWriteNode(newRoot, meta);

            BNode_Free(newRoot);
        }
    }
}


/**
 * @brief Helper function used during queries by key.
 * 
 * @param meta
 * @param nodeRRN 
 * @param key 
 * @return BNode_t* 
 */
BNode_t* GetNodeByKeyRecur(BTreeMetadata_t* meta, int nodeRRN, int key) {
    if (nodeRRN < 0) return NULL;

    BNode_t* current = BinaryReader_BTreeNode(meta, nodeRRN);

    if (current == NULL) return NULL;

    for (int i = 0; i < current->indexedKeysCount; i++) {
        if (current->keys[i] == key) return current;
        else if (current->keys[i] > key){
            int filhoRRN = current->childrenRRNs[i];
            BNode_Free(current);
            return GetNodeByKeyRecur(meta, filhoRRN, key);
        } 
    }

    int filhoRRN = current->childrenRRNs[current->indexedKeysCount];
    BNode_Free(current);
    return GetNodeByKeyRecur(meta, filhoRRN, key);
}

BNode_t* BTreeMetadata_GetNodeByKey(BTreeMetadata_t* meta, int key) {
    return GetNodeByKeyRecur(meta, meta->header->rootRRN, key);
}