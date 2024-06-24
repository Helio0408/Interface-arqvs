#ifndef REGISTER_H
#define REGISTER_H

#include "def.h"
#include "csv.h"

/* cria um registro */
REGISTER * register_create(void);

/* essa função calcular o tamanho do registro */
int register_calculateSize(REGISTER *reg);

/* lê o registrador de um arquivo csv e retorna o registro */
REGISTER * register_readFromCSV(CSV *csv);

/* Lê um registro por vez e passa para o prox,
 * Retorna nulo caso o registro tenha sido logicamente removido */
REGISTER* register_readFromBIN(FILE *file);

/* Escreve o registro "reg" no arquivo "file", o arquivo
 * tem que já estar aberto */
void register_write(FILE *file, REGISTER *reg);

/* Imprime o registro na tela */
void register_print(REGISTER *reg);

/* Função de debug */
void register_debug(REGISTER *reg);

/* libera a memória utilizada por esse registro */
void register_free(REGISTER *reg);

/* Realiza uma comparação entre os campos não nulos de dois registros.
 * Se todos os não nulos forem iguais, ele 1.
 * Vale a pena ressaltar que o 'a' é o registro base e o 'b' é o de comparação.
 * Logo, se as comparações serão realizadas apenas contra os campos não nulos de 'b'
 * Caso contrário, retorna 0 */
int register_cmp(REGISTER *a, REGISTER *b);

/* As três funções abaixo são para o usuário definir as strings sem
 * manualmente alocar memória */
void register_setNomeJogador(REGISTER *reg, const char *str);

void register_setNacionalidade(REGISTER *reg, const char *str);

void register_setNomeClube(REGISTER *reg, const char *str);


/* ##### FUNÇÕES PARA INDEX ##### 
 * tem as mesmas funções das 
 * versões anteriores*/

REGISTER_IND * registerInd_create(void);

REGISTER_IND* registerInd_readFromBIN(FILE *file);

void registerInd_write(FILE *file, REGISTER_IND *reg);

void registerInd_free(REGISTER_IND *reg);

int registerInd_cmp(REGISTER_IND *a, REGISTER_IND *b);

#endif
