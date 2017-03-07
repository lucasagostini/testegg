#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cryfs.h"
#include "simpletest.h"

void teste1() {
	DESCRIBE("Testa criacao sistema de arquivos");
	WHEN("Nenhum sistema de arquivo ainda existe");
	IF("Um sistema Criado");
	THEN("A funcao retornara SUCESSO");
	isEqual(initfs("dc/cripto1",19),SUCESSO);
}

void teste2() {
	DESCRIBE("Testa criacao sistema de arquivos");
	WHEN("Dois sistemas sao criados com mesmo nome");
	IF("Dois Sistemas de Arquivos com mesmo nome");
	THEN("A funcao retorna FALHA");
	initfs("dc/cripto2",200);
	isEqual(initfs("dc/cripto2",200),FALHA);
}

void teste3() {
	DESCRIBE("Testa criacao sistema de arquivos");
	WHEN("Tamanho e menor que o minimo");
	IF("Numero de blocos menor que 19");
	THEN("O sistema nao pode ser criado");
	isEqual(initfs("dc/cripto3",18),FALHA);
}

void teste4() {
	DESCRIBE("Testa a abertura do sistema de arquivos");
	WHEN("Um sistema e aberto");
	IF("O sistema ja existe");
	THEN("A funcao retorna sucesso");
	initfs("dc/cripto4",19);
	isNotNull(cry_openfs("dc/cripto4"));
}

void teste5() {
	DESCRIBE("Testa a abertura do sistema de arquivos");
	WHEN("Nao existe nenhum Sistema");
	IF("Sistema nao foi criado");
	THEN("A funcao retorna falha");
	isNull(cry_openfs("dc/cripto5"));
}

cry_desc_t * openfs(char* name) {
	initfs(name,19);
	return cry_openfs(name);
}

void teste6() {
	DESCRIBE("Testa a abertura de um ou mais arquivos");
	WHEN("Sistema de arquivos ja existe, mas o arquivo nao");
	IF("Arquivo nao existe e atributo eh de LEITURAESCRITA");
	THEN("O arquivo deve ser criado e a funcao retorna o file handler");
	cry_desc_t * fs = openfs("dc/cripto6");
	isNotEqual(cry_open(fs, "arquivo1", LEITURAESCRITA, 0), 0);
}

void teste7() {
	DESCRIBE("Testa a abertura de um ou mais arquivos");
	WHEN("Um mesmo arquivo eh aberto mais de uma vez");
	IF("Arquivo ja esta aberto");
	THEN("O File Handler retornado deve ser o do arquivo ja aberto");
	cry_desc_t * fs = openfs("dc/cripto7");
	isEqual(cry_open(fs, "arquivo1", LEITURAESCRITA, 0), cry_open(fs, "arquivo1", LEITURAESCRITA, 0));
}

void teste8() {
	DESCRIBE("Testa a abertura de um ou mais arquivos");
	WHEN("Dois arquivos sao criados e abertos");
	IF("Dois arquivos diferentes sao abertos");
	THEN("A funcao deve retornar diferentes file handlers");
	cry_desc_t * fs = openfs("dc/cripto8");
	isNotEqual(cry_open(fs, "arquivo1", LEITURAESCRITA, 0), cry_open(fs, "arquivo2", LEITURAESCRITA, 0));
}

void teste9() {
	DESCRIBE("Testa a abertura de um ou mais arquivos");
	WHEN("O acesso e de LEITURA");
	IF("Acesso eh LEITURA e arquivo nao existe");
	THEN("A funcao deve retornar FALHA");
	cry_desc_t * fs = openfs("dc/cripto9");
	isEqual(cry_open(fs, "arquivo1", LEITURA, 0), 0);
}

void teste10() {
	DESCRIBE("Testa o fechamento de um arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo esta aberto");
	THEN("A funcao deve retornar SUCESSO");
	cry_desc_t * fs = openfs("dc/cripto10");
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isEqual(cry_close(indice), 1);
}

void teste11() {
	DESCRIBE("11");
	WHEN("");
	IF("");
	THEN("");
	cry_desc_t * fs = openfs("dc/cripto11");
	int indice1 = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_open(fs, "arquivo2", LEITURAESCRITA, 0);
	cry_close(indice1);
	isEqual(cry_open(fs, "arquivo3", LEITURAESCRITA, 0), 1);
}

void teste12() {
	DESCRIBE("12");
	WHEN("");
	IF("");
	THEN("");
	cry_desc_t * fs = openfs("dc/cripto12");
	char str[12];
	for (int i = 1; i <= 256; i++) {
		sprintf(str, "%d", i);
		if (cry_open(fs, strcat(str,"arquivo"), LEITURAESCRITA, 0) == FALHA) {
			isEqual(0, 1);
		}
	}
	isEqual(1, 1);
}

void teste13() {
	DESCRIBE("13");
	WHEN("");
	IF("");
	THEN("");
	cry_desc_t * fs = openfs("dc/cripto13");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isEqual(cry_write(arquivo, 3, "abc"), SUCESSO);
}

void teste14() {
	DESCRIBE("14");
	WHEN("");
	IF("");
	THEN("");
	cry_desc_t * fs = openfs("dc/cripto14");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_close(arquivo);
	isEqual(cry_write(arquivo, 3, "abc"), FALHA);
}

void teste15() {
	DESCRIBE("15");
	WHEN("");
	IF("");
	THEN("");
	cry_desc_t * fs = openfs("dc/cripto15");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_close(arquivo);
	arquivo = cry_open(fs, "arquivo1", LEITURA, 0);
	isEqual(cry_write(arquivo, 3, "abc"), FALHA);
}

/*
void teste32() {
	DESCRIBE("Testa a modificacao do ponteiro de seek");
	WHEN("O File Handler eh passado como parametro junto com a posicao desejada");
	IF("Arquivo existe");
	THEN("A funcao deve retornar SUCESSO");
	cry_desc_t * fs = openfs("dc/cripto32");
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isNotEqual(cry_seek(indice, 320), 0);
}

void teste33() {
	DESCRIBE("Testa a leitura do tempo de criacao do arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo existe");
	THEN("A funcao deve retornar o tempo de criacao do mesmo");
	cry_desc_t * fs = openfs("dc/cripto33");
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isNotEqual(cry_creation(indice), 0);
}

void teste34() {
	DESCRIBE("Testa a leitura do tempo de acesso do arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo existe");
	THEN("A funcao deve retornar o tempo de criacao do mesmo");
	cry_desc_t * fs = openfs("dc/cripto34");
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isNotEqual(cry_accessed(indice), 0);
}

void teste35() {
	DESCRIBE("Testa a leitura do tempo de modificacao do arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo existe");
	THEN("A funcao deve retornar o tempo de modificacao do mesmo");
	cry_desc_t * fs = openfs("dc/cripto35");
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isNotEqual(cry_last_modified(indice), 0);
}
*/
int main() {
	teste1();
	teste2();
	teste3();
	teste4();
	teste5();
	teste6();
	teste7();
	teste8();
	teste9();
	teste10();
	teste11();
	teste12();
	teste13();
	teste14();
	teste15();
	return 0;
}
