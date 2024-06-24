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

	if(indice->header->status != '1'){
		headerInd_free(indice->header);
		free(indice);
		fclose(file);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	indice->num_reg = (ftell(file) - 1) / sizeof(REGISTER_IND);

	indice->regs = (REGISTER_IND **) malloc(sizeof(REGISTER_IND *) * indice->num_reg);

	fseek(file, HEADER_SIZE_IND, SEEK_SET);

	for(int i = 0; i < indice->num_reg; i++)
		indice->regs[i] = registerInd_readFromBIN(file);

	fclose(file);
	
	return indice;
}

void indice_free(INDICE *indice){
	for(int i = 0; i < indice->num_reg; i++)
		registerInd_free(indice->regs[i]);

	free(indice->regs);
	
	headerInd_free(indice->header);

	free(indice);
}
