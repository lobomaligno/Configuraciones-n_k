#define ENCONTRADO 1
#define NO_ENCONTRADO 0
#define VALIDO 0
#define INVALIDO 1
#define MENOR -1
#define IGUAL 0
#define MAYOR 1
#include "nauty.h"
#include "profile_time.h"
#include <stdio.h>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "SRHfunciones.h"
void Borrar(struct nodo *pi){
    if(pi!=NULL){
        Borrar(pi->mayores);
        Borrar(pi->menores);
        free(pi);
    }
}
void escribir(struct nodo *pi, int tam, ofstream *myfile){
    if(pi!=NULL){
        for (int i=0; i<tam; ++i)
            *myfile<<pi->grafica[i]<<" ";
        *myfile<<pi->hash<<"\n";
        escribir(pi->mayores, tam, myfile);
        escribir(pi->menores, tam, myfile);
    }
}
void alinear(struct nodo *P, struct nodo **soluciones, unsigned long *contar){
    if (P!=NULL) {
        alinear(P->mayores, soluciones, contar);
        alinear(P->menores, soluciones, contar);
        soluciones[*contar]=P;
        ++(*contar);
    }
}
int main(int argc, const char * argv[]) {
    FILE *finput1;
    ofstream myfile;
    char name[50];
    int nvertices, tlinea, tam, j, dato;
    unsigned long ns, i, k;
    double temp;
    long hash;
    //printf("se imprime %i %s %s %s \n", argc,argv[0], argv[1], argv[2]);
    sprintf(name, "soluciones%s.txt", argv[1]);
    //printf("%s\n", name);
    finput1 = fopen(name,"r");
    if(finput1==NULL){
        printf("Repartir: No esta el archivo\n");
        system("pause");
        exit(23);
    }
    //printf("Empieza la reparticion\n");
    fscanf(finput1,"%d",&nvertices);
    //printf("nvertices=%d\n", nvertices);
    fscanf(finput1,"%d",&tlinea);
    //printf("tlinea=%d\n", tlinea);
    fscanf(finput1,"%d",&tam);
    //printf("tam=%d\n", tam);
    fscanf(finput1,"%lu",&ns);
    //printf("ns=%d\n", ns);
    temp=ns;
    k=floor(temp/72);
    //printf("k=%i\n", k);
    for (i=0; i<ns; ++i) {
        if (i%k==0  && i/k<72){
            myfile.close();
            myfile.open ("solucion"+std::to_string(i/k)+".txt");
            myfile<<nvertices<<" "<<tlinea<<" "<<tam<<" ";
            if (i/k<71)
                myfile<<k<<"\n";
            else
                myfile<<ns-i<<"\n";
        }
        for (j=0; j<tam; ++j){
            fscanf(finput1,"%d",&dato);
            myfile<<dato<<" ";
        }
        fscanf(finput1,"%lu",&hash);
        myfile<<"\n";
    }
    myfile.close();
    if(tam<nvertices){
        sprintf(name, "rm soluciones%s.txt", argv[1]);
        system(name);
        sprintf(name, "./ciclosolucion1.sh %i %i %i", nvertices, tam, tlinea);
        system(name);
    }
}
