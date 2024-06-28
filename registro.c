#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesAuxiliares.h"

// Função que imprime os registros de acordo com os parâmetros de busca
void imprimirRegistros(DADOS **registros)
{
    if(registros == NULL || registros[0] == NULL)
    {
        printf("Registro inexistente.\n");
        printf("\n");
        return;
    }
    for (int i = 0; registros[i] != NULL; i++) // Percorre o vetor de registros até encontrar um registro nulo
    {
        char *nomeClube = get_nomeClube(registros[i]);
        char *nacionalidade = get_nacionalidade(registros[i]);
        char *nomeJogador = get_nomeJogador(registros[i]);

        printf("Nome do Jogador: ");
        if (strcmp(nomeJogador, "SEM DADO") == 0) // se o nome do jogador for "SEM DADO", imprime "SEM DADO"
        {
            printf("SEM DADO\n");
        }
        else // se não, imprime cada caractere do nome do jogador
        {
            for (int j = 0; j < get_tamNomeJogador(registros[i]); j++)
            {
                printf("%c", nomeJogador[j]);
            }
            printf("\n");
        }

        printf("Nacionalidade do Jogador: ");
        if (strcmp(nacionalidade, "SEM DADO") == 0) // se a nacionalidade do jogador for "SEM DADO", imprime "SEM DADO"
        {
            printf("SEM DADO\n");
        }
        else // se não, imprime cada caractere da nacionalidade
        {
            for (int j = 0; j < get_tamNacionalidade(registros[i]); j++)
            {
                printf("%c", nacionalidade[j]);
            }
            printf("\n");
        }

        printf("Clube do Jogador: ");
        if(strcmp(nomeClube, "SEM DADO") == 0) // se o nome do clube do jogador for "SEM DADO", imprime "SEM DADO"
        {
            printf("SEM DADO\n");
        }
        else // se não, imprime cada caractere do nome do clube
        {
            for(int j = 0; j<get_tamNomeClube(registros[i]); j++)
            {
                printf("%c", nomeClube[j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

// Função que retorna a intersecao de dois vetores de registros, verificando se cada registro do primeiro vetor está no segundo vetor 
DADOS **intersecaoDoisRegistros(DADOS **registro1, DADOS **registro2)
{
    DADOS **intersecao = (DADOS **)malloc(0);
    int quantidade = 0;

    // para cada registro do primeiro vetor, verifica se existe um registro do segundo vetor que é igual a ele (possui o mesmo id)
    for (int i = 0; registro1[i] != NULL; i++)
    {
        for (int j = 0; registro2[j] != NULL; j++)
        {
            if (get_id(registro1[i]) == get_id(registro2[j])) // se forem iguais, adiciona o registro no vetor da intersecao
            {
                intersecao = (DADOS **)realloc(intersecao, sizeof(DADOS *) * (quantidade + 1));
                intersecao[quantidade] = registro1[i];
                quantidade++;
            }
        }
    }

    // adiciona mais um registro com o valor nulo no vetor intersecao para indicar o fim do vetor
    intersecao = (DADOS **)realloc(intersecao, sizeof(DADOS *) * (quantidade + 1));
    intersecao[quantidade] = NULL;
    return intersecao;
}

// Função que cria um registro com os valores iniciais
DADOS *criarRegistroNulo()
{
    DADOS *registro = (DADOS *)malloc(sizeof(DADOS));
    registro->removido = '1';
    registro->tamanhoRegistro = 0;
    registro->prox = -1;
    registro->id = -1;
    registro->idade = -1;
    registro->tamNomeJog = -1;
    registro->nomeJogador = NULL;
    registro->tamNacionalidade = -1;
    registro->nacionalidade = NULL;
    registro->tamNomeClube = -1;
    registro->nomeClube = NULL;
    return registro;
}

// Função que cria um registro com os valores passados por parâmetro na função
DADOS *criarRegistro(char removido, int tamanhoRegistro, long prox, int id, int idade, int tamNomeJogador, char *nomeJogador, int tamNacionalidade, char *nacionalidade, int tamNomeClube, char *nomeClube) {
    DADOS *registro = (DADOS *)malloc(sizeof(DADOS));
    registro->removido = removido;
    registro->tamanhoRegistro = tamanhoRegistro;
    registro->prox = prox;
    registro->id = id;
    registro->idade = idade;
    registro->tamNomeJog = tamNomeJogador;
    registro->nomeJogador = (nomeJogador != NULL) ? strdup(nomeJogador) : NULL;
    registro->tamNacionalidade = tamNacionalidade;
    registro->nacionalidade = (nacionalidade != NULL) ? strdup(nacionalidade) : NULL;
    registro->tamNomeClube = tamNomeClube;
    registro->nomeClube = (nomeClube != NULL) ? strdup(nomeClube) : NULL;
    return registro;
}


// Funções get para que seja possível acessar os atributos dos registros em outras partes do código

char get_removido(DADOS *registro)
{
    return registro->removido;
}

DADOS *buscarRegistroOffset(long long offset, FILE *file) {
    DADOS *registro = lerRegistroFromBin(offset, file); // lê o registro do arquivo binário
    return registro;
}

int get_tamanhoRegistro(DADOS *registro)
{
    return registro->tamanhoRegistro;
}

long long int get_prox(DADOS *registro)
{
    return registro->prox;
}

int get_id(DADOS *registro)
{
    return registro->id;
}

int get_idade(DADOS *registro)
{
    return registro->idade;
}

int get_tamNomeJogador(DADOS *registro)
{
    return registro->tamNomeJog;
}

char *get_nomeJogador(DADOS *registro) {
    if (registro->nomeJogador == NULL || strcmp(registro->nomeJogador, "") == 0) // Check for NULL and empty string
    {
        return "SEM DADO";
    }
    return registro->nomeJogador;
}

int get_tamNacionalidade(DADOS *registro)
{
    return registro->tamNacionalidade;
}

char *get_nacionalidade(DADOS *registro)
{
    if (registro->nacionalidade == NULL || strcmp(registro->nacionalidade, "") == 0) // Check for NULL and empty string
    {
        return "SEM DADO";
    }
    return registro->nacionalidade;
}

int get_tamNomeClube(DADOS *registro)
{
    return registro->tamNomeClube;
}

char *get_nomeClube(DADOS *registro) {
    if (registro->nomeClube == NULL || strcmp(registro->nomeClube, "") == 0) // Check for NULL and empty string
    {
        return "SEM DADO";
    }
    return registro->nomeClube;
}


// Funções set para que seja possível alterar o valor de um atributo de um registro em outras partes do código

void set_removido(DADOS *registro, int removido)
{
    registro->removido = removido;
}

void set_tamanhoRegistro(DADOS *registro, int tamanhoRegistro)
{
    registro->tamanhoRegistro = tamanhoRegistro;
}

void set_prox(DADOS *registro, long long int prox)
{
    registro->prox = prox;
}

void set_id(DADOS *registro, int id)
{
    registro->id = id;
}

void set_idade(DADOS *registro, int idade)
{
    if(idade == 0) // se idade for igual a 0, muda o valor do atributo idade do registro para -1 para indicar que é uma idade inválida
    {
        registro->idade = -1;
        return;
    }
    registro->idade = idade;
}

void set_tamNomeJogador(DADOS *registro, int tamNomeJogador)
{
    registro->tamNomeJog = tamNomeJogador;
}

void set_nomeJogador(DADOS *registro, char *nomeJogador)
{
    registro->nomeJogador = nomeJogador;
}

void set_tamNacionalidade(DADOS *registro, int tamNacionalidade)
{
    registro->tamNacionalidade = tamNacionalidade;
}

void set_nacionalidade(DADOS *registro, char *nacionalidade)
{
    registro->nacionalidade = nacionalidade;
}

void set_tamNomeClube(DADOS *registro, int tamNomeClube)
{
    registro->tamNomeClube = tamNomeClube;
}

void set_nomeClube(DADOS *registro, char *nomeClube)
{
    registro->nomeClube = nomeClube;
}

// Função que libera o espaço de memória do registro e de seus atributos
void liberarRegistro(DADOS *registro) {
    if (registro->nomeJogador)
        free(registro->nomeJogador);
    if (registro->nacionalidade)
        free(registro->nacionalidade);
    if (registro->nomeClube)
        free(registro->nomeClube);
    free(registro);
}

DADOS *lerRegistroFromBin(int posicao, FILE *arquivoBin) {
    fseek(arquivoBin, posicao, SEEK_SET);

    DADOS *registro = criarRegistroNulo();

    char removido;
    fread(&removido, sizeof(char), 1, arquivoBin); // lê o caractere "removido" de um registro do arquivo e salva na variável removido
    set_removido(registro, removido);

    int tamanhoRegistro;
    fread(&tamanhoRegistro, sizeof(int), 1, arquivoBin); // lê o tamanho de um registro do arquivo e salva na variável tamanhoRegistro
    set_tamanhoRegistro(registro, tamanhoRegistro);

    long long int prox;
    fread(&prox, sizeof(long long int), 1, arquivoBin); // lê a posição do próximo registro removido do arquivo e salva na variável prox
    set_prox(registro, prox);

    int id;
    fread(&id, sizeof(int), 1, arquivoBin); // lê o id de um registro do arquivo e salva na variável id
    set_id(registro, id);

    int idade;
    fread(&idade, sizeof(int), 1, arquivoBin); // lê a idade do jogador de um registro do arquivo e salva na variável idade
    set_idade(registro, idade);

    int tamNomeJogador;
    fread(&tamNomeJogador, sizeof(int), 1, arquivoBin); // lê o tamanho do nome do jogador de um registro do arquivo e salva na variável tamNomeJogador
    set_tamNomeJogador(registro, tamNomeJogador);

    if (tamNomeJogador > 0)
    {
        char *nomeJogador = (char *)malloc(tamNomeJogador + 1);
        fread(nomeJogador, sizeof(char), tamNomeJogador, arquivoBin);
        nomeJogador[tamNomeJogador] = '\0'; // Ensure null-termination
        set_nomeJogador(registro, nomeJogador);
    }
    else
    {
        set_nomeJogador(registro, NULL);
    }

    int tamNacionalidade;
    fread(&tamNacionalidade, sizeof(int), 1, arquivoBin); // lê o tamanho da string nacionalidade de um registro do arquivo e salva na variável tamNacionalidade
    set_tamNacionalidade(registro, tamNacionalidade);

    if (tamNacionalidade > 0)
    {
        char *nacionalidade = (char *)malloc(tamNacionalidade + 1);
        fread(nacionalidade, sizeof(char), tamNacionalidade, arquivoBin);
        nacionalidade[tamNacionalidade] = '\0'; // Ensure null-termination
        set_nacionalidade(registro, nacionalidade);
    }
    else
    {
        set_nacionalidade(registro, NULL);
    }

    int tamNomeClube;
    fread(&tamNomeClube, sizeof(int), 1, arquivoBin); // lê o tamanho do nome do clube de um registro do arquivo e salva na variável tamNomeClube
    set_tamNomeClube(registro, tamNomeClube);

    if (tamNomeClube > 0)
    {
        char *nomeClube = (char *)malloc(tamNomeClube + 1);
        fread(nomeClube, sizeof(char), tamNomeClube, arquivoBin);
        nomeClube[tamNomeClube] = '\0'; // Ensure null-termination
        set_nomeClube(registro, nomeClube);
    }
    else
    {
        set_nomeClube(registro, NULL);
    }

    return registro;
}