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
	IF("asd");
	THEN("");
	isEqual(initfs("dc/cripto1",19),SUCESSO);
}

void teste2() {
	DESCRIBE("Testa criacao sistema de arquivos");
	WHEN("Nenhum sistema de arquivo ainda existe");
	IF("asd");
	THEN("");
	initfs("dc/cripto2",200);
	isEqual(initfs("dc/cripto2",200),FALHA);
}

void teste3() {
	DESCRIBE("Testa criacao sistema de arquivos");
	WHEN("Nenhum sistema de arquivo ainda existe");
	IF("asd");
	THEN("");
	isEqual(initfs("dc/cripto3",18),FALHA);
}

int main() {
	teste1();
	teste2();
	teste3();
	return 0;
}
