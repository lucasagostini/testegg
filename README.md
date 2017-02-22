# Trabalho 2 - Sistema de Arquivos Criptografado

## Descrição

Para o trabalho 2, um sistema de arquivos criptografado será implementado em nível de usuário para sistemas de acordo com POSIX, especialmente Linux. Os alunos poderão realizar o trabalho em duplas.

Normalmente, para implementar um sistema de arquivos é necessário ter acesso direito aos dispositivos. Para simplificar a implementação e depuração, um arquivo grande sobre um outro sistema de arquivos será usado. Este arquivo será usado para armazenar os descritores de arquivos e os blocos que os compõem.

O tamanho de bloco a ser usado é 4 KiB.

O gerenciamento de blocos livres deve utilizar a tabela de alocação de arquivos. O índice do primeiro bloco livre estará armazenado nos primeiros 32 bits do primeiro bloco. 

A implementação deverá prover um diretório de um nível com 256 entradas, armazenado nos primeiros blocos do sistema de arquivos implementado. Os descritores deverão estar armazenados em formato binário, i.e., um descritor pode estar dividido em dois blocos. Cada descritor de arquivo deverá conter:

- O nome do arquivo com no máximo 255 caracteres UTF-8 (todos em zero se a entrada não estiver sendo usada)
- O tempo de criação do arquivo
- O tempo da última modificação
- O tempo do último acesso
- O índice do primeiro bloco
- O tamanho em bytes

Os índices que compõem todos os arquivos serão armazenados no bloco seguinte aos 19 blocos do diretório como um vetor de índices de 32 bits. Cada entrada indica qual o próximo bloco que pertence ao arquivo. O índice 0 é reservado para indicar que não há próximo bloco.

## Criptografia

Os blocos de dados dos arquivos devem ser criptografados com uma chave simples de deslocamento. Cada byte armazenado é rotacionado **n** vezes, onde **n** é a chave utilizada. Mais detalhes podem sem encontrados no artigo sobre Cifra de César na Wikipedia.

A chave **NÃO** deve ser armazenada junto ao arquivo. Ela deve ser informada quando o arquivo é aberto e armazenada em estrutura de dados temporária de arquivos abertos do sistema.

## Funções a serem implementadas

Detalhes dos parâmetros e da semântica das funções estão descritas na documentação do arquivo crypfs.h. Outras funções podem ser implementadas, mas não devem ser visíveis aos usuários finais. O arquivo crypfs.h **NÃO** deve ser modificado.

### initfs()

Função para inicializar um arquivo no sistema de arquivos hospedeiro com os blocos necessários.

Aloca os blocos, incluindo os blocos de metadados da estrutura do diretório.

### cry_openfs()

Função usada para abrir um sistema de arquivos já inicializado por initfs().


### cry_open()

Abre um arquivo criptografado com uma chave **n**. Deve ser chamada antes de qualquer outra operação com o arquivo e especificar se o arquivo é aberto para leitura, escrita ou ambos.

Retorna um file handler, NULL se houve erro.

### cry_read()

Lê **n** bytes de um arquivo. Os bytes são entregues decriptografados com a chave usada para abrir o arquivo. Retorna o número de bytes lidos.

### cry_write()

Escreve **n** bytes. Retorna **1** se sucesso, **0** se erro.

### cry_delete()

Remove o arquivo e o fecha.  Retorna **1** se sucesso, **0** se erro.

### cry_close()

Fecha o arquivo.  Retorna **1** se sucesso, **0** se erro.

## cry_seek()

Muda a posição atual no acesso ao arquivo. Retorna **1** se sucesso, **0** se erro.

## cry_creation(), cry_accessed(), cry_last_modified()

Retornam o tempo em que o arquivo foi criado, acessado e escrito pela última vez, respectivamente.

## Casos de Teste

O trabalho deverá conter um Makefile com regras para:

- Gerar o **cryfs.o** a partir da implementação das funções em um arquivo **cryfs.c**
- Gerar os casos de teste e executá-los (arquivo **teste.c**, regra **teste**)

O caso de teste deve criar um sistema de arquivos de nome **cripto**

Os casos de teste deverão demonstrar todas as funcionalidades implementadas. Para tanto, deve ser usado a biblioteca **simpletest**.

As flags **-std=c11** e **-Wall** são obrigatórias. O compilador GCC deve ser usado. O código deve compilar e executar em Linux.

## Avaliação

Serão pontuados:

- Implementação da `cryfs`: **8 pontos**
- Casos de teste dos alunos: **2 pontos**
- Implementação de journaling dos metadados: **1 ponto extra**
- Cada *warning*: **-0,5 pontos**


## Cronograma

- Entrega no github: **28/11/2016**

## Referências

* [Cifra de César -- Wikipedia] (https://pt.wikipedia.org/wiki/Cifra_de_C%C3%A9sar)
