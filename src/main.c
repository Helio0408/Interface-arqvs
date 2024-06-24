#include <stdio.h>

#include "binary.h"

/* Trabalho de Arquivos feito pelos alunos:
 * Gabriel Martins Monteiro - N° USP: 14572099
 * Hélio Márcio Cabral Santos - N° USP: 14577862 */

/* Estrutura dos arquivos
 * ./src
 * 		- def.h         -- definições como HEADER e REGISTER, que são a base do projeto.
 * 		- header.h      -- funções relacionadas ao cabeçalho.
 * 		- register.h    -- funções relacionadas à manipulação de registros.
 * 		- csv.h         -- funções relacionadas a leitura de um csv e um parser de csv 
 * 		- binary.h      -- funções relacionadas às operações binárias que podem ser feitas num arquivo
 * 	Makefile            -- arquivo Makefile para compilação
 *
 * 	Todos as implementações dessas funções estão em seus respectivos .c */

int main(void){
	int op;
	int num_busca;
	char entrada[25], saida[25], indice[25];
	
	scanf("%d", &op);

	switch(op){
		case -1:
			scanf("%s", entrada);
			bin_debugprintdeletedlist(entrada);
			break;
		case 1: 
			scanf("%s %s", entrada, saida);
			bin_CreateTable(entrada, saida);
			break;
	
		case 2:
			scanf("%s", entrada);
			bin_SelectFrom(entrada);
			break;

		case 3:
			scanf("%s %d", entrada, &num_busca);
			bin_SelectFrom_Where(entrada, num_busca);
			break;

		case 4:
			scanf("%s %s", entrada, saida);
			bin_CreateIndex(entrada, saida);
			break;

		case 5:
			scanf("%s %s %d", entrada, indice, &num_busca);
			bin_Delete(entrada, indice, num_busca);
			break;

		case 6:
			scanf("%s %s %d", entrada, indice, &num_busca);
			bin_InsertInto(entrada, indice, num_busca);
			break;
	
		default:
			printf("op invalida\n");
			break;
	}

	return 0;
}
