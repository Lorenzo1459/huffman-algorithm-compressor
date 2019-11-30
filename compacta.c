#include "compactador.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
  int n = 0;
  FILE* fp = fopen("bombom.txt","r"); //abrindo arquivo
  int VetorASC[256] = {0}; //criando e inicializando com 0's um vetor das posicoes ascii
  char c; // aux do ponteiro de arquivo

  if (fp != NULL) {
    while ((c = fgetc(fp)) != EOF) { // percorrendo o arquivo
      n++;
      printf("%c", c);
      unsigned char aux = c;
      VetorASC[aux]++; // aumentando o indice do caractere lido
    }//no final desse loop o VetorASC contem todos os pesos dos caracteres
  }
  else{
    printf("Erro na leitura do arquivo\n");
  }  
  TipoLista* ListaDeArvores = InicializaLista(); //criando lista para armazenar as arvores
  for (int i = 0; i < 256; i++) { // filtrando o loop para caracteres visiveis
    if (VetorASC[i] > 0) { // filtrando para caracteres com pelo menos 1 aparicao no texto
      Insere_lista(arv_cria((char)i,VetorASC[i],NULL,NULL),ListaDeArvores);
      // ^ criando uma arvore com o caractere e seu respectivo peso
      //E inserindo na lista de arvores
    }
   }
   printf("-------------ANTES DE HUFFMAN--------------\n");
  //ImprimeLista(ListaDeArvores); // debug pré huffma
  bubbleSort(retorna_inicio_lista(ListaDeArvores));
  //ImprimeLista(ListaDeArvores); // debug pré huffman
  // // printf("fim da imprime lista pré huffman\n");
  // // printf("-------------DEPOIS DE HUFFMAN-------------\n");
  Alg_HuffmanComp(ListaDeArvores);  //processo de aplicacao de huffman
  Arv* arv_otima = RetornaArvoreOtima(ListaDeArvores); // pega a primeira arvore da lista (depois de huffman a unica arvore da lista é a arvore otima)
  arv_imprime(arv_otima); // debug pós huffman (arvore otima)
  // printf("fim da imprime lista pós huffman\n");

  bitmap bm = bitmapInit(1024);
  //VetorASC = {0};
  int i = 1;
  char vetor[9];
  vetor[0]='1';

  arv_mapeia(arv_otima, &bm, i,vetor);

  arv_cabecalho(arv_otima, &bm);
    printf("DEBUG BITMAP\n");  
    // int i;
    for (i=0; i< bitmapGetLength(bm); i++) {      
		  printf("bit #%d = %0xh\n", i, bitmapGetBit(bm, i));
	  }
    printf("%d\n",bitmapGetLength(bm));
    printf("FIM DO DEBUG BITMAP\n");
    printf("%d\n", n);
  return 0;  
}
