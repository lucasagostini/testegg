#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "cryfs.h"
#include "simpletest.h"

void descriptor_cleaner() {

}

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
	IF("Arquivo nao existe e atributo e de LEITURAESCRITA");
	THEN("O arquivo deve ser criado e a funcao retorna o file handler");
	cry_desc_t * fs = openfs("dc/cripto6");
	isNotEqual(cry_open(fs, "arquivo1", LEITURAESCRITA, 0), 0);
}

void teste7() {
	DESCRIBE("Testa a abertura de um ou mais arquivos");
	WHEN("Um mesmo arquivo e aberto mais de uma vez");
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
	IF("Acesso e LEITURA e arquivo nao existe");
	THEN("A funcao deve retornar FALHA");
	cry_desc_t * fs = openfs("dc/cripto9");
	isEqual(cry_open(fs, "arquivo1", LEITURA, 0), 0);
}

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
	return 0;
}
