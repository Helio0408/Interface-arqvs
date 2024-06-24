#include "csv.h"

CSV * csv_load(const char *filename){
	CSV *csv;

	csv = (CSV *) malloc(sizeof(CSV));

	csv->file = fopen(filename, "r");

	/* Retorna nulo se o arquivo não foi lido */
	if(csv->file == NULL){
		free(csv);
		return NULL;
	}

	return csv;
}

int csv_nextLine(CSV *csv){
	/* caso seja possível ler o csv, definitimos o valor da posição
	 * do caractere atual como 0 */
	if(fgets(csv->line, LINE_SIZE, csv->file) != NULL){
		csv->current_char = 0;
		return 1;
	}
	
	return 0;
}

int csv_nextToken(CSV *csv, char **token, uint32_t *tamanho){
	int original_char_pos, str_size;
	char ch;

	if(csv->line[csv->current_char] == '\0') return 0;
	if(csv->line[csv->current_char] == '\n') return 0;
	if(csv->line[csv->current_char] == '\r') return 0;

	original_char_pos = csv->current_char;

	/* loop iterando até achar o próximo '\n' ou ',' */
	while(1){
		ch = csv->line[csv->current_char];

		if(ch == ',' || ch == '\n' || ch == '\r') break;

		csv->current_char++;
	}

	/* calculando o tamanho da string até o ponto anterior ao ',' ou '\n' */
	str_size = csv->current_char - original_char_pos + 1;
	*tamanho = str_size - 1;

	/* se o tamanho for apenas o tamanho do '\0'*/
	if(str_size == 1){
		*token = NULL;
		csv->current_char++;
		return 1;
	}

	/* a variável *tamanho será salva com o tamanho da string sem o '\0' */
	*token = (char *) malloc(sizeof(char) * str_size);

	(*token)[*tamanho] = '\0';

	/* copiamos a string */
	strncpy(
			*token,
			csv->line + original_char_pos,
			str_size - 1
			);

	/* iteramos para o próximo caractere */
	csv->current_char++;

	return 1;
}

int csv_nextTokenInt(CSV *csv, int *token){
	uint32_t tamanho;
	int return_value;
	char *str = NULL;

	csv_nextToken(csv, &str, &tamanho);

	if(str == NULL) return 0;

	/* lê-se o próximo token como unsigned int */
	return_value = sscanf(str, "%d", token);

	/* libera a memória utilizada */
	free(str);

	/* caso a conversão tenha falhado, retorna 0 */
	if(return_value == EOF) return 0;

	return 1;
}

void csv_free(CSV *csv){
	fclose(csv->file);
	free(csv);
}
