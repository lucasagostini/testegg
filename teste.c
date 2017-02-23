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

void teste4() {
	DESCRIBE("4");
	WHEN("");
	IF("");
	THEN("");
	initfs("dc/cripto4",19);
	isNotNull(cry_openfs("dc/cripto4"));
}

void teste5() {
	DESCRIBE("5");
	WHEN("");
	IF("");
	THEN("");
	isNull(cry_openfs("dc/cripto5"));
}

cry_desc_t * openfs(char* name) {
	initfs(name,19);
	return cry_openfs(name);
}

void teste6() {
	DESCRIBE("6");
	WHEN("");
	IF("");
	THEN("");
	cry_desc_t * fs = openfs("dc/cripto6");
	isNotEqual(cry_open(fs, "arquivo1", LEITURAESCRITA, 0), 0);
}

void teste7() {
	DESCRIBE("7");
	WHEN("");
	IF("");
	THEN("");
	cry_desc_t * fs = openfs("dc/cripto7");
	isEqual(cry_open(fs, "arquivo1", LEITURAESCRITA, 0), cry_open(fs, "arquivo1", LEITURAESCRITA, 0));
}

void teste8() {
	DESCRIBE("8");
	WHEN("");
	IF("");
	THEN("");
	cry_desc_t * fs = openfs("dc/cripto8");
	isNotEqual(cry_open(fs, "arquivo1", LEITURAESCRITA, 0), cry_open(fs, "arquivo2", LEITURAESCRITA, 0));
}

void teste9() {
	DESCRIBE("9");
	WHEN("");
	IF("");
	THEN("");
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
