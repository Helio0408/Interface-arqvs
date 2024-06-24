#ifndef BINARY_H
#define BINARY_H

#include "register.h"

/* Cria um arquivo binario "saida" com as informações do
 * aquivo csv "entrada" */
void bin_CreateTable(char *entrada, char *saida);

/* Imprime todos os dados do arquivo binarario "entrada" */
void bin_SelectFrom(char *entrada);

/* Imprime todos os correspondentes das buscas realizadas 
 * no arquivo binario "entrada" */
void bin_SelectFrom_Where(char *entrada, int num_buscas);

/* Cria um aquivo indice binario "saida" que indexa as 
 * informações do arquivo de dados "entrada" */
void bin_CreateIndex(char *entrada, char *saida);

/* Função para deletar valores na tabela */
void bin_Delete(char *entrada, char *indice, int num_deletes);

/* Função para inserir valores na tabela */
void bin_InsertInto(char *entrada, char *indice, int num_inserts);

/* Função de debug para imprimir a lista dos registros logicamente
 * removidos */
void bin_debugprintdeletedlist(char *entrada);

#endif
