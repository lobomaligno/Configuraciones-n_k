//
//  SRHtraducir.cpp
//  
//
//  Created by Víctor Andrés Hernández Patiño on 06/01/20.
//
#include <functional>
#include <fstream>
using namespace std;
int main(int argc, const char * argv[]) {
    FILE *finput;
    finput = fopen(argv[1], "r");
    int nvertices, tlinea, tam, n;
    if(argv[2]==NULL){
        printf("Traducir: Se necesita un segundo argumento para el nombre del archivo de salida\n");
        system("pause");
        exit(23);
    }
    else{
        printf("Traducir: El archivo de salida es %s\n", argv[2]);
    }
    if(finput==NULL){
        printf("Traducir: No esta el archivo %s\n", argv[1]);
        system("pause");
        exit(23);
    }
    fscanf(finput,"%d",&nvertices);
    fscanf(finput,"%d",&tlinea);
    fscanf(finput,"%d",&tam);
    fscanf(finput,"%d",&n);
    ofstream myfile;
    myfile.open (argv[2]);
    int i, j, k, l, m;
    unsigned long hash;
    for(i=0; i<n; i++){
        for (j=0, k=1; j<tlinea; ++j) {
            myfile<<"(1";
            for (l=1; l<tlinea; ++l){
                myfile<<","<<++k;
            }
            myfile<<")";
        }
        for(j=0; j<tam; j++){
            myfile<<"(";
            fscanf(finput, "%d", &k);
            for (l=1, m=0; m<tlinea || l<nvertices; l++) {
                if(k&(1<<l)){
                    myfile<<(l+1);
                    m++;
                    if(m<tlinea){
                        myfile<<",";
                    }
                }
            }
            myfile<<")";
        }
        fscanf(finput,"%lu",&hash);
        myfile<<"\n";
    }
}
