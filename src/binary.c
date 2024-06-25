#include "binary.h"
#include "header.h"
#include "funcoes_fornecidas.h"
#include "indice.h"

/* Essa função pesquisa qualquer registro que tem os mesmos
 * parâmetros não nulos do registro base e imprime esse registro.
 * Caso não seja achado nenhum, ela imprime "registro inexistente */
static void bin_Search(FILE *file, REGISTER *base);

/* Essa função cria um index a partir de uma estrutura FILE */
static int bin_CreateIndexFromFILE(FILE *file, char *saida);

/* Essa função pesquisa qualquer registro que tem os mesmos
 * parâmetros não nulos do registro base e deleta esse registro.
 * Caso não seja achado nenhum, ela imprime "registro inexistente */
static void bin_SearchAndDelete(FILE *file, INDICE *indice, REGISTER *base);

/* Essa função é a que faz o papel de deletar o registro e pô-lo na lista de registros deletados */
static void bin_DeleteAuxList(FILE *file, HEADER *header, REGISTER *reg);

/* Essa é a função auxiliar que insere um registro no arquivo */
static void bin_InsertRegister(FILE *file, HEADER *header, REGISTER *reg);

/* Lê um registro atual da stdin com parâmetros de search */
static void bin_readRegisterSearch(REGISTER *base);

/* Lê um registro atual da stdin com parâmetros de insert */
static void bin_readRegisterInsert(REGISTER *base);

static void bin_Search(FILE *file, REGISTER *base){
	HEADER *h;
	REGISTER *reg;
	int cmp;
	int found = 0;

	fseek(file, 0, SEEK_SET); /* ler o arquivo do início  */

	h = header_read(file);

	/* vamos ter que passar por todo o arquivo, por isso somamos
	 * os registros logicamente removido e os não */
	for(int i = 0; i < h->nroRegArq + h->nroRegRem; i++){
		reg = register_readFromBIN(file);
		
		/* se o registro foi removido, liberemos a memória usada por ele */
		if(reg->removido == '1'){
			register_free(reg);
			continue;
		}

		cmp = register_cmp(reg, base);

		/* se os dois forem iguais, imprima um registro com quebra de linha */
		if(cmp){
			register_printAll(reg);
			found = 1;
			printf("\n");
		}

		/* liberemos a memória usada pelo registro */
		register_free(reg);

		/* Isso é para um caso específico.
		 * Cada id na tabela é único, ou seja, se já achamos
		 * um id que deu match, então podemos parar de pesquisar */
		if(cmp && reg->id == base->id) break;
	}

	/* Caso não seja achado nenhum registro, imprimir registro inexistente */
	if(!found) printf("Registro inexistente.\n\n");

	/* liberar a memória utilizada pelo header */
	header_free(h);
}

static int bin_CreateIndexFromFILE(FILE *dados, char *saida){
	FILE *index;
	HEADER *h;
	HEADER_IND *h_ind;
	REGISTER *reg;
	REGISTER_IND *reg_ind;
	int tam;

	fseek(dados, 0, SEEK_SET);

	h = header_read(dados);

	/* se o header estiver inválido, não criar o index */
	if(h->status != '1'){
		printf("Falha no processamento do arquivo.\n");
		header_free(h);
		return 0;
	}

	/* abrir o index */
	index = fopen(saida, "wb");

	h_ind = headerInd_create();
	reg_ind = registerInd_create();

	/* setar o tamanho para a soma de todos os registros */
	tam = h->nroRegArq + h->nroRegRem;
	header_free(h);

	fseek(index, HEADER_SIZE_IND, SEEK_SET);
	for(int i = 0; i < tam; i++){
		/* ler registro por registro */
		reg = register_readFromBIN(dados);

		/* checar se o registro é removido ou não */
		if(reg->removido != '1'){
			reg_ind->id = reg->id;
			/* Considerando que o ponteiro de leitura estará imediatamente
		 	* após o registro lido, devemos subtrair este do tamanho do registro */
			reg_ind->ByteOffset = ftell(dados) - reg->tamanhoRegistro;

			registerInd_write(index, reg_ind);
		}
		
		register_free(reg);
	}
	registerInd_free(reg_ind);
	
	h_ind->status = '1';
	
	rewind(index);
	/* escrever o cabeçalho do arquivo de index */
	headerInd_write(index, h_ind);

	headerInd_free(h_ind);

	fclose(index);

	return 1;
}

static void bin_SearchAndDelete(FILE *file, INDICE *indice, REGISTER *base){
	HEADER *h;
	REGISTER *reg;
	REGISTER_IND *reg_index;
	int cmp;

	fseek(file, 0, SEEK_SET); /* ler o arquivo do início  */

	h = header_read(file);

	if(base->id == -1){
		/* se a pesquisa não for feita com base no id */
		/* vamos ter que passar por todo o arquivo, por isso somamos
	 	* os registros logicamente removido e os não */
		for(int i = 0; i < h->nroRegArq + h->nroRegRem; i++){
			reg = register_readFromBIN(file);
		
			/* se o registro foi removido, liberemos a memória usada por ele */
			if(reg->removido == '1'){
				register_free(reg);
				continue;
			}

			cmp = register_cmp(reg, base);

			/* se os dois forem iguais, remova da lista */
			if(cmp){
				bin_DeleteAuxList(file, h, reg);
			}

			/* liberemos a memória usada pelo registro */
			register_free(reg);
		}
	}
	else{
		/* caso ela seja feita com base no id */
		for(int i = 0; i < indice->num_reg; i++){
			reg_index = indice->regs[i];

			if(reg_index->id == base->id){
				fseek(file, reg_index->ByteOffset, SEEK_SET);

				reg = register_readFromBIN(file);
				bin_DeleteAuxList(file, h, reg);

				register_free(reg);

				break;
			}
		}
	}

	fseek(file, 0, SEEK_SET);
	header_write(file, h);

	/* liberar a memória utilizada pelo header */
	header_free(h);
}

static void bin_DeleteAuxList(FILE *file, HEADER *header, REGISTER *reg){
	REGISTER *prev = NULL;
	REGISTER *next = NULL;
	long offset_reg, offset_prev_reg, offset_next_reg;

	reg->removido = '1'; /* marcar como logicamente removido */

	/* guardar o offset do registro a ser removido */
	offset_reg = ftell(file) - reg->tamanhoRegistro;

	/* atualizar os dados do header */
	header->nroRegArq--;
	header->nroRegRem++;

	/* agora a função para achar o lugar na lista de registros removidos.
	 * a lista está em ordem crescente, ou seja, o topo aponta para o
	 * menor elemento */
	/* Se não houver nenhum registor logicamente removido,
	 * o topo do header apontará para o registro atual */
	if(header->topo == -1){
		header->topo = offset_reg;
	}
	else /* caso contrário, vamos para o primeiro elemento */
		fseek(file, header->topo, SEEK_SET);

	/* Começar o loop apenas se houver mais de um registro removido */
	while(header->nroRegRem != 1){
		/* lendo o registro logicamente removido */
		next = register_readFromBIN(file);
		offset_next_reg = ftell(file) - next->tamanhoRegistro;

		/* como a lista está ordenada, quando acharmos o primeiro registro logicamente removido
		 * que é maior do que o registro que queremos, achamos sua posição na lista */
		if(reg->tamanhoRegistro < next->tamanhoRegistro){
			reg->Prox = offset_next_reg;

			if(prev == NULL){ /* Se o anterior era nulo, a posição achada foi a do primeiro elemento */
				header->topo = offset_reg;
			}
			else{ /* Se não era nulo, atualizar o registro anterior */
				prev->Prox = offset_reg;
				fseek(file, offset_prev_reg, SEEK_SET);
				register_write(file, prev);
				register_free(prev);
				prev = NULL;
			}

			register_free(next);
			next = NULL;
			break;
		}
		else if(next->Prox == -1){ 
			/* caso contrário, se não houver um próximo registro logicamente removido,
			 * atualizamos o ultimo registro logicamente removido */
			next->Prox = offset_reg;
			fseek(file, offset_next_reg, SEEK_SET);
			register_write(file, next);
			register_free(next);
			next = NULL;
			break;
		}
			
		/* liberar a memória utilizada pelo anterior */
		if(prev != NULL){
			register_free(prev);
			prev = NULL;
		}

		prev = next;
		next = NULL;
		offset_prev_reg = offset_next_reg;

		fseek(file, prev->Prox, SEEK_SET);
	}

	if(next != NULL) register_free(next);
	if(prev != NULL) register_free(prev);

	/* voltar para o início do endereço do registro */
	fseek(file, offset_reg, SEEK_SET);
	register_write(file, reg);
}

static void bin_InsertRegister(FILE *file, HEADER *header, REGISTER *reg){
	REGISTER *prev = NULL;
	REGISTER *next = NULL;
	long offset_prev_reg, offset_next_reg;

	/* atualizar os dados do header */
	header->nroRegArq++;

	/* agora a função para achar o lugar na lista de registros removidos.
	 * a lista está em ordem crescente, ou seja, o topo aponta para o
	 * menor elemento */
	/* se não houver elementos no topo, inserir o registro no final do arquivo */
	if(header->topo == -1){
		fseek(file, 0, SEEK_END);
		register_write(file, reg);
		header->proxByteOffset = ftell(file);
		return;
	}
	else /* caso contrário, vamos para o primeiro elemento */
		fseek(file, header->topo, SEEK_SET);

	/* Começar o loop apenas se houver mais de um registro removido */
	while(1){
		/* lendo o registro logicamente removido */
		next = register_readFromBIN(file);
		offset_next_reg = ftell(file) - next->tamanhoRegistro;

		/* Como a lista está ordenada, precisamos achar o primeiro registro logicamente
		 * removido que comporte o registro inserido (Best fit) */
		if(reg->tamanhoRegistro <= next->tamanhoRegistro){
			header->nroRegRem--; /* diminuíremos o número de registros removidos */

			if(prev == NULL){ /* se não há registro antigo, atualizaremos o header */
				header->topo = next->Prox;
			}
			else{ /* se há registro antigo, é necessário atualizá-lo */
				/* o registro anterior apontará para o registro que o registro atual apontava como próximo */
				prev->Prox = next->Prox;
				fseek(file, offset_prev_reg, SEEK_SET);
				register_write(file, prev);

				register_free(prev);
				prev = NULL;
			} 

			/* copiamos o tamanho do registro do next para o reg */
			reg->tamanhoRegistro = next->tamanhoRegistro;

			/* voltamos para o offset next reg */
			fseek(file, offset_next_reg, SEEK_SET);

			/* escrever o registro */
			register_write(file, reg);

			register_free(next);
			next = NULL;
			break;
		}
		else if(next->Prox == -1){ 
			/* caso contrário, se não houver um próximo registro logicamente removido
			 * e não houver nenhum registro que comporte seu tamanho,
			 * o registro será escrito no fim do arquivo */
			fseek(file, 0, SEEK_END);
			register_write(file, reg);

			register_free(next);
			next = NULL;
			register_free(prev);
			prev = NULL;

			/* atualizando o proxByteOffset do header */
			header->proxByteOffset = ftell(file);
			break;
		}
			
		/* liberar a memória utilizada pelo anterior */
		if(prev != NULL){
			register_free(prev);
			prev = NULL;
		}

		offset_prev_reg = offset_next_reg;
		prev = next;
		next = NULL;

		fseek(file, prev->Prox, SEEK_SET);
	}

	if(next != NULL) register_free(next);
	if(prev != NULL) register_free(prev);
}

static void bin_readRegisterSearch(REGISTER *base){
	char campo[64];
	char valor[256];
	int num_parametros;

	scanf("%d", &num_parametros);

	/* ler os campos */
	for(int j = 0; j < num_parametros; j++){
		scanf("%s", campo);

		if(strcmp(campo, "id") == 0){
			scanf("%d", &base->id);
		}
		if(strcmp(campo, "idade") == 0){
			scanf("%d", &base->idade);
		}
		if(strcmp(campo, "nomeJogador") == 0){
			scan_quote_string(valor);
			register_setNomeJogador(base, valor);
		}
		if(strcmp(campo, "nacionalidade") == 0){
			scan_quote_string(valor);
			register_setNacionalidade(base, valor);
		}
		if(strcmp(campo, "nomeClube") == 0){
			scan_quote_string(valor);
			register_setNomeClube(base, valor);
		}
	}
}

static void bin_readRegisterInsert(REGISTER *base){
	char valor[256];

	scanf("%d", &base->id);

	scan_quote_string(valor);

	if(strlen(valor) != 0)
		base->idade = atoi(valor);
	else
		base->idade = -1;

	scan_quote_string(valor);
	register_setNomeJogador(base, valor);

	scan_quote_string(valor);
	register_setNacionalidade(base, valor);

	scan_quote_string(valor);
	register_setNomeClube(base, valor);

	/* calcular o tamanho do registro, visto que inicialmente
	 * o seu valor é 0 */
	base->tamanhoRegistro = register_calculateSize(base);
}

void bin_CreateTable(char *entrada, char *saida){
	int numero_registro = 0;
	int final_do_arquivo = HEADER_SIZE;
	FILE *bin;
	HEADER *h;
	REGISTER *reg;
	CSV *csv;

	csv = csv_load(entrada);

	if(csv == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	h = header_create();
	reg = NULL;

	bin = fopen(saida, "wb");

	fseek(bin, HEADER_SIZE, SEEK_SET);

	/* a primeira linha do csv sempre é uma linha dando os nomes dos campos, logo
	 * podemos ignorar ela */

	csv_nextLine(csv);

	while(csv_nextLine(csv)){
		reg = register_readFromCSV(csv); /* aqui lemos o registro pelo csv */

		register_write(bin, reg); /* aqui escrevemos no arquivo binário adequado */

		final_do_arquivo += reg->tamanhoRegistro; /* mantemos o byteoffset do final do arquivo */

		numero_registro++; /* contamos o numero de registro */

		register_free(reg); /* liberamos a memória utilizada */
	}

	h->nroRegArq = numero_registro;
	h->status = '1';
	h->proxByteOffset = final_do_arquivo;

	rewind(bin);
	header_write(bin, h);

	/* liberemos a memória utilizada pelo header, csv e fechemos o arquivo */
	header_free(h);
	csv_free(csv);
	fclose(bin);

	binarioNaTela(saida); /* chamamos a função binário na tela */
}

void bin_SelectFrom(char *entrada){
	FILE *bin;
	HEADER *h;
	REGISTER *reg;

	bin = fopen(entrada, "rb");

	/* Verificação de existencia e de integridade
	 * do arquivo binário */

	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	h = header_read(bin);

	if(h->status != '1'){
		printf("Falha no processamento do arquivo.\n");
		header_free(h);
		return;
	}

	reg = NULL;

	/* Caso ele tenha 0 registros, imprimimos como registro inexistente */
	if(h->nroRegArq == 0){
		printf("Registro inexistente.\n\n");
		fclose(bin); /* fechar o arquivo */
		header_free(h); /* não esquecer de liberar a memória */
		return;
	}
	
	for(int i = 0; i < h->nroRegArq + h->nroRegRem; i++){
		reg = register_readFromBIN(bin);

		if(reg == NULL)
			continue;

		if(reg->removido == '1'){
			register_free(reg);
			continue;
		}

		register_printAll(reg);

		register_free(reg);
		printf("\n");
	}

	header_free(h);
	fclose(bin);
}

void bin_SelectFrom_Where(char *entrada, int num_buscas){
	FILE *file;
	HEADER *h;
	REGISTER *base;

	/* Nessa função, usaremos um registro de base,
	 * que será as bases de nossas comparações */

	file = fopen(entrada, "rb");

	/* Verificação de existencia e de integridade
	 * do arquivo binário */
	if(file == NULL){
		printf("Falha no processamento do arquivo.\n");
	}

	h = header_read(file);

	if(h->status != '1'){
		printf("Falha no processamento do arquivo.\n");
		header_free(h);
		return;
	}

	for(int i = 0; i < num_buscas; i++){
		base = register_create();

		bin_readRegisterSearch(base);

		printf("Busca %d\n\n", i + 1);

		bin_Search(file, base);

		/* liberar a memória utilizado no registro */
		register_free(base);
	}

	header_free(h);
	fclose(file);
}

void bin_CreateIndex(char *entrada, char *saida){
	FILE *dados;

	dados = fopen(entrada, "rb");
	
	/* Verificação de existencia e de integridade
	 * do arquivo binário */
	if(dados == NULL){
		printf("Falha no processamento do arquivo.\r\n");
		return;
	}

	/* se a criação do arquivo de índice foi bem sucedida,
	 * imprimir o binário na tela */
	if(bin_CreateIndexFromFILE(dados, saida)){
		binarioNaTela(saida);
	}

	fclose(dados);
}

void bin_Delete(char *entrada, char *indice, int num_deletes){
	FILE *file;
	INDICE *indice_file;
	REGISTER *base;

	/* Nessa função, usaremos um registro de base,
	 * que será as bases de nossas comparações */
	file = fopen(entrada, "rb+");

	bin_CreateIndexFromFILE(file, indice);

	indice_file = indice_load(indice);

	/* Verificação de existencia e de integridade
	 * do arquivo binário */
	if(file == NULL || indice_file == NULL){
		printf("Falha no processamento do arquivo.\n");
		if(file != NULL) fclose(file);
		if(indice_file != NULL) indice_free(indice_file);
		return;
	}

	for(int i = 0; i < num_deletes; i++){
		base = register_create();

		bin_readRegisterSearch(base);

		bin_SearchAndDelete(file, indice_file, base);

		/* liberar a memória utilizado no registro */
		register_free(base);
	}

	indice_free(indice_file);

	fclose(file);

	binarioNaTela(entrada);
	bin_CreateIndex(entrada, indice);
}

void bin_InsertInto(char *entrada, char *indice, int num_inserts){
	FILE *file, *indice_file;
	HEADER *header;
	REGISTER *base;

	/* Nessa função, usaremos um registro de base,
	 * que será as bases de nossas comparações */
	file = fopen(entrada, "rb+");

	bin_CreateIndexFromFILE(file, indice);

	indice_file = fopen(indice, "rb");

	/* Verificação de existencia e de integridade
	 * do arquivo binário */
	if(file == NULL || indice_file == NULL){
		printf("Falha no processamento do arquivo.\n");
		if(file != NULL) fclose(file);
		if(indice_file != NULL) fclose(file);
		return;
	}

	header = header_read(file);

	for(int i = 0; i < num_inserts; i++){
		base = register_create();

		bin_readRegisterInsert(base);

		/* inserir registro no arquivo */
		bin_InsertRegister(file, header, base);

		/* liberar a memória utilizado no registro */
		register_free(base);
	}

	/* voltar ao início e escrever o header com suas modificações */
	fseek(file, 0, SEEK_SET);
	header_write(file, header);
	header_free(header);

	fclose(indice_file);

	fclose(file);

	binarioNaTela(entrada);
	bin_CreateIndex(entrada, indice);
}

/* função de debug, ignorar */
void bin_debugprintdeletedlist(char *entrada){
	FILE *file;
	HEADER *h;
	REGISTER *reg;

	file = fopen(entrada, "rb");

	h = header_read(file);

	if(h->topo == -1) return;

	fseek(file, h->topo, SEEK_SET);

	while(ftell(file) < h->proxByteOffset){
		reg = register_readFromBIN(file);
		register_debug(reg);
		fseek(file, reg->Prox, SEEK_SET);
		if(reg->Prox == -1) break;
	}

	header_free(h);
}
