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
 
#define MIN(a,b) (((a)<(b))?(a):(b))

const int BLOCOS_MIN = (sizeof(descritor_t) * 256 / 4096) + 1;

cry_desc_t descritor_fs;

typedef struct bloco {
  struct bloco *next;
  int indice;
  indice_arquivo_t dono;
} bloco_t;

bloco_t blocost[3000000000];
int num_blocos;

int bloco_inicial[256];

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
  
  num_blocos = blocos;
  int i;
  for(i = 19; i < num_blocos - 1; i++) {
    blocost[i].next = &blocost[i+1];
    blocost[i].indice = i;
    blocost[i].dono = -1;
  }
  blocost[i].next = NULL;
  blocost[i].indice = i;
  blocost[i].dono = -1;
  
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
  for (int i = 0; i < 256; ++i) {
    if(!strcmp(desc->descritores[i].nome, nome)) {
      return i;
    }
  }
  return 0;
}

int procura_bloco_livre() {
  for(int i = 0; i < num_blocos; i++) {
    if (blocost[i].dono == -1) {
      return i;
    }
  }
  return num_blocos;
}

int create_file(cry_desc_t * cry_desc, char * nome) {
  int i;
  for (i = 0; i < 256; i++) {
    if (cry_desc->descritores[i].nome[0] == 0) {
      break;
    }
  }
  strcpy(cry_desc->descritores[i].nome, nome);
  cry_desc->descritores[i].tamanho = 0;
  time(&cry_desc->descritores[i].criacao);
  int bloco = procura_bloco_livre();
  bloco_inicial[i] = bloco;
  blocost[bloco].dono = i;
  blocost[bloco].next = NULL;
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
  for (indice_aberto = 0; indice_aberto < 256; indice_aberto++) {
    if (cry_desc->abertos[indice_aberto].arquivo == NULL) {
      break;
    }
    if (!strcmp(cry_desc->abertos[indice_aberto].arquivo->nome, nome)) {
      return indice_aberto + 1;
    }
  }

  int position = procura_nome(cry_desc, nome);
  if (position == 0) {
    if (acesso == LEITURA) {
      return FALHA;
    }
    position = create_file(cry_desc, nome);
  }
  
  time(&cry_desc->descritores[position].ultimo_acesso);
  cry_desc->abertos[indice_aberto].arquivo = &cry_desc->descritores[position];
  cry_desc->abertos[indice_aberto].acesso = acesso;
  cry_desc->abertos[indice_aberto].posicao = cry_desc->abertos[indice_aberto].arquivo->tamanho;
  return indice_aberto + 1;
}

/** Fecha um arquivo criptografado.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return SUCESSO OU FALHA
 */
int cry_close(indice_arquivo_t arquivo) {
  int indice = arquivo - 1;
  if (descritor_fs.abertos[indice].arquivo == NULL) {
    return FALHA;
  }
  
  descritor_fs.abertos[indice].arquivo = NULL;
  return SUCESSO;
}

int bloco_atual(arquivo_aberto_t* aberto) {
  int i;
  for (i = 0; i < 256; i++) {
    if (aberto->arquivo == &descritor_fs.descritores[i]) {
      break;
    }
  }
  int bloco_atual = bloco_inicial[i];
  bloco_atual += aberto->posicao / BLOCO;
  return bloco_atual;
}

/** Lê bytes de um arquivo criptografado aberto.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param tamanho número de bytes a serem lidos
 * @param buffer ponteiro para buffer onde serão armazenados os bytes lidos
 * @return número de bytes lidos
 */
uint32_t cry_read(indice_arquivo_t arquivo, uint32_t tamanho, char *buffer) {
  int indice = arquivo - 1;
  if (descritor_fs.abertos[indice].arquivo == NULL) {
    printf("Nao existe\n");
    return FALHA;
  }
  
  if (descritor_fs.abertos[indice].acesso == ESCRITA) {
    printf("Nao pode ler\n");
    return FALHA;
  }
  
  arquivo_aberto_t* aberto = &descritor_fs.abertos[indice];
  
  int a_ler = tamanho;
  int atual = bloco_atual(aberto);
  int posicao_no_bloco = aberto->posicao % BLOCO;
  
  printf("Bloco atual = %d, posicao no bloco = %d\n", atual, posicao_no_bloco);

  fseek(descritor_fs.arquivo_host, atual * 4096 + posicao_no_bloco, SEEK_SET);
  int lidos = fread(buffer, sizeof(char), MIN(a_ler, BLOCO - posicao_no_bloco), descritor_fs.arquivo_host);
  a_ler -= MIN(a_ler, BLOCO - posicao_no_bloco);
  
  while (a_ler > 0) {
      blocost[atual] = *blocost[atual].next;
      atual = blocost[atual].indice;
      fseek(descritor_fs.arquivo_host, atual * BLOCO, SEEK_SET);
      lidos += fread(buffer + lidos, sizeof(char), MIN(a_ler, BLOCO), descritor_fs.arquivo_host);
      a_ler -= BLOCO;
  }
  
  return lidos;
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
  int indice = arquivo - 1;
  if (descritor_fs.abertos[indice].arquivo == NULL) {
    return FALHA;
  }
  
  if (descritor_fs.abertos[indice].acesso == LEITURA) {
    return FALHA;
  }
  
  arquivo_aberto_t* aberto = &descritor_fs.abertos[indice];
  descritor_t* desc = descritor_fs.abertos[indice].arquivo;
  
  int posicao_no_bloco = desc->tamanho % BLOCO;
  // if desc->tamanho == aberto->posicao (maybe)
  // aberto->posicao += tamanho;
  desc->tamanho += tamanho;
  
  int a_escrever = tamanho;
  int atual = bloco_atual(aberto);
  fseek(descritor_fs.arquivo_host, atual * 4096 + posicao_no_bloco, SEEK_SET);
  fwrite(buffer, sizeof(char), MIN(a_escrever, BLOCO - posicao_no_bloco), descritor_fs.arquivo_host);
  a_escrever -= MIN(a_escrever, BLOCO - posicao_no_bloco);
  
  while (a_escrever > 0) {
      int bloco_livre = procura_bloco_livre();
      blocost[atual].next = &blocost[bloco_livre];
      blocost[bloco_livre].next = NULL;
      blocost[bloco_livre].dono = blocost[atual].dono;
      atual = bloco_livre;
      fseek(descritor_fs.arquivo_host, atual * BLOCO, SEEK_SET);
      fwrite(buffer, sizeof(char), MIN(a_escrever, BLOCO), descritor_fs.arquivo_host);
      a_escrever -= BLOCO;
  }
  
  // TODO: Checagem de espaço disponível
  // int disponivel = blocos_livres()*4096 + (tamanho - posicao%4096);
  // tamanho - posicao + blocos_livres()*4096;
  // for ()
  time(&desc->modificacao);
  return SUCESSO;
}

int ultimo_bloco_livre() {
  for (int i = 0; i < num_blocos; i++) {
    if(blocost[i].next == NULL && blocost[i].dono == -1) {
      return i;
    }
  }
  return -1;
}

/** Apaga um arquivo e o fecha.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return SUCESSO ou FALHA
 */

int cry_delete(indice_arquivo_t arquivo) {
  int indice = arquivo - 1;
  if (descritor_fs.abertos[indice].arquivo == NULL) {
    return FALHA;
  }
  
  descritor_t* desc = descritor_fs.abertos[indice].arquivo;
  desc->nome[0] = 0;
  desc->criacao = 0;
  desc->modificacao = 0;
  desc->ultimo_acesso = 0;
  desc->tamanho = 0;
  
  int livre = ultimo_bloco_livre();
  blocost[livre].next = &blocost[bloco_inicial[indice]];
  int atual = bloco_inicial[indice];
  do {
    blocost[atual].dono = -1;
  } while (blocost[atual++].next != NULL);
  
  descritor_fs.abertos[indice].arquivo = NULL;
  return SUCESSO;
}

/** Modifica a posição atual de leitura ou escrita do arquivo
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param seek deslocamento em relação ao início do arquivo
 * @return SUCESSO ou FALHA
 */
int cry_seek(indice_arquivo_t arquivo, uint32_t seek) {
  int indice = arquivo - 1;
  if (descritor_fs.abertos[indice].arquivo == NULL) {
    return FALHA;
  }
  
  if (seek > descritor_fs.abertos[indice].arquivo->tamanho) {
    return FALHA;
  }
  
  descritor_fs.abertos[indice].posicao = seek;
  return SUCESSO;
}

/** Retorna o tempo em que o arquivo foi criado
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return tempo
 */
time_t cry_creation(indice_arquivo_t arquivo) {
  int indice = arquivo - 1;
  if (descritor_fs.abertos[indice].arquivo == NULL) {
    return FALHA;
  }
  
  descritor_t* desc = descritor_fs.abertos[indice].arquivo;
  return desc->criacao;
}

/** Retorna o tempo em que o arquivo foi acessado
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return tempo
 */
time_t cry_accessed(indice_arquivo_t arquivo) {
  int indice = arquivo - 1;
  if (descritor_fs.abertos[indice].arquivo == NULL) {
    return FALHA;
  }
  
  descritor_t* desc = descritor_fs.abertos[indice].arquivo;
  return desc->ultimo_acesso;
}

/** Retorna o tempo em que o arquivo foi modificado
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return tempo
 */
time_t cry_last_modified(indice_arquivo_t arquivo) {
  int indice = arquivo - 1;
  if (descritor_fs.abertos[indice].arquivo == NULL) {
    return FALHA;
  }
  
  descritor_t* desc = descritor_fs.abertos[indice].arquivo;
  return desc->modificacao;
}
