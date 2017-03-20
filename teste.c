#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cryfs.h"
#include "simpletest.h"

cry_desc_t * openfs() {
	initfs("cripto",100000);
	return cry_openfs("cripto");
}

int removendo(){
	remove("cripto");
	remove("cripto.log");
	return 0;
}

void teste1() {
	removendo();
	DESCRIBE("1-Testa criacao sistema de arquivos");
	WHEN("Nenhum sistema de arquivo ainda existe");
	IF("Um sistema Criado");
	THEN("A funcao retornara SUCESSO");
	isEqual(initfs("cripto",19),SUCESSO);
	removendo();
}

void teste2() {
	DESCRIBE("2-Testa criacao sistema de arquivos");
	WHEN("Dois sistemas sao criados com mesmo nome");
	IF("Dois Sistemas de Arquivos com mesmo nome");
	THEN("A funcao retorna FALHA");
	initfs("cripto",200);
	isEqual(initfs("cripto",200),FALHA);
	removendo();
}

void teste3() {
	DESCRIBE("3-Testa criacao sistema de arquivos");
	WHEN("Tamanho e menor que o minimo");
	IF("Numero de blocos menor que 19");
	THEN("O sistema nao pode ser criado");
	isEqual(initfs("cripto",18),FALHA);
	removendo();
}

void teste4() {
	DESCRIBE("4-Testa a abertura do sistema de arquivos");
	WHEN("Um sistema e aberto");
	IF("O sistema ja existe");
	THEN("A funcao retorna sucesso");
	initfs("cripto",19);
	isNotNull(cry_openfs("cripto"));
	removendo();
}

void teste5() {
	DESCRIBE("5-Testa a abertura do sistema de arquivos");
	WHEN("Nao existe nenhum Sistema");
	IF("Sistema nao foi criado");
	THEN("A funcao retorna falha");
	isNull(cry_openfs("cripto"));
	removendo();
}



void teste6() {
	DESCRIBE("6-Testa a abertura de um ou mais arquivos");
	WHEN("Sistema de arquivos ja existe, mas o arquivo nao");
	IF("Arquivo nao existe e atributo eh de LEITURAESCRITA");
	THEN("O arquivo deve ser criado e a funcao retorna o file handler");
	cry_desc_t * fs = openfs();
	isNotEqual(cry_open(fs, "arquivo1", LEITURAESCRITA, 0), 0);
	removendo();
}

void teste7() {
	DESCRIBE("7-Testa a abertura de um ou mais arquivos");
	WHEN("Um mesmo arquivo eh aberto mais de uma vez");
	IF("Arquivo ja esta aberto");
	THEN("O File Handler retornado deve ser o do arquivo ja aberto");
	cry_desc_t * fs = openfs();
	isEqual(cry_open(fs, "arquivo1", LEITURAESCRITA, 0), cry_open(fs, "arquivo1", LEITURAESCRITA, 0));
	removendo();
}

void teste8() {
	DESCRIBE("8-Testa a abertura de um ou mais arquivos");
	WHEN("Dois arquivos sao criados e abertos");
	IF("Dois arquivos diferentes sao abertos");
	THEN("A funcao deve retornar diferentes file handlers");
	cry_desc_t * fs = openfs();
	isNotEqual(cry_open(fs, "arquivo1", LEITURAESCRITA, 0), cry_open(fs, "arquivo2", LEITURAESCRITA, 0));
	removendo();
}

void teste9() {
	DESCRIBE("9-Testa a abertura de um ou mais arquivos");
	WHEN("O acesso e de LEITURA");
	IF("Acesso eh LEITURA e arquivo nao existe");
	THEN("A funcao deve retornar FALHA");
	cry_desc_t * fs = openfs();
	isEqual(cry_open(fs, "arquivo9", LEITURA, 0), 0);
	removendo();
}

void teste10() {
	DESCRIBE("10-Testa o fechamento de um arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo esta aberto");
	THEN("A funcao deve retornar SUCESSO");
	cry_desc_t * fs = openfs();
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isEqual(cry_close(indice), 1);
	removendo();
}

void teste11() {
	DESCRIBE("11-Testa a abertura de um arquivo apos o fechamento de outro");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo 1 esta aberto e 2 fechado");
	THEN("A funcao deve retornar SUCESSO");
	cry_desc_t * fs = openfs();
	int indice1 = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_open(fs, "arquivo2", LEITURAESCRITA, 0);
	cry_close(indice1);
	isEqual(cry_open(fs, "arquivo3", LEITURAESCRITA, 0), 1);
	removendo();
}

void teste12() {
	DESCRIBE("12-Testa a abertura de 256 arquivos");
	WHEN("Nao tem nenhum arquivo aberto");
	IF("256 arquivos sao abertos");
	THEN("A funcao deve retornar sucesso");
	cry_desc_t * fs = openfs("cripto2");
	char str[12];
	for (int i = 1; i <= 256; i++) {
		sprintf(str, "%d", i);
		if (cry_open(fs, strcat(str,"arquivo"), LEITURAESCRITA, 0) == FALHA) {
			isEqual(0, 1);
		}
	}
	isEqual(1, 1);
	removendo();
}

void teste13() {
	DESCRIBE("13-Testa a escrita simples em um arquivo");
	WHEN("Arquivo esta aberto");
	IF("Temos espaco para escrever");
	THEN("A funcao escreve e retorna sucesso");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isEqual(cry_write(arquivo, 3, "abc"), SUCESSO);
	removendo();
}

void teste14() {
	DESCRIBE("14-Testa a escrita simples em um arquivo");
	WHEN("Arquivo esta fechado");
	IF("Arquivo fechado");
	THEN("A funcao retorna falha");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_close(arquivo);
	isEqual(cry_write(arquivo, 3, "abc"), FALHA);
	removendo();
}

void teste15() {
	DESCRIBE("15-Testa a escrita simples em um arquivo");
	WHEN("Quando o arquivo eh fechado e depois reaberto");
	IF("Arquivo esta aberto");
	THEN("A funcao deve retornar SUCESSO");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_close(arquivo);
	arquivo = cry_open(fs, "arquivo1", LEITURA, 0);
	isEqual(cry_write(arquivo, 3, "abc"), FALHA);
	removendo();
}

void teste16() {
	DESCRIBE("16-Testa a leitura simples de um arquivo");
	WHEN("Ele esta vazio");
	IF("Nao tem nada escrito");
	THEN("A funcao nao pode ler nada, retorna falha");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", ESCRITA, 0);
	char out[100];
	isEqual(cry_read(arquivo, 100, out), FALHA);
	removendo();
}

void teste17() {
	DESCRIBE("17-Testa a leitura simples de um arquivo");
	WHEN("Ele possui dados escritos");
	IF("O que foi lido eh igual ao que foi escrito");
	THEN("A funcao deve retornar sucesso");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste17");
	char out[7];
	int lidos = cry_read(arquivo, 7, out);
	isEqual(lidos, 7);
	isEqual(strncmp(out, "teste17", lidos), 0);
	removendo();
}

void teste18() {
	DESCRIBE("18-Testa a funcao seek");
	WHEN("Temos dados escritos no arquivo");
	IF("Damos seek para a posicao 2 do arquivo");
	THEN("A leitura deve ser feita a partir da segunda posicao");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste18");
	char out[5];
	isEqual(cry_seek(arquivo, 2), SUCESSO);
	int lidos = cry_read(arquivo, 5, out);
	isEqual(lidos, 5);
	isEqual(strncmp(out, "ste18", lidos), 0);
	removendo();
}

void teste19() {
	DESCRIBE("19-Testa a funcao seek");
	WHEN("Escrevemos 7 char em um arquivo");
	IF("Damos seek para a posicao 7");
	THEN("A funcao deve retornar sucesso");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste19");
	isEqual(cry_seek(arquivo, 7), SUCESSO);
	removendo();
}

void teste20() {
	DESCRIBE("20-Testa a funcao seek");
	WHEN("Escrevemos 7 char em um arquivo");
	IF("Damos seek para a posicao 8");
	THEN("A funcao deve retornar falha");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste20");
	isEqual(cry_seek(arquivo, 8), FALHA);
	removendo();
}

void teste21() {
	DESCRIBE("21-Testa a funcao delete");
	WHEN("Um arquivo existe com dados dentro");
	IF("Mandamos deletar ele e depois recriar");
	THEN("O arquivo deve estar vazio, entao o seek(1) deve retornar falha");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste21");
	cry_delete(arquivo);
	arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isEqual(cry_seek(arquivo, 1), FALHA);
	removendo();
}

void teste22() {
	DESCRIBE("22-Testa a funcao delete");
	WHEN("Um arquivo existe com dados dentro");
	IF("Mandamos deletar ele e depois abrir em modo leitura");
	THEN("A abertura deve retornar falha pois o arquivo nao existe e o modo eh leitura");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(arquivo, 7, "teste22");
	cry_delete(arquivo);
	arquivo = cry_open(fs, "arquivo1", LEITURA, 0);
	isEqual(arquivo, FALHA);
	removendo();
}

void teste23() {
	DESCRIBE("23-Testa a leitura do tempo de criacao do arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo existe");
	THEN("A funcao deve retornar o tempo de criacao do mesmo");
	cry_desc_t * fs = openfs();
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isNotEqual(cry_creation(indice), 0);
	isEqual(cry_last_modified(indice), 0);
	removendo();
}

void teste24() {
	DESCRIBE("24-Testa a leitura do tempo de acesso do arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo existe");
	THEN("A funcao deve retornar o tempo de criacao do mesmo");
	cry_desc_t * fs = openfs();
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	isNotEqual(cry_accessed(indice), 0);
	isEqual(cry_last_modified(indice), 0);
	removendo();
}

void teste25() {
	DESCRIBE("25-Testa a leitura do tempo de modificacao do arquivo");
	WHEN("O File Handler eh passado como parametro");
	IF("Arquivo existe");
	THEN("A funcao deve retornar o tempo de modificacao do mesmo");
	cry_desc_t * fs = openfs();
	int indice = cry_open(fs, "arquivo1", LEITURAESCRITA, 0);
	cry_write(indice, 7, "teste25");
	isNotEqual(cry_last_modified(indice), 0);
	removendo();
}

void teste26() {
	DESCRIBE("26-Testa leitura de um mesmo arquivo com duas chaves diferentes");
	WHEN("Abrimos um mesmo arquivo com duas chaves diferentes");
	IF("Escrevermos com uma chave e depois lermos usando chaves diferentes");
	THEN("As strings nao podem ser iguais");
	cry_desc_t * fs = openfs();
	int arquivo1 = cry_open(fs, "arquivo1", LEITURAESCRITA, 2);
	cry_write(arquivo1, 7, "teste26");
	char out1[7],out2[7];
	int lidos = cry_read(arquivo1, 7, out1);
	isEqual(lidos, 7);
	int arquivo2 = cry_open(fs, "arquivo1", LEITURAESCRITA, 3);
	lidos = cry_read(arquivo2, 7, out1);
	isEqual(lidos, 7);
	isNotEqual(strncmp(out1, out2, lidos), 0);
	removendo();
}
void teste27() {
	DESCRIBE("27-Testa leitura de um arquivo com uma chave");
	WHEN("Abrimos um arquivo usando somente uma chave");
	IF("Escrevermos com uma chave e depois lermos com a mesma chave");
	THEN("As strings devem ser iguais");
	cry_desc_t * fs = openfs();
	int arquivo = cry_open(fs, "arquivo1", LEITURAESCRITA, 2);
	cry_write(arquivo, 7, "teste27");
	char out[7];
	int lidos = cry_read(arquivo, 7, out);
	isEqual(lidos, 7);
	isEqual(strncmp(out, "teste27", lidos), 0);
	removendo();
}

void teste28() {
	DESCRIBE("28-Testa escrita/leitura de um mesmo arquivo com duas chaves diferentes");
	WHEN("Abrimos um mesmo arquivo com duas chaves diferentes");
	IF("Escrevermos com chaves diferentes e depois lermos usando a chave certa");
	THEN("As strings devem fazer sentido e serem iguais");
	cry_desc_t * fs = openfs();
	char out1[7],out2[7];
	int arquivo1 = cry_open(fs, "arquivo1", LEITURAESCRITA, 2);
	cry_write(arquivo1, 7, "teste28");
	int lidos1 = cry_read(arquivo1, 7, out1);
	isEqual(lidos1, 7);
	isEqual(cry_close(arquivo1), SUCESSO);
	int arquivo2 = cry_open(fs, "arquivo1", LEITURAESCRITA, 3);
	cry_write(arquivo2, 7, "teste28");
	cry_seek(arquivo2, 7);
	int lidos2 = cry_read(arquivo2, 7, out2);
	isEqual(lidos2, 7);
	cry_close(arquivo2);
	isEqual(strncmp(out1, out2, lidos2), 0);
	removendo();
}

void teste_final() {
	DESCRIBE("Final- Testa todas as funcoes do sistema de arquivos");
	WHEN("Executamos todas as funcoes");
	IF("Todas funcionarem corretamente");
	THEN("O retorno deve ser SUCESSO");
	cry_desc_t * fs = openfs();
	char out1[7],out2[7];
	int arquivo1 = cry_open(fs, "arquivo1", LEITURAESCRITA, 2);
	cry_write(arquivo1, 7, "teste28");
	int lidos1 = cry_read(arquivo1, 7, out1);
	isEqual(lidos1, 7);
	isEqual(cry_close(arquivo1), SUCESSO);
	int arquivo2 = cry_open(fs, "arquivo1", LEITURAESCRITA, 3);
	cry_write(arquivo2, 7, "teste28");
	isEqual(cry_seek(arquivo2, 7),SUCESSO);
	int lidos2 = cry_read(arquivo2, 7, out2);
	isEqual(lidos2, 7);
	isGreaterThan(cry_accessed(arquivo2),0);
	isGreaterThan(cry_creation(arquivo2),0);
	isGreaterThan(cry_last_modified(arquivo2),0);
	isGreaterThan(cry_delete(arquivo2),0);
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
	teste_final();
	return 0;
}
