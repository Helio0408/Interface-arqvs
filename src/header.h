#ifndef HEADER_H
#define HEADER_H

#include "def.h"
#include <stdio.h>
#include <stdlib.h>

/* tamanho dos headers em bytes */
#define HEADER_SIZE 25
#define HEADER_SIZE_IND 1

/* cria uma estrutura para o arquivo */
HEADER* header_create();

/* libera a memória utilizada pelo header */
void header_free(HEADER *h);

/* escreve o header em um arquivo já aberto */
void header_write(FILE *file, HEADER *h);

/* lê o header de um arquivo já aberto */
HEADER* header_read(FILE *file);


/* ##### FUNÇÕES PARA INDEX ##### 
 * tem as mesmas funções das 
 * versões anteriores*/

HEADER_IND* headerInd_create();

void headerInd_free(HEADER_IND *h);

void headerInd_write(FILE *file, HEADER_IND *h);

HEADER_IND* headerInd_read(FILE *file);

#endif
