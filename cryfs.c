#include "cryfs.h"
#include <unistd.h>
#include <string.h>

/** Inicializa o arquivo no sistema de arquivos hospedeiro.
 *
 *  Deve ser chamada apenas uma vez para um determinado sistema de arquivos.
 *  Caso o nome de arquivo já exista, deve retornar erro.
 *  Também deve retornar erro caso o número de blocos não seja o suficiente
 *  para armazenar os metadados pelo menos.
 *
 *  @param arquivo nome do arquivo no sistema de arquivos hospedeiro
 *  @param blocos número de blocos do arquivo
 *  @return SUCCESSO ou FALHA
 */

const int BLOCOS_MIN = (sizeof(descritor_t) * 256 / 4096) + 1;

cry_desc_t descritor_fs;

int initfs(char * arquivo, int blocos) {
  if (blocos < BLOCOS_MIN) {
    return FALHA;
  }
  if (access(arquivo,F_OK) != -1) {
    return FALHA;
  }

  for(int i = 0; i < 256; i++) {
    descritor_fs.descritores[i].nome[0] = 0;
    descritor_fs.abertos[i].arquivo = NULL;
  }
  FILE* fp = fopen(arquivo, "w+");
  return (fp != 0) ? SUCESSO : FALHA;
}


/** Abre um sistema de arquivos.
 *
 *  @param nome do arquivo no sistema de arquivos hospedeiro
 *  @return ponteiro para descritor de sistema de arquivos ou FALHA
 */

cry_desc_t * cry_openfs(char * arquivo) {
  if (access(arquivo,F_OK) == -1) {
    return FALHA;
  }
  FILE* fp = fopen(arquivo, "r+");
  descritor_fs.arquivo_host = fp;
  return &descritor_fs;
}

int procura_nome(cry_desc_t *desc, char *nome) {
  for (int i = 1; i < 256; ++i) {
    if(!strcmp(desc->descritores[i].nome, nome)) {
      return i;
    }
  }
  return 0;
}

int create_file(cry_desc_t * cry_desc, char * nome) {
  int i;
  for (i = 1; i < 256; i++) {
    if (cry_desc->descritores[i].nome[0] == 0) {
      break;
    }
  }
  strcpy(cry_desc->descritores[i].nome, nome);
  return i;
}

/** Abre um arquivo criptografado.
 *
 *
 * @param cry_desc o descritor do sistema de arquivos criptografado
 * @param nome o arquivo a ser aberto
 * @param acesso LEITURA, ESCRITA ou LEITURAESCRITA
 * @param chave deslocamento a ser usado nos acessos
 * @return índice do arquivo aberto, FALHA se não abrir
 */
indice_arquivo_t cry_open(cry_desc_t *cry_desc, char * nome,  int acesso, char deslocamento) {
  int indice_aberto;
  for (indice_aberto = 1; indice_aberto < 256; indice_aberto++) {
    if (cry_desc->abertos[indice_aberto].arquivo == NULL) {
      break;
    }
    if (!strcmp(cry_desc->abertos[indice_aberto].arquivo->nome, nome)) {
      return indice_aberto;
    }
  }

  int position = procura_nome(cry_desc, nome);
  if (position == 0) {
    if (acesso == LEITURA) {
      return FALHA;
    }
    position = create_file(cry_desc, nome);
  }
  cry_desc->abertos[indice_aberto].arquivo = &cry_desc->descritores[position];
  return indice_aberto;
}

/** Fecha um arquivo criptografado.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return SUCESSO OU FALHA
 */
int cry_close(indice_arquivo_t arquivo) {
  return FALHA;
}

/** Lê bytes de um arquivo criptografado aberto.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param tamanho número de bytes a serem lidos
 * @param buffer ponteiro para buffer onde serão armazenados os bytes lidos
 * @return número de bytes lidos
 */
uint32_t cry_read(indice_arquivo_t arquivo, uint32_t tamanho, char *buffer) {
  return FALHA;
}

/** Escreve bytes em um arquivo criptografado aberto.
 *
 * A escrita é sempre realizada no modo append, ou seja, no fim do arquivo.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param tamanho número de bytes a serem escritos
 * @param buffer ponteiro para conteúdo a ser escrito
 * @return SUCESSO ou FALHA
 */
int cry_write(indice_arquivo_t arquivo, uint32_t tamanho, char *buffer) {
  return FALHA;
}

/** Apaga um arquivo e o fecha.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return SUCESSO ou FALHA
 */

int cry_delete(indice_arquivo_t arquivo) {
  return FALHA;
}

/** Modifica a posição atual de leitura ou escrita do arquivo
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param seek deslocamento em relação ao início do arquivo
 * @return SUCESSO ou FALHA
 */
int cry_seek(indice_arquivo_t arquivo, uint32_t seek) {
  return FALHA;
}

/** Retorna o tempo em que o arquivo foi criado
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return tempo
 */
time_t cry_creation(indice_arquivo_t arquivo) {
  return FALHA;
}

/** Retorna o tempo em que o arquivo foi acessado
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return tempo
 */
time_t cry_accessed(indice_arquivo_t arquivo) {
  return FALHA;
}

/** Retorna o tempo em que o arquivo foi modificado
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return tempo
 */
time_t cry_last_modified(indice_arquivo_t arquivo) {
  return FALHA;
}
