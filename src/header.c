#include "header.h"

HEADER* header_create(){
	HEADER *h;

	h = (HEADER*) malloc(sizeof(HEADER));

	h->status = '0';
	h->topo = -1;
	h->proxByteOffset = 0;
	h->nroRegArq = 0;
	h->nroRegRem = 0;

	return h;
}

void header_free(HEADER *h){
	if(h == NULL)
		return;
	
	free(h);
}

void header_write(FILE *file, HEADER *h){
	fwrite(&(h->status), sizeof(h->status), 1, file);
	fwrite(&(h->topo), sizeof(h->topo), 1, file);
	fwrite(&(h->proxByteOffset), sizeof(h->proxByteOffset), 1, file);
	fwrite(&(h->nroRegArq), sizeof(h->nroRegArq), 1, file);
	fwrite(&(h->nroRegRem), sizeof(h->nroRegRem), 1, file);
}

HEADER* header_read(FILE *file){
	HEADER* h;
	long int pos;
	int flag = 0;

	h = header_create();

	pos = ftell(file);

	/* volta para a posição 0 do arquivo */
	if(pos != 0){
		rewind(file);
		flag = 1;
	}

	fread(&(h->status), sizeof(h->status), 1, file);
	fread(&(h->topo), sizeof(h->topo), 1, file);
	fread(&(h->proxByteOffset), sizeof(h->proxByteOffset), 1, file);
	fread(&(h->nroRegArq), sizeof(h->nroRegArq), 1, file);
	fread(&(h->nroRegRem), sizeof(h->nroRegRem), 1, file);

	/* volta para a posição anterior do arquivo */
	if(flag == 1)
		fseek(file, pos, SEEK_SET);

	return h;
}

HEADER_IND* headerInd_create(){
	HEADER_IND *h;

	h = (HEADER_IND*) malloc(sizeof(HEADER_IND));

	h->status = '0';

	return h;
}

void headerInd_free(HEADER_IND *h){
	if(h == NULL)
		return;

	free(h);
}

void headerInd_write(FILE *file, HEADER_IND *h){
	fwrite(&(h->status), sizeof(h->status), 1, file);
}

HEADER_IND* headerInd_read(FILE *file){
	HEADER_IND* h;
	long int pos;
	int flag = 0;

	h = headerInd_create();

	pos = ftell(file);

	/* volta para a posição 0 do arquivo */
	if(pos != 0){
		rewind(file);
		flag = 1;
	}

	fread(&(h->status), sizeof(h->status), 1, file);

	/* volta para a posição anterior do arquivo */
	if(flag == 1)
		fseek(file, pos, SEEK_SET);

	return h;	
}
