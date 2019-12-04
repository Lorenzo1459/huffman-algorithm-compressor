#include "descompactador.h" 
#include "bitmap.h"

int main(int argc, char const *argv[]) {
    short int n;
    char lixo;        
    char c; // aux do ponteiro de arquivo
    bitmap bm = bitmapInit(1024);
    FILE* fp = fopen(argv[1],"rb"); //abrindo arquivo    
    if(fp != NULL){
        fread(&n,sizeof(short int),1,fp); // le o num de bytes de cabecalho
        printf("n bytes = %d\n", n);    
        fread(&lixo,sizeof(unsigned char),1,fp); // le o num de bits de lixo do cabecalho
        printf("lixo = %d\n", lixo);    

        for(int i = 0; i < n; i++){
            unsigned char c;
            fread(&c,sizeof(unsigned char),1,fp);
            for(int j = 7; j >= 0; j--){
                bitmapAppendLeastSignificantBit(&bm,(c>>j)&1);
            }                        
        }                
    }
    Arv* serializada = arv_criavazia();    
    int cont = 0; 
    char letra;    
    for(int i = 0; i < bitmapGetLength(bm); i++){
        unsigned char bit = bitmapGetBit(bm,i);
        // printf("index = %d - bit = %d\n",i, bit);
        if(bit == 1){
            i++;
            bitmap bm_aux = bitmapInit(9);
            //criar folha
            unsigned char vet[8], letra;
            for(int j = i + 7; j >= i; j--){
                vet[j - i] = (bitmapGetBit(bm,j) & 1);
                bitmapAppendLeastSignificantBit(&bm_aux,(bitmapGetBit(bm,j) & 1));
            }
            unsigned char aux = 0;
            for (int k = 0; k < 8; k++){
                aux = aux << 1;
                aux = aux | vet[k];
            }
            printf("%c", aux);
            // for(int k = 0; k < 8; k++){
            //     printf("%d", vet[k]);
            // }
            // letra = bitmapGetContents(bm_aux);
            // printf("letra = %c\n", letra);
            printf("\n");
            // fread(&letra,sizeof(unsigned char),1,fp);
            // printf("caractere = %d\n", letra);
            // printf("entrei\n");
            i+=7;

        }
        else if(bit == 0){  
            // cria nó arv          
            continue;
        }
        
        // printf("%d",bitmapGetBit(bm,i));
    }
    // printf("cont = %d\n",cont);
    // int i;
    // for (i=0; i< bitmapGetLength(bm); i++) {      
	//   printf("bit #%d = %0xh\n", i, bitmapGetBit(bm, i));
	// }
    // printf("%d\n",bitmapGetLength(bm));
    // printf("FIM DO DEBUG BITMAP\n");
    // printf("%d\n", n);
    
    
    return 0;
}