#include "compactador.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[]) {
  int n = 0;
  FILE* fp = fopen(argv[1],"r"); //abrindo arquivo
  int VetorASC[256] = {0}; //criando e inicializando com 0's um vetor das posicoes ascii
  char c; // aux do ponteiro de arquivo

  if (fp != NULL) {
    while ((c = fgetc(fp)) != EOF) { // percorrendo o arquivo
      n++;
      // printf("%c", c);
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
  // printf("-------------ANTES DE HUFFMAN--------------\n");
  bubbleSort(retorna_inicio_lista(ListaDeArvores));
  // ImprimeLista(ListaDeArvores); // debug pré huffman
  // printf("fim da imprime lista pré huffman\n");
  // printf("-------------DEPOIS DE HUFFMAN-------------\n");
  Alg_HuffmanComp(ListaDeArvores);  //processo de aplicacao de huffman
  Arv* arv_otima = RetornaArvoreOtima(ListaDeArvores); // pega a primeira arvore da lista (depois de huffman a unica arvore da lista é a arvore otima)
  // arv_imprime(arv_otima); // debug pós huffman (arvore otima)
  // printf("fim da imprime lista pós huffman\n");

  bitmap bm = bitmapInit(1024);  

  int j = 0;
  char vetor[9];  

  char** VetASC = (char**)malloc(256 * sizeof(char*)); // NAO CONFUNDIR COM VetorASC USADO ANTERIORMENTE
  for(int k = 0; k < 256; k++)  
   VetASC[k] = (char*)malloc(9 * sizeof(char));

  arv_mapeia(arv_otima, j,vetor, VetASC); // Mapeia o caminho necessario até cada caractere na arvore otima de huffman
  //e os salva em seu índice ASCII na matriz declarada acima, para termos acesso mais rapido na codificacao posteriormente


  // printf("inicio debug\n");

  // for(int k = 0; k < 256; k++){         // debug do caminho até cada caractere
  //   printf("%c - ", k);   
  //   printf("strlen %ld - ", strlen(VetASC[k]));
  //   puts(VetASC[k]);
  // }

  rewind(fp); //resetando ponteiro do arquivo para o inicio
  // n = 0; 
  FILE* compactado = fopen("arquivo.comp","wb");

  arv_cabecalho(arv_otima, &bm, compactado);  // prepara a serializacao da arvore, que sera escrita no cabecalho
  printf("length ========= %d\n",bitmapGetLength(bm));

  int byteaux;
  int lixo = (bitmapGetLength(bm)%8); // bits restantes do ultimo byte  
  if(lixo == 0)
    byteaux = 0;  
  else{
    byteaux = 1;      
  }
  lixo = 8 - lixo; // agora sim eh lixo  
  int Tamanho_serializacao = (bitmapGetLength(bm)/8) + byteaux;

  printf("debugwen = %d\nlixo = %d\n", Tamanho_serializacao, lixo);
  fwrite(&Tamanho_serializacao,sizeof(short int),1,compactado); // escreve quantos dos proximos bits do arquivo sao cabecalho  
  fwrite(&lixo, sizeof(unsigned char), 1, compactado); // escreve quantos bits de lixo o ultimo byte da serializacao possui
  fwrite(bitmapGetContents(bm),sizeof(unsigned char),Tamanho_serializacao,compactado);  // escreve a serializacao da arvore no arquivo binario  
  
  free(bitmapGetContents(bm)); // libera o bitmap                 
  bm = bitmapInit(1024); // reinicia o bitmap para continuar a codificacao
  while ((c = fgetc(fp)) != EOF) { // percorrendo o arquivo a ser compactado
      unsigned char aux = c;      
      for(int i = 0; i <= strlen(VetASC[aux]) - 1; i++){
        if(bitmapGetLength(bm) == bitmapGetMaxSize(bm)){
          //ESVAZIAR BITMAP
          fwrite(bitmapGetContents(bm),sizeof(unsigned char),(bitmapGetLength(bm)+7)/8,compactado); // transcreve o bitmap no arquivo binario "compactado"          
          free(bitmapGetContents(bm)); // libera o bitmap                 
          bm = bitmapInit(1024); // reinicia o bitmap para continuar a codificacao
        }        
        printf("%s - ",VetASC[aux]);
        printf("%c - ", VetASC[aux][i]);        
        printf("debug ------- %c\n", ((unsigned char)VetASC[aux][i]));
        bitmapAppendLeastSignificantBit(&bm, ((unsigned char)VetASC[aux][i])); // dá append em cada bit do caminho ate alcançar o caractere lido (armazenado em VetASC)
      }
      printf("---------------------------------------\n");
    }    
    fwrite(bitmapGetContents(bm),sizeof(unsigned char),(bitmapGetLength(bm)+7)/8,compactado); // escrevo o que sobrou do bitmap
    free(bitmapGetContents(bm));  // libera o bitmap
    bm = bitmapInit(1024);
    fclose(fp);  
  

    // printf("DEBUG BITMAP\n");  
    // int i;
    // for (i=0; i< bitmapGetLength(bm); i++) {      
		//   printf("bit #%d = %0xh\n", i, bitmapGetBit(bm, i));
	  // }
    printf("%d\n",bitmapGetLength(bm));
    // printf("FIM DO DEBUG BITMAP\n");
    // printf("%d\n", n);
  return 0;
}
