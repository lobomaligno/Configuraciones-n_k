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
    /*
    FILE *finput;
    finput = fopen(argv[1], "r");
     */
    ifstream finput (argv[1], ios::in|ios::binary);
    int nvertices, tlinea, tam;
    unsigned long n;
    if(argv[2]==NULL){
        printf("Traducir: Se necesita un segundo argumento para el nombre del archivo de salida\n");
        system("pause");
        exit(23);
    }
    else{
        printf("Traducir: El archivo de salida es %s\n", argv[2]);
    }
    if(!finput.is_open()){
        printf("Traducir: No esta el archivo %s\n", argv[1]);
        system("pause");
        exit(23);
    }
    /*
    fscanf(finput,"%d",&nvertices);
    fscanf(finput,"%d",&tlinea);
    fscanf(finput,"%d",&tam);
    fscanf(finput,"%d",&n);
     */
    finput.read((char*)&nvertices, sizeof(int));
    finput.read((char*)&tlinea, sizeof(int));
    finput.read((char*)&tam, sizeof(int));
    finput.read((char*)&n, sizeof(long));
    printf("nvertices=%d\ntlinea=%d\ntam=%d\nn=%lu\n", nvertices, tlinea, tam, n);
    ofstream myfile;
    myfile.open (argv[2]);
    printf("Archivo creado\n");
    int i, j, k, l, m;
    unsigned long hash;
    for(i=0; i<n; i++){
	//printf("i=%d\n", i);
        for (j=0, k=1; j<tlinea; ++j) {
            myfile<<"(1";
            for (l=1; l<tlinea; ++l){
                myfile<<","<<++k;
            }
            myfile<<")";
        }
	//printf("Primeros puntos escritos\n");
        for(j=0; j<tam; j++){
	    //printf("j=%d\n", j);
            myfile<<"(";
            finput.read((char*)&k, sizeof(int));
	    //printf("k=%d\n", k);
            for (l=1, m=0; l<nvertices; l++) {
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
	//printf("configuracion guardada\n");
	finput.read((char*)&hash, sizeof(long));
	myfile<<"hash="<<hash;
        myfile<<"\n";
    }
}
