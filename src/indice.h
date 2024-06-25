#ifndef INDICE_H
#define INDICE_H

#include "def.h"

typedef struct{
	HEADER_IND *header;
	REGISTER_IND **regs;
	int num_reg;
} INDICE;

/* Carrega o índice em memória primária */
INDICE *indice_load(const char *filename);

/* Libera a memória do carregamento do índice */
void indice_free(INDICE *indice);

#endif
