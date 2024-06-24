#ifndef INDICE_H
#define INDICE_H

#include "def.h"

typedef struct{
	HEADER_IND *header;
	REGISTER_IND **regs;
	int num_reg;
} INDICE;

INDICE *indice_load(const char *filename);

void indice_free(INDICE *indice);

#endif
