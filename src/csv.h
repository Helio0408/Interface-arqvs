#ifndef CSV_H
#define CSV_H

#define LINE_SIZE 200

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct{
	FILE *file;
	char line[LINE_SIZE];
	int current_char;
} CSV;

/* Lê um arquivo e retorna NULL caso ele não exista */
CSV * csv_load(const char *filename);

/* Lê a próxima linha.
 * Retorna 1 caso seja possível ler a próxima linha;
 * Retorna 0 caso contrário. */
int csv_nextLine(CSV *csv);

/* Salva o token procurado em *token e salva o tamanho utilizado no
 * argumento passado. É responsabilidade do usuário dar free
 * no token depois.
 * Retorna 1 caso haja o próximo token. Retorna 0 caso contrário. */
int csv_nextToken(CSV *csv, char **token, uint32_t *tamanho);

/* Converte o próximo token em inteiro e salva em token.
 * Retorna 0 caso a conversão tenha dado errado,
 * retorna 1 caso contrário. */
int csv_nextTokenInt(CSV *csv, int *token);

/* Libera a memória utilizada pelo csv */
void csv_free(CSV *csv);

#endif
