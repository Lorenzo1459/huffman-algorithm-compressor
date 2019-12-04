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
    Arv* reconst = franco_rec(&bm, 0);
    arv_imprime(reconst);
    // int i;
    // for (i=0; i< bitmapGetLength(bm); i++) {      
	//   printf("bit #%d = %0xh\n", i, bitmapGetBit(bm, i));
	// }
    // printf("%d\n",bitmapGetLength(bm));
    // printf("FIM DO DEBUG BITMAP\n");
    // printf("%d\n", n);
    
    
    return 0;
}