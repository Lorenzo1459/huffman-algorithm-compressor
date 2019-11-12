#ifndef COMPACTADOR_H_
#define COMPACTADOR_H_

typedef struct arv Arv;

typedef struct lista TipoLista;

//FUNCOES DE LISTA
TipoLista* InicializaLista();
void Insere_lista (Arv* arvore, TipoLista* lista);
Arv* Retira_lista (TipoLista* lista, Arv* arv);
void ImprimeLista (TipoLista* lista);
TipoLista* LiberaLista (TipoLista* lista);
//FIM DAS FUNCOES DE LISTA

//FUNCOES DE ARVORE
Arv* arv_criavazia (void);
Arv* arv_cria (char c, int peso, Arv* e, Arv* d);
Arv* arv_libera (Arv* a);
int arv_vazia (Arv* a);
int arv_pertence (Arv* a, char c);
void arv_imprime (Arv* a);
char info (Arv* a);
int altura (Arv* a);
//FIM DAS FUNCOES DE ARVORE

//FUNCOES ESPECIFICAS
TipoLista* Alg_HuffmanComp(TipoLista* lista);
TipoLista* Alg_Huffman1rep(TipoLista* lista);
//FIM DAS FUNCOES ESPECIFICAS



#endif // COMPACTADOR_H_
