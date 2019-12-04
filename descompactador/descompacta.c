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

        for(int i = 0; i < n; i++){ // le o cabecalho e armazena no bitmap bm
            unsigned char c;
            fread(&c,sizeof(unsigned char),1,fp);
            for(int j = 7; j >= 0; j--){
                bitmapAppendLeastSignificantBit(&bm,(c>>j)&1);
            }                        
        }                
    }    
    int p = 0;
    Arv* reconst = reconstroi_arv(&bm, &p);
    // arv_imprime(reconst); //debug

    free(bitmapGetContents(bm));
    bm = bitmapInit(1024); // reinicia o bitmap p/ leitura da codificacao

    FILE* descompactado = fopen("descompactado","w");
    int z = 0;
    Arv* aux = reconst;
    // int cont = 0;
    while(!feof(fp)){        
        unsigned char c;
        fread(&c,sizeof(unsigned char),1,fp);
        for(int j = 7; j >= 0; j--){
            if(bitmapGetLength(bm) == bitmapGetMaxSize(bm)){
                // for(int t = 0; t < bitmapGetLength(bm); t++){
                //     aux = percorre(aux, ) // escrever no arquivo
                // }
                free(bitmapGetContents(bm));
                bm = bitmapInit(1024);
            }
            // bitmapAppendLeastSignificantBit(&bm,(c>>j)&1);
            // aux = percorre(aux,(c>>j)&&1);
            // if(eh_folha(aux)){
            //     unsigned char d = info(aux);
            //     printf("%c - ", d);
            //     cont++;
            // }
        }
        z++;
    }
    // printf("cont = %d\n", cont);
    // escrever no arquivo
    free(bitmapGetContents(bm));
    bm = bitmapInit(1024);
    // fclose(descompactado);
    // int i;
    // for (i=0; i< bitmapGetLength(bm); i++) {      
	//   printf("bit #%d = %0xh\n", i, bitmapGetBit(bm, i));
	// }
    // printf("%d\n",bitmapGetLength(bm));
    // printf("FIM DO DEBUG BITMAP\n");
    // printf("%d\n", n);
    
    
    return 0;
}