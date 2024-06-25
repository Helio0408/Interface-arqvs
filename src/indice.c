#include "indice.h"

#include "header.h"
#include "register.h"

INDICE *indice_load(const char *filename){
	INDICE *indice;
	FILE *file;

	file = fopen(filename, "rb");

	if(file == NULL) return NULL;

	indice = (INDICE *) malloc(sizeof(INDICE));

	indice->header = headerInd_read(file);
	indice->num_reg = 0;

	/* retorna nulo caso o arquivo esteja inválido */
	if(indice->header->status != '1'){
		headerInd_free(indice->header);
		free(indice);
		fclose(file);
		return NULL;
	}

	/* Calcula a quantidade de registros do arquivo */
	fseek(file, 0, SEEK_END);
	indice->num_reg = (ftell(file) - 1) / sizeof(REGISTER_IND);

	/* Cria uma array para armazenar ponteiros para os registros de índice */
	indice->regs = (REGISTER_IND **) malloc(sizeof(REGISTER_IND *) * indice->num_reg);

	fseek(file, HEADER_SIZE_IND, SEEK_SET);

	/* lê os índices */
	for(int i = 0; i < indice->num_reg; i++)
		indice->regs[i] = registerInd_readFromBIN(file);

	fclose(file);
	
	return indice;
}

void indice_free(INDICE *indice){
	/* libera memória utilizada por cada índice */
	for(int i = 0; i < indice->num_reg; i++)
		registerInd_free(indice->regs[i]);

	/* libera a memória para o ponteiro da array */
	free(indice->regs);
	
	/* libera a memória do header */
	headerInd_free(indice->header);

	/* libera a memória do ponteiro */
	free(indice);
}
