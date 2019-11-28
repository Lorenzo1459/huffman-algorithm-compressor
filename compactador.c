#include "compactador.h"
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

int arv_mapeia(Arv* a, bitmap* bm){  
  if (!arv_vazia(a)) {
    if(a->esq != NULL){
      bitmapAppendLeastSignificantBit(bm, 0);      
      arv_mapeia(a->esq, bm);
    }
    if(a->dir != NULL){      
      bitmapAppendLeastSignificantBit(bm, 1);
      arv_mapeia(a->dir, bm);
    }
    if(eh_folha(a)){                  
      for(int i = 7; i >= 0; i--){
        bitmapAppendLeastSignificantBit(bm, (a->c >> i) & 1); // dá append em cada bit do caractere de dado nó no bitmap
      }
    }    
  }  
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
    a->arvore_lista->peso = b->arvore_lista->peso;
    b->arvore_lista->peso = temp;
} 
//FIM DAS FUNCOES DE LISTA

//FUNCOES ESPECIFICAS
TipoLista* Alg_HuffmanComp(TipoLista* lista){ // usando huffman recursivamente ate a lista possuir 1 elemento
  lista = Alg_Huffman1rep(lista);
  if (lista->inicio->prox == NULL) { //(condicao de parada) se nao há segundo elemento, só sobrou a arvore ótima de huffman
    return lista;
  }
  else{ // permutaçao da recursao
    Alg_HuffmanComp(lista);
  }
}
TipoLista* Alg_Huffman1rep(TipoLista* lista){
  Arv* t1 = lista->inicio->arvore_lista; //primeiro elemento da lista
  Arv* t2 = lista->inicio->prox->arvore_lista; // segundo elemento da lista
  int novopeso = t1->peso + t2->peso; // peso da nova arvore
  Arv* tr = arv_cria('z',novopeso,t1,t2); // nova arvore com os elementos acima
  Retira_lista(lista, t2); // remove 2 elemento
  Retira_lista(lista, t1); // remove 1 elemento
  Insere_lista(tr,lista); // adiciona nova arvore ao final da lista
}
//FIM DAS FUNCOES ESPECIFICAS
