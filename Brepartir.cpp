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
    //FILE *finput1;
    ofstream myfile;
    char name[50];
    int nvertices, tlinea, tam, j, dato;
    unsigned long ns, i, k, templong;
    double temp;
    long hash;
    //printf("se imprime %i %s %s %s \n", argc,argv[0], argv[1], argv[2]);
    sprintf(name, "soluciones%s.bin", argv[1]);
    printf("name=%s.\n", name);
    //finput1 = fopen(name,"r");
    ifstream finput1 (name, ios::in|ios::binary);
    if(!finput1.is_open()){
        printf("Repartir: No esta el archivo\n");
        system("pause");
        exit(23);
    }
    /*
    printf("Empieza la reparticion\n");
    fscanf(finput1,"%d",&nvertices);
    printf("nvertices=%d\n", nvertices);
    fscanf(finput1,"%d",&tlinea);
    printf("tlinea=%d\n", tlinea);
    fscanf(finput1,"%d",&tam);
    printf("tam=%d\n", tam);
    fscanf(finput1,"%lu",&ns);
    printf("ns=%d\n", ns);
     */
    finput1.read((char*)&nvertices, sizeof(int));
    finput1.read((char*)&tlinea, sizeof(int));
    finput1.read((char*)&tam, sizeof(int));
    finput1.read((char*)&ns, sizeof(long));
    temp=ns;
    k=floor(temp/72);
    //printf("k=%i\n", k);
    for (i=0; i<ns; ++i) {
        if (i%k==0  && i/k<72){
            myfile.close();
            myfile.open ("solucion"+std::to_string(i/k)+".bin", ios::out|ios::binary);
            //myfile<<nvertices<<" "<<tlinea<<" "<<tam<<" ";
            myfile.write((char*)&nvertices, sizeof(int));
            myfile.write((char*)&tlinea, sizeof(int));
            myfile.write((char*)&tam, sizeof(int));
            if (i/k<71){
                //myfile<<k<<"\n";
                myfile.write((char*)&k, sizeof(long));
            }
            else{
                templong=ns-i;
                myfile.write((char*)&templong, sizeof(long));
                //myfile<<ns-i<<"\n";
            }
        }
        for (j=0; j<tam; ++j){
            /*
            fscanf(finput1,"%d",&dato);
            myfile<<dato<<" ";
            */
            finput1.read((char*)&dato, sizeof(int));
            myfile.write((char*)&dato, sizeof(int));
        }
        /*
        fscanf(finput1,"%lu",&hash);
        myfile<<"\n";
         */
        finput1.read((char*)&hash, sizeof(long));
    }
    myfile.close();
    if(tam<nvertices){
        sprintf(name, "df -h");
        system(name);
        printf("Borrar soluciones%s.bin\n", argv[1]);
        sprintf(name, "rm soluciones%s.bin", argv[1]);
        system(name);
        sprintf(name, "df -h");
        system(name);
        sprintf(name, "./ciclosolucionB.sh %i %i %i", nvertices, tam, tlinea);
        system(name);
    }
}
