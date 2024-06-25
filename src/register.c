#include "register.h"

#define min(x, y) (x < y ? x : y)

/* o tamanho base do registro sem contar com os campos de
 * tamanho variável é 33 bytes */
#define TAMANHO_BASE 33
#define TAMANHO_BASE_IND 12

/* essa função imprime uma string de tamanho definido
 * é usada para quando uma string não termina com '\0' */
static void register_printStr(const char *str, int size);

static void register_printStr(const char *str, int size){
	if(size == 0){
		printf("SEM DADO\n");
	}
	else{
		fwrite(str, size, 1, stdout);
		printf("\n");
	}
}

REGISTER * register_create(void){
	REGISTER * reg;
	reg = (REGISTER *) malloc(sizeof(REGISTER));

	reg->id = -1;
	reg->idade = -1;
	reg->removido = '0';
	reg->tamanhoRegistro = 0;
	reg->Prox = -1;
	reg->tamNomeJog = 0;
	reg->nomeJogador = NULL;
	reg->tamNacionalidade = 0;
	reg->nacionalidade = NULL;
	reg->tamNomeClube = 0;
	reg->nomeClube = NULL;

	return reg;
}

int register_calculateSize(REGISTER *reg){
	int tamanhoRegistro = 0;
	tamanhoRegistro = TAMANHO_BASE;

	tamanhoRegistro += reg->tamNomeJog;
	tamanhoRegistro += reg->tamNacionalidade;
	tamanhoRegistro += reg->tamNomeClube;

	return tamanhoRegistro;
}

REGISTER * register_readFromCSV(CSV *csv){
	REGISTER *reg;

	reg = register_create();

	/* a ordem é:
	 * id, idade, nome, nacionalidade, nome clube */

	/* lendo id */
	csv_nextTokenInt(csv, &reg->id);

	/* lendo idade */
	if(!csv_nextTokenInt(csv, &reg->idade)) reg->idade = -1;

	/* lendo nomeJogador */
	csv_nextToken(csv, &reg->nomeJogador, &reg->tamNomeJog);
	/* lendo nacionalidade */
	csv_nextToken(csv, &reg->nacionalidade, &reg->tamNacionalidade);
	/* lendo nomeClube */
	csv_nextToken(csv, &reg->nomeClube, &reg->tamNomeClube);

	/* função para calcular o tamanho do registro */
	reg->tamanhoRegistro = register_calculateSize(reg);

	return reg;
}

void register_print(REGISTER *reg){
	printf("Nome do Jogador: ");
	register_printStr(reg->nomeJogador, reg->tamNomeJog);

	printf("Nacionalidade do Jogador: ");
	register_printStr(reg->nacionalidade, reg->tamNacionalidade);
	
	printf("Clube do Jogador: ");
	register_printStr(reg->nomeClube, reg->tamNomeClube);
}

void register_printAll(REGISTER *reg){
	printf("ID : %d\n", reg->id);
	printf("Idade: %d\n", reg->idade);

	printf("Nome do Jogador: ");
	register_printStr(reg->nomeJogador, reg->tamNomeJog);

	printf("Nacionalidade do Jogador: ");
	register_printStr(reg->nacionalidade, reg->tamNacionalidade);
	
	printf("Clube do Jogador: ");
	register_printStr(reg->nomeClube, reg->tamNomeClube);
}

void register_debug(REGISTER *reg){
	printf("removido: %c\n", reg->removido);
	printf("tamanho: %d, %x\n", reg->tamanhoRegistro, reg->tamanhoRegistro);
	printf("proximo: %ld, %lx\n", reg->Prox, reg->Prox);
	printf("id: %d\n", reg->id);
	printf("idade: %d\n", reg->idade);
	register_print(reg);
	printf("\n");
}

void register_free(REGISTER *reg){
	if(reg == NULL) return;

	if(reg->nomeJogador != NULL)
		free(reg->nomeJogador);

	if(reg->nacionalidade != NULL)
		free(reg->nacionalidade);

	if(reg->nomeClube != NULL)
		free(reg->nomeClube);

	free(reg);
}

REGISTER* register_readFromBIN(FILE *file){
	int unused;

	REGISTER *reg;

	reg = register_create();

	fread(&(reg->removido), sizeof(reg->removido), 1, file);
	fread(&(reg->tamanhoRegistro), sizeof(reg->tamanhoRegistro), 1, file);
	
	fread(&(reg->Prox), sizeof(reg->Prox), 1, file);

	fread(&(reg->id), sizeof(reg->id), 1, file);
	fread(&(reg->idade), sizeof(reg->idade), 1, file);

	fread(&(reg->tamNomeJog), sizeof(reg->tamNomeJog), 1, file);
	if(reg->tamNomeJog != 0){
		reg->nomeJogador = (char*) malloc(reg->tamNomeJog*sizeof(char));
		fread(reg->nomeJogador, sizeof(char), reg->tamNomeJog, file);
	}
	
	fread(&(reg->tamNacionalidade), sizeof(reg->tamNacionalidade), 1, file);

	if(reg->tamNacionalidade != 0){
		reg->nacionalidade = (char*) malloc(reg->tamNacionalidade*sizeof(char));
		fread(reg->nacionalidade, sizeof(char), reg->tamNacionalidade, file);
	}

	fread(&(reg->tamNomeClube), sizeof(reg->tamNomeClube), 1, file);

	if(reg->tamNomeClube != 0){
		reg->nomeClube = (char*) malloc(reg->tamNomeClube*sizeof(char));
		fread(reg->nomeClube, sizeof(char), reg->tamNomeClube, file);
	}

	/* calcular o espaço inutilizado */
	unused = reg->tamanhoRegistro - register_calculateSize(reg);

	/* andar o espaço que foi inutilizado */
	fseek(file, unused, SEEK_CUR);

	return reg;
}

void register_write(FILE *file, REGISTER *reg){
	int unused;

	if(reg->tamanhoRegistro == 0)
		reg->tamanhoRegistro = register_calculateSize(reg);

	/* calcular o espaço inutilizado */
	unused = reg->tamanhoRegistro - register_calculateSize(reg);

	fwrite(&(reg->removido), sizeof(reg->removido), 1, file);
	fwrite(&(reg->tamanhoRegistro), sizeof(reg->tamanhoRegistro), 1, file);
	fwrite(&(reg->Prox), sizeof(reg->Prox), 1, file);
	fwrite(&(reg->id), sizeof(reg->id), 1, file);
	fwrite(&(reg->idade), sizeof(reg->idade), 1, file);
	fwrite(&(reg->tamNomeJog), sizeof(reg->tamNomeJog), 1, file);
	fwrite(reg->nomeJogador, sizeof(char), reg->tamNomeJog, file);
	fwrite(&(reg->tamNacionalidade), sizeof(reg->tamNacionalidade), 1, file);
	fwrite(reg->nacionalidade, sizeof(char), reg->tamNacionalidade, file);
	fwrite(&(reg->tamNomeClube), sizeof(reg->tamNomeClube), 1, file);
	fwrite(reg->nomeClube, sizeof(char), reg->tamNomeClube, file);

	/* preencher os caracteres inutilizados com '$' */
	for(int i = 0; i < unused; i++){
		fwrite("$", sizeof(char), 1, file);
	}
}

int register_cmp(REGISTER *a, REGISTER *b){
	/* Lembrando que apenas os campos não nulos de b são usados para fins de comparação */
	if(b->id != -1){
		if(a->id != b->id) return 0;
	}
	
	if(b->idade != -1){
		if(a->idade != b->idade) return 0;
	}

	if(b->nomeJogador != NULL){
		if(a->nomeJogador == NULL) return 0;
		if(strncmp(a->nomeJogador, b->nomeJogador, min(a->tamNomeJog, b->tamNomeJog)) != 0)
			return 0;
	}

	if(b->nacionalidade != NULL){
		if(a->nacionalidade == NULL) return 0;
		if(strncmp(a->nacionalidade, b->nacionalidade, min(a->tamNacionalidade, b->tamNacionalidade)) != 0)
			return 0;
	}

	if(b->nomeClube != NULL){
		if(a->nomeClube == NULL) return 0;
		if(strncmp(a->nomeClube, b->nomeClube, min(a->tamNomeClube, b->tamNomeClube)) != 0)
			return 0;
	}

	return 1;
}

void register_setNomeJogador(REGISTER *reg, const char *str){
	if(reg->nomeJogador != NULL) return;

	reg->tamNomeJog = strlen(str);
	reg->nomeJogador = malloc(1 + strlen(str));
	strcpy(reg->nomeJogador, str);
}

void register_setNacionalidade(REGISTER *reg, const char *str){
	if(reg->nacionalidade != NULL) return;

	reg->tamNacionalidade = strlen(str);
	reg->nacionalidade = malloc(1 + strlen(str));
	strcpy(reg->nacionalidade, str);
}

void register_setNomeClube(REGISTER *reg, const char *str){
	if(reg->nomeClube != NULL) return;

	reg->tamNomeClube = strlen(str);
	reg->nomeClube = malloc(1 + strlen(str));
	strcpy(reg->nomeClube, str);
}

REGISTER_IND * registerInd_create(void){
	REGISTER_IND * reg;
	reg = (REGISTER_IND *) malloc(sizeof(REGISTER_IND));

	reg->id = -1;
	reg->ByteOffset = -1;

	return reg;
}

REGISTER_IND* registerInd_readFromBIN(FILE *file){
	REGISTER_IND *reg;

	reg = registerInd_create();
	
	fread(&(reg->id), sizeof(reg->id), 1, file);
	fread(&(reg->ByteOffset), sizeof(reg->ByteOffset), 1, file);

	return reg;
}

void registerInd_write(FILE *file, REGISTER_IND *reg){
	fwrite(&(reg->id), sizeof(reg->id), 1, file);
	fwrite(&(reg->ByteOffset), sizeof(reg->ByteOffset), 1, file);
}

void registerInd_free(REGISTER_IND *reg){
	if(reg == NULL) return;

	free(reg);
}

int registerInd_cmp(REGISTER_IND *a, REGISTER_IND *b){
	/* Lembrando que apenas os campos não nulos de b são usados para fins de comparação */
	if(b->id != -1){
		if(a->id != b->id) return 0;
	}

	return 1;
}
