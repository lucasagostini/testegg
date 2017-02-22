#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "simpletest.h"
#include "calcula.h"
#include "calculat.c"

int main(){
	//linhas = 0; contagem = 0;
	test_simples();
	//linhas = 0; contagem = 0;
	test_medio();
	//linhas = 0; contagem = 0;	
	test_vazio();
	//linhas = 0; contagem = 0;	
	test_full();
	//linhas = 0; contagem = 0;	
	test_nenhum();

}