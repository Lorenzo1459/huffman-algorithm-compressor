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
        fseek(fp,2,SEEK_SET);
        printf("n bytes = %d\n", n);    
        fread(&lixo,sizeof(unsigned char),1,fp); // le o num de bits de lixo do cabecalho        
        printf("lixo = %d\n", lixo);
        for(int i = 0; i < n; i++){ // le o cabecalho e armazena no bitmap bm        
            unsigned char c;
            fread(&c,sizeof(unsigned char),1,fp);            
            for(int j = 7; j >= 0; j--){
                printf("%d", (c>>j)&1);
                bitmapAppendLeastSignificantBit(&bm,(c>>j)&1);
            }
            printf("\n");
        }
        printf("tell = %ld\n", ftell(fp));

    int p = 0;
    Arv* reconst = reconstroi_arv(&bm, &p);
    arv_imprime(reconst); //debug

    free(bitmapGetContents(bm));
    bm = bitmapInit(1024); // reinicia o bitmap p/ leitura da codificacao    

    FILE* descompactado = fopen("descompactado","w");    
    char l;
    Arv* aux = reconst;   
    //bom esse bombom
    //bbb ooo mmm ee ss 
    while(!feof(fp)){   
        unsigned char c;
        fread(&c,sizeof(unsigned char),1,fp);
        for(int j = 7; j >= 0; j--){         
            printf("%d",(c>>j)&&1);
            if(((c>>j)&&1)==0){
                // printf("entrei no retorna esq\n");
                aux = retorna_esq(aux);
            }
            else if (((c>>j)&&1)==1){
                // printf("entrei no retorna dir\n");
                aux = retorna_dir(aux);
            }
            if(retorna_esq(aux) == NULL && retorna_dir(aux) == NULL){
                printf("entrei no retorna folha = ");
                l = retorna_caractere(aux);
                printf("%c\n", l);
                fwrite(&l,sizeof(unsigned char), 1, descompactado);
                
                aux = reconst;
            }
            // bitmapAppendLeastSignificantBit(&bm,(c>>j)&1);
        }        
        printf("\n");
    }
        printf("tell = %ld\n", ftell(fp));
    // escrever no arquivo    
    // fclose(descompactado);
    // int i;
    // for (i=0; i< bitmapGetLength(bm); i++) {      
	//   printf("bit #%d = %0xh\n", i, bitmapGetBit(bm, i));
	// }
    // printf("%d\n",bitmapGetLength(bm));
    // printf("FIM DO DEBUG BITMAP\n");
    // printf("%d\n", n);
    }
    
    return 0;
}