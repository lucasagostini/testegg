#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void test_simples() {
	DESCRIBE("Testa Com Uma Ocorrencia");
	WHEN("O arquivo de teste tem dados com uma substring");
	IF("one substring");
	THEN("o resultado deve ser 1 ");
	isEqual(ocorrencias("docs/input.txt", "teste", 4), 1);
}
void test_medio() {
	DESCRIBE("Testa Com poucas Ocorrencia");
	WHEN("O arquivo de teste tem dados com poucas substring");
	IF("18 substring");
	THEN("o resultado deve ser 18 ");
	isEqual(ocorrencias("docs/input2.txt", "teste", 4), 18);
}

void test_vazio() {

	DESCRIBE("Testa Vazio");
	WHEN("O arquivo de teste esta vazio");
	IF("input empty");
	THEN("o resultado deve ser 0 ");
	isEqual(ocorrencias("docs/input3.txt", "teste", 4), 0);
}

void test_full() {
	DESCRIBE("Testa Com muitas Ocorrencia");
	WHEN("O arquivo de teste tem dados com muitas substring");
	IF("3960 substring");
	THEN("o resultado deve ser 3960 ");
	isEqual(ocorrencias("docs/input4.txt", "teste", 4), 3960);
}
void test_nenhum() {

	DESCRIBE("Testa Nenhuma Ocorrencia");
	WHEN("O arquivo de teste tem dados mas nao tem substring");
	IF("no substring");
	THEN("o resultado deve ser 0 ");
	isEqual(ocorrencias("docs/input5.txt", "teste", 4), 0);
}

/*int main(){
	w = 0; linhas = 0; vthread = 0; contagem = 0;
	test_simples();
	w = 0; linhas = 0; vthread = 0; contagem = 0;
	test_medio();
	w = 0; linhas = 0; vthread = 0; contagem = 0;
	test_vazio();
	w = 0; linhas = 0; vthread = 0; contagem = 0;
	test_full();
	w = 0; linhas = 0; vthread = 0; contagem = 0;
	test_nenhum();

}*/