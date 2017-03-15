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
	initfs(name,1000);
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
	DESCRIBE("Testa a abertura de um arquivo apos o fechamento de outro");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo 1 esta aberto e 2 fechado");
	THEN("A funcao deve retornar SUCESSO");
	cry_desc_t * fs = openfs("dc/cripto11");
	int indice1 = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_open(fs, "arquivo2", LEITURAESCRITA, 0);
	cry_close(indice1);
	isEqual(cry_open(fs, "arquivo3", LEITURAESCRITA, 0), 1);
}

void teste12() {
	DESCRIBE("Testa a abertura de 256 arquivos");
	WHEN("Nao tem nenhum arquivo aberto");
	IF("256 arquivos sao abertos");
	THEN("A funcao deve retornar sucesso");
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
	DESCRIBE("Testa a escrita simples em um arquivo");
	WHEN("Arquivo esta aberto");
	IF("Temos espaco para escrever");
	THEN("A funcao escreve e retorna sucesso");
	cry_desc_t * fs = openfs("dc/cripto13");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isEqual(cry_write(arquivo, 3, "abc"), SUCESSO);
}

void teste14() {
	DESCRIBE("Testa a escrita simples em um arquivo");
	WHEN("Arquivo esta fechado");
	IF("Arquivo fechado");
	THEN("A funcao retorna falha");
	cry_desc_t * fs = openfs("dc/cripto14");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_close(arquivo);
	isEqual(cry_write(arquivo, 3, "abc"), FALHA);
}

void teste15() {
	DESCRIBE("Testa a escrita simples em um arquivo");
	WHEN("Quando o arquivo eh fechado e depois reaberto");
	IF("Arquivo esta aberto");
	THEN("A funcao deve retornar SUCESSO");
	cry_desc_t * fs = openfs("dc/cripto15");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_close(arquivo);
	arquivo = cry_open(fs, "arquivo1", LEITURA, 0);
	isEqual(cry_write(arquivo, 3, "abc"), FALHA);
}

void teste16() {
	DESCRIBE("Testa a leitura simples de um arquivo");
	WHEN("Ele esta vazio");
	IF("Nao tem nada escrito");
	THEN("A funcao nao pode ler nada, retorna falha");
	cry_desc_t * fs = openfs("dc/cripto16");
	int arquivo = cry_open(fs, "arquivo1", ESCRITA, 0);
	char out[100];
	isEqual(cry_read(arquivo, 100, out), FALHA);
}

void teste17() {
	DESCRIBE("Testa a leitura simples de um arquivo");
	WHEN("Ele possui dados escritos");
	IF("O que foi lido eh igual ao que foi escrito");
	THEN("A funcao deve retornar sucesso");
	cry_desc_t * fs = openfs("dc/cripto17");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste17");
	char out[7];
	int lidos = cry_read(arquivo, 8, out);
	isEqual(lidos, 7);
	isEqual(strncmp(out, "teste17", lidos), 0);
}

void teste18() {
	DESCRIBE("Testa a funcao seek");
	WHEN("Temos dados escritos no arquivo");
	IF("Damos seek para a posicao 2 do arquivo");
	THEN("A leitura deve ser feita a partir da segunda posicao");
	cry_desc_t * fs = openfs("dc/cripto18");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste18");
	char out[5];
	isEqual(cry_seek(arquivo, 2), SUCESSO);
	int lidos = cry_read(arquivo, 8, out);
	isEqual(lidos, 5);
	isEqual(strncmp(out, "ste18", lidos), 0);
}

void teste19() {
	DESCRIBE("Testa a funcao seek");
	WHEN("Escrevemos 7 char em um arquivo");
	IF("Damos seek para a posicao 7");
	THEN("A funcao deve retornar sucesso");
	cry_desc_t * fs = openfs("dc/cripto19");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste19");
	isEqual(cry_seek(arquivo, 7), SUCESSO);
}

void teste20() {
	DESCRIBE("Testa a funcao seek");
	WHEN("Escrevemos 7 char em um arquivo");
	IF("Damos seek para a posicao 8");
	THEN("A funcao deve retornar falha");
	cry_desc_t * fs = openfs("dc/cripto20");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste20");
	isEqual(cry_seek(arquivo, 8), FALHA);
}

void teste21() {
	DESCRIBE("Testa a funcao delete");
	WHEN("Um arquivo existe com dados dentro");
	IF("Mandamos deletar ele e depois recriar");
	THEN("O arquivo deve estar vazio, entao o seek(1) deve retornar falha");
	cry_desc_t * fs = openfs("dc/cripto21");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste21");
	cry_delete(arquivo);
	arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isEqual(cry_seek(arquivo, 1), FALHA);
}

void teste22() {
	DESCRIBE("Testa a funcao delete");
	WHEN("Um arquivo existe com dados dentro");
	IF("Mandamos deletar ele e depois abrir em modo leitura");
	THEN("A abertura deve retornar falha pois o arquivo nao existe e o modo eh leitura");
	cry_desc_t * fs = openfs("dc/cripto22");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste22");
	cry_delete(arquivo);
	arquivo = cry_open(fs, "arquivo1", LEITURA, 0);
	isEqual(arquivo, FALHA);
}

void teste23() {
	DESCRIBE("Testa a leitura do tempo de criacao do arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo existe");
	THEN("A funcao deve retornar o tempo de criacao do mesmo");
	cry_desc_t * fs = openfs("dc/cripto23");
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isNotEqual(cry_creation(indice), 0);
	isEqual(cry_last_modified(indice), 0);
}

void teste24() {
	DESCRIBE("Testa a leitura do tempo de acesso do arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo existe");
	THEN("A funcao deve retornar o tempo de criacao do mesmo");
	cry_desc_t * fs = openfs("dc/cripto24");
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isNotEqual(cry_accessed(indice), 0);
	isEqual(cry_last_modified(indice), 0);
}

void teste25() {
	DESCRIBE("Testa a leitura do tempo de modificacao do arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo existe");
	THEN("A funcao deve retornar o tempo de modificacao do mesmo");
	cry_desc_t * fs = openfs("dc/cripto25");
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(indice, 7, "teste25");
	isNotEqual(cry_last_modified(indice), 0);
}

void teste26() {
	DESCRIBE("Testa leitura de um mesmo arquivo com duas chaves diferentes");
	WHEN("Abrimos um mesmo arquivo com duas chaves diferentes");
	IF("Escrevermos com uma chave e depois lermos usando chaves diferentes");
	THEN("As strings nao podem ser iguais");
	cry_desc_t * fs = openfs("dc/cripto26");
	int arquivo1 = cry_open(fs, "arquivo1", LEITURAESCRITA, 2);
	cry_write(arquivo1, 7, "teste26");
	char out1[7],out2[7];
	int lidos = cry_read(arquivo1, 8, out1);
	isEqual(lidos, 7);
	int arquivo2 = cry_open(fs, "arquivo1", LEITURAESCRITA, 3);
	lidos = cry_read(arquivo2, 8, out1);
	isEqual(lidos, 7);
	isNotEqual(strncmp(out1, out2, lidos), 0);
}


void teste27() {
	DESCRIBE("Testa leitura de um arquivo com uma chave");
	WHEN("Abrimos um arquivo usando somente uma chave");
	IF("Escrevermos com uma chave e depois lermos com a mesma chave");
	THEN("As strings devem ser iguais");
	cry_desc_t * fs = openfs("dc/cripto27");
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 2);
	cry_write(arquivo, 7, "teste27");
	char out[7];
	int lidos = cry_read(arquivo, 8, out);
	isEqual(lidos, 7);
	isEqual(strncmp(out, "teste27", lidos), 0);
}

void teste28() {
	DESCRIBE("Testa escrita/leitura de um mesmo arquivo com duas chaves diferentes");
	WHEN("Abrimos um mesmo arquivo com duas chaves diferentes");
	IF("Escrevermos com chaves diferentes e depois lermos usando a chave certa");
	THEN("As strings devem fazer sentido e serem iguais");
	cry_desc_t * fs = openfs("dc/cripto28");
	char out1[7],out2[7];
	int arquivo1 = cry_open(fs, "arquivo1", LEITURAESCRITA, 2);
	cry_write(arquivo1, 7, "teste28");
	int lidos1 = cry_read(arquivo1, 8, out1);
	isEqual(lidos1, 7);
	cry_close(arquivo1);
	int arquivo2 = cry_open(fs, "arquivo1", LEITURAESCRITA, 3);
	cry_write(arquivo2, 7, "teste28");
	cry_seek(arquivo2, 7);
	int lidos2 = cry_read(arquivo2, 8, out2);
	isEqual(lidos2, 7);
	cry_close(arquivo2);
	isEqual(strncmp(out1, out2, lidos2), 0);
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
	teste10();
	teste11();
	teste12();
	teste13();
	teste14();
	teste15();
	teste16();
	teste17();
	teste18();
	teste19();
	teste20();
	teste21();
	teste22();
	teste23();
	teste24();
	teste25();
	teste26();
	teste27();
	teste28();
	return 0;
}
