#include "descompactador.h" 
#include "bitmap.h"

int main(int argc, char const *argv[]) {
    short int n;
    char lixo;        
    char c; // aux do ponteiro de arquivo
    bitmap bm = bitmapInit(4096);
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
                // printf("%d", (c>>j)&1)
                bitmapAppendLeastSignificantBit(&bm,(c>>j)&1);
            }
        }
        printf("tell = %ld\n", ftell(fp));
        printf("\n");

    int p = 0;
    Arv* reconst = reconstroi_arv(&bm, &p);
    // arv_imprime(reconst); //debug

    free(bitmapGetContents(bm));
    bm = bitmapInit(4096); // reinicia o bitmap p/ leitura da codificacao    

    FILE* descompactado = fopen("descompactado","w");    
    char l;
    Arv* aux = reconst;    
    int cont = 0;
    printf("tell = %ld\n", ftell(fp));
    while(!feof(fp)){   
        unsigned char c;
        fread(&c,sizeof(unsigned char),1,fp);
        for(int j = 7; j >= 0; j--){         
            // printf("%d",(c>>j)&1);
            cont++;
            if(((c>>j)&1)==0){
                // printf("entrei no retorna esq\n");
                aux = retorna_esq(aux);
            }
            else if (((c>>j)&1)==1){
                // printf("entrei no retorna dir\n");
                aux = retorna_dir(aux);
            }
            if(eh_folha(aux)){
                l = retorna_caractere(aux);
                // printf("entrei no retorna folha = %c, ", l);                
                fwrite(&l,sizeof(unsigned char), 1, descompactado);
                // printf("pos = %ld\n", ftell(fp));
                aux = reconst;                
            }
            // bitmapAppendLeastSignificantBit(&bm,(c>>j)&1);
        }        
        // printf("\n");
    }
    printf("\ncont = %d\n", cont);
        printf("tell = %ld\n", ftell(fp));
    // escrever no arquivo
    fclose(fp);
    arv_libera(reconst);
    free(bitmapGetContents(bm));    
    fclose(descompactado);
    
    }
    
    return 0;
}