#ifndef DESCOMPACTADOR_H_
#define DESCOMPACTADOR_H_
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct arv Arv;
typedef struct celula TCelula;
typedef struct lista TipoLista;

//FUNCOES DE LISTA
TipoLista* InicializaLista();
void Insere_lista (Arv* arvore, TipoLista* lista);
Arv* Retira_lista (TipoLista* lista, Arv* arv);
void ImprimeLista (TipoLista* lista);
TipoLista* LiberaLista (TipoLista* lista);
Arv* RetornaArvoreOtima(TipoLista* lista);
TCelula* retorna_inicio_lista(TipoLista* lista);

void bubbleSort(TCelula* comeco);
void troca(TCelula *a, TCelula *b);
//FIM DAS FUNCOES DE LISTA

//FUNCOES DE ARVORE
Arv* arv_criavazia (void);
Arv* arv_cria (char c, int peso, Arv* e, Arv* d);
Arv * franco_rec (bitmap*  bm, int * i);
Arv* arv_libera (Arv* a);
int arv_vazia (Arv* a);
int arv_pertence (Arv* a, char c);
int eh_folha(Arv* a);
void arv_mapeia(Arv* a, int i, char* posicao, char** asc);
void arv_cabecalho(Arv* a, bitmap* bm, FILE* saida);
void arv_imprime (Arv* a);
char info (Arv* a);
int altura (Arv* a);

Arv* reconstroi_arv(bitmap*  bm, Arv* a);
//FIM DAS FUNCOES DE ARVORE


#endif // DESCOMPACTADOR_H_