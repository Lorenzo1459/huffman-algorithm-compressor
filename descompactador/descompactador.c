#include "descompactador.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct arv{
 char c;
 int peso;
 Arv* esq;
 Arv* dir;
};

struct celula{
  Arv* arvore_lista;
  TCelula* prox;
};

struct lista{
  TCelula* inicio;
  TCelula* fim;
};

//FUNCOES DE ARVORE
Arv* arv_criavazia (void){
 return NULL;
}

Arv* arv_cria (char c, int peso, Arv* e, Arv* d){
  Arv* a = (Arv*)malloc(sizeof(Arv));
  a->c = c;
  a->peso = peso;
  a->esq = e;
  a->dir = d;
  return a;
}

int arv_vazia (Arv* a){
  return (a == NULL);
}

Arv* arv_libera (Arv* a){
  if (!arv_vazia(a)) {
    arv_libera(a->esq);
    arv_libera(a->dir);
    free(a);
  }
  return NULL;
}

int arv_pertence (Arv* a, char c){
  if (arv_vazia(a)) {
    return 0;
  }
  else{
    if (c == a->c) {
      return 1;
    }
    else{
      return arv_pertence(a->esq, c) || arv_pertence(a->dir, c);
    }
  }
}

int eh_folha(Arv* a){
  if(arv_vazia(a->esq) && arv_vazia(a->dir)){
    return 1;
  }
  else{
    return 0;
  }
}

void arv_mapeia(Arv* a, int i, char* posicao, char** asc){ // i sempre será 1 na chamada  
  //botar 1 no começo depois * por 10 se for pra esquerda e por 10 + 1 pra direita
  if (!arv_vazia(a)) {
    if(a->esq != NULL){ //coloca 0 se for pra esquerda      
      posicao[i]='0';
      i++;      
      arv_mapeia(a->esq, i, posicao, asc);
      i--;  //pra quando estiver voltando n guardar lixo
    }
    
    if(a->dir != NULL){ //coloca 1 se for pra direita      
      posicao[i]='1';
      i++;      
      arv_mapeia(a->dir, i,posicao, asc); 
      i--;
    }
    
    if(eh_folha(a)){      
      posicao[i]='\0';
      // printf("%c -> ",a -> c);      
      // printf("%s\n",posicao);
      unsigned char aux = a->c;                  
      // asc[aux] = (char*)malloc(sizeof(posicao));
      strcpy(asc[aux],posicao);
    }
  }
  return ;  
}


void arv_cabecalho(Arv* a, bitmap* bm, FILE* saida){
  if(!arv_vazia(a)){
    if(eh_folha(a)){      
      bitmapAppendLeastSignificantBit(bm, 1);      
      for(int i = 7; i >= 0; i--){
        bitmapAppendLeastSignificantBit(bm, (a->c >> i) & 1); // dá append em cada bit do caractere de dado nó no bitmap
      }
    }
    else{
      bitmapAppendLeastSignificantBit(bm, 0);
      arv_cabecalho(a->esq, bm, saida);
      arv_cabecalho(a->dir, bm, saida);
    }
  }
}

Arv* reconstroi_arv (bitmap*  bm, int* i)
{   
  if (*i >= bitmapGetLength(*bm)){
    printf("erro\n");
    return NULL;
  }  
  Arv* arvaux;
  unsigned char bit = bitmapGetBit(*bm,*i);
  if (bit == 1)
  {  
    (*i)++;
    unsigned char vet[8], letra; // vetor p armazenar o codigo do caractere
    for(int j = *i + 7; j >= *i; j--){
        vet[j - *i] = (bitmapGetBit(*bm,j) & 1);                
    }
    unsigned char aux = 0;
    for (int k = 0; k < 8; k++){ // transformar o vetor em um caractere com o codigo do vetor
        aux = aux << 1;
        aux = aux | vet[k];
    }
    arvaux = arv_cria (aux, 1, NULL, NULL);    
    (*i) += 7;
    return arvaux;
  }
  else
  {
    (*i)++;
    Arv * aux_esq = reconstroi_arv (bm, i);
    (*i)++;
    Arv * aux_dir = reconstroi_arv (bm, i);
    arvaux = arv_cria ('z', 0, aux_esq, aux_dir);
  }
}

Arv* percorre(Arv* a, int i){    
  if(a != NULL){    
    if(i == 0)
        return a->esq;

    else if(i == 1)
        return a->dir;
  }

    return NULL;
}

void arv_imprime (Arv* a){
  if (!arv_vazia(a)) {
    printf("info = %c\n", a->c);
    printf("peso = %d\n", a->peso);
    arv_imprime(a->esq);
    arv_imprime(a->dir);
  }
}

char info (Arv* a){
  return a->c;
}

static int maiorDeDois(int a, int b){
  if (a > b) {
    return a;
  }
  else{
    return b;
  }
}

int altura (Arv* a){
  int h;
  if (arv_vazia(a)) {
    return -1;
  }
  else{
    return 1 + maiorDeDois(altura(a->esq), altura(a->dir));
  }
}
//FIM DAS FUNCOES DE ARVORE

//FUNCOES DE LISTA
TipoLista* InicializaLista(){
  TipoLista* lst = (TipoLista*)malloc(sizeof(TipoLista));
  lst->inicio = lst->fim = NULL;
  return lst;
}

void Insere_lista (Arv* arvore, TipoLista* lista){
  TCelula* novo = (TCelula*)malloc(sizeof(TCelula));
  novo->arvore_lista = arvore;
  novo->prox = NULL;
  if (lista->inicio == NULL) {
      lista->inicio = novo;
      lista->fim = novo;
  }
  else{
    lista->fim->prox = novo;
    lista->fim = novo;
  }
} // insere no final

Arv* Retira_lista (TipoLista* lista, Arv* arv){
  TCelula* aux = lista->inicio;
  TCelula* ant = NULL;
  while (aux!=NULL && aux->arvore_lista != arv) {
    ant = aux;
    aux = aux->prox;
  }

  if (aux == NULL) { //Lista chegou ao fim
    // printf("------------------------ELEMENTO NAO ENCONTRADO------------------------\n");
    return NULL;
  }

  if (ant == NULL) { //Caso da remoçao do primeiro
    Arv* retirado = aux->arvore_lista;
    // printf(">>>>>%c\n", retirado->c);
    lista->inicio = aux->prox;
    // printf("------------------------REMOCAO PRIMEIRO ELEMENTO------------------------\n");
    return retirado;
  }

  if(aux->arvore_lista == arv) { //Caso da remocao comum
    Arv* retirado = aux->arvore_lista;
    // printf(">>>>>%c\n", retirado->c);
    ant->prox = aux->prox;
    // printf("------------------------REMOCAO COMUM------------------------\n");
    return retirado;
  }

}

void ImprimeLista (TipoLista* lista){
  TCelula* aux = lista->inicio;
  while (aux != NULL) {
    arv_imprime(aux->arvore_lista);
    printf("---------------------------\n");
    aux = aux->prox;
  }
}

TipoLista* LiberaLista (TipoLista* lista){
  TCelula* aux = lista->inicio;
  while (aux != NULL) {
    TCelula* aux2 = aux->prox;
    free(aux->arvore_lista);
    free(aux);
    aux = aux2;
  }
  free(lista);
  return NULL;
}

Arv* RetornaArvoreOtima(TipoLista* lista){
  return lista->inicio->arvore_lista;
}

TCelula* retorna_inicio_lista(TipoLista* lista){
  return lista->inicio;
}

void bubbleSort(TCelula* comeco) 
{ 
    int trocou, i; 
    TCelula *aux; 
    TCelula *aux2 = NULL; 
      
    if (comeco == NULL) 
        return;  
    do
    { 
        trocou = 0; 
        aux = comeco; 
  
        while (aux->prox != aux2) 
        { 
            if (aux->arvore_lista->peso > aux->prox->arvore_lista->peso) 
            {  
                troca(aux, aux->prox); 
                trocou = 1; 
            } 
            aux = aux->prox; 
        } 
        aux2 = aux; 
    } 
    while (trocou); 
} 
  
void troca(TCelula *a, TCelula *b) 
{ 
    int temp = a->arvore_lista->peso; 
    int temp2 = a->arvore_lista->c;
    a->arvore_lista->peso = b->arvore_lista->peso;
    a->arvore_lista->c = b->arvore_lista->c;
    b->arvore_lista->peso = temp;
    b->arvore_lista->c = temp2;
} 
