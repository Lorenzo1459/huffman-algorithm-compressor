#include "compactador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[]) {
  FILE* fp = fopen(argv[1],"r"); //abrindo arquivo
  int VetorASC[150] = {0}; //criando e inicializando com 0's um vetor das posicoes ascii
  char c; // aux do ponteiro de arquivo

  if (fp != NULL) {
    while ((c = fgetc(fp)) != EOF) { // percorrendo o arquivo
      printf("%c", c);
      int aux = c;
      VetorASC[aux]++; // aumentando o indice do caractere lido
    }//no final desse loop o VetorASC contem todos os pesos dos caracteres
  }
  else{
    printf("Erro na leitura do arquivo\n");
  }
  TipoLista* ListaDeArvores = InicializaLista(); //criando lista para armazenar as arvores
  for (int i = 32; i < 127; i++) { // filtrando o loop para caracteres visiveis
    if (VetorASC[i] > 0) { // filtrando para caracteres com pelo menos 1 aparicao no texto
      Insere_lista(arv_cria((char)i,VetorASC[i],NULL,NULL),ListaDeArvores);
      // ^ criando uma arvore com o caractere e seu respectivo peso
      //E inserindo na lista de arvores
    }
  }
  printf("-------------ANTES DE HUFFMAN--------------\n");
  ImprimeLista(ListaDeArvores); // debug pré huffman
  printf("fim da imprime lista pré huffman\n");
  printf("-------------DEPOIS DE HUFFMAN-------------\n");
  Alg_HuffmanComp(ListaDeArvores);  //processo de aplicacao de huffman
  ImprimeLista(ListaDeArvores); // debug pós huffman (reparar na ausencia dos "---"), indicando que a lista agora só possui uma árvore (arvore otima)
  printf("fim da imprime lista pós huffman\n"); 
  return 0;
}
