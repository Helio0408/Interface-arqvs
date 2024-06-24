#ifndef DEF_H
#define DEF_H

#include <stdint.h>

/* As definições conforme as predefinições desejadas */

typedef struct{
	char status;
	int64_t topo;
	int64_t proxByteOffset;
	int32_t nroRegArq;
	int32_t nroRegRem;
} HEADER;

typedef struct{
	char removido;
	int32_t tamanhoRegistro;
	int64_t Prox;
	int32_t id;
	int32_t idade;
	uint32_t tamNomeJog;
	char *nomeJogador;
	uint32_t tamNacionalidade;
	char *nacionalidade;
	uint32_t tamNomeClube;
	char *nomeClube;
} REGISTER;

typedef struct{
	char status;
} HEADER_IND;

typedef struct{
	int32_t id;
	int64_t ByteOffset;
} REGISTER_IND;

#endif
