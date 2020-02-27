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
#include "SRHfunciones.h"
#include "naututil.h"
void Borrar(struct nodo *pi){
    if(pi!=NULL){
        Borrar(pi->mayores);
        Borrar(pi->menores);
        free(pi->grafica);
        free(pi);
    }
}
void registrarlinea(int *lineas, int tam, ofstream *myfile, unsigned long hash){
    for (int i=0; i<tam; ++i)
        *myfile<<lineas[i]<<" ";
    *myfile<<hash<<"\n";
}
void escribir(struct nodo *pi, int tam, ofstream *myfile){
    if(pi!=NULL){
        escribir(pi->menores, tam, myfile);
        int temp;
        unsigned long temp2;
        for (int i=0; i<tam; ++i){
            //*myfile<<pi->grafica[i]<<" ";
            temp=pi->grafica[i];
            myfile->write( (char*)&temp, sizeof(int));
        }
        //*myfile<<pi->hash<<"\n";
        temp2=pi->hash;
        myfile->write( (char*)&temp2, sizeof(long));
        escribir(pi->mayores, tam, myfile);
    }
}
void escribir2(struct nodo *pi, ofstream *myfile){
    if(pi!=NULL){
        *myfile<<pi->hash<<"\n";
        escribir2(pi->mayores, myfile);
        escribir2(pi->menores, myfile);
    }
}
void alinear(struct nodo *P, struct nodo **soluciones, int *contar){
    if (P!=NULL) {
        alinear(P->mayores, soluciones, contar);
        alinear(P->menores, soluciones, contar);
        soluciones[*contar]=P;
        ++(*contar);
    }
}
int main(int argc, const char * argv[]) {
    char name[50];
    int nvertices, tlinea, tam, i, a, avance;
    unsigned long k, contar1, contar2;
    sprintf(name, "solucion%s.bin", argv[1]);
    //printf("name=%s\n",name);
    ifstream finput (name, ios::in|ios::binary);
    //printf("archivo abierto\n",name);
    if(!finput.is_open()){
        printf("Traducir: No esta el archivo %s\n", argv[1]);
        system("pause");
        exit(23);
    }
    //printf("archivo abierto confirmado\n");
    finput.read((char*)&nvertices, sizeof(int));
    finput.read((char*)&tlinea, sizeof(int));
    finput.read((char*)&tam, sizeof(int));
    finput.read((char*)&contar1, sizeof(long));
    //printf("paso: nvertices=%d, tlinea=%d, tam=%d y contar=%d\n", nvertices, tlinea, tam, contar1);
    int lineas[nvertices], lab[2*nvertices], ptn[2*nvertices], orbits[2*nvertices];
    for (i=0; i<2*nvertices; ++i)
        lab[i]=i;
    struct nodo *arbol=NULL;
    for (i=0, k=0; i<tlinea; ++i) {
        lineas[i]=0;
        lineas[i]|=(1<<0);
        for (a=1; a<tlinea; ++a)
            lineas[i]|=(1<<++k);
    }
    contar2=0;
    if (tam<nvertices-2*tlinea-1) {
        avance=1;
    }
    else{
        avance=tlinea+1;
    }
    //printf("avance=%d\n", avance);
    for (k=0; k<contar1; ++k){
        for (a=0; a<tam; ++a){
            //fscanf(finput,"%d",&lineas[a+tlinea]);
            finput.read((char*)&lineas[a+tlinea], sizeof(int));
        }
	//imprimirlineas(nvertices, tlinea, lineas, tlinea+tam);
        llenarlineas(lineas, lab, ptn, orbits, nvertices, tlinea, tlinea+tam, &arbol, &contar2, avance-1);
    }
    sprintf(name, "soluciones%s.bin", argv[1]);
    ofstream myfile;
    myfile.open (name, ios::out | ios::binary);
    //printf("segundo archivo abierto\n");
    myfile.write((char*)&nvertices, sizeof(int));
    myfile.write((char*)&tlinea, sizeof(int));
    tam+=avance;
    //printf("tam=%d\n", tam);
    myfile.write((char*)&tam, sizeof(int));
    myfile.write((char*)&contar2, sizeof(long));
    //printf("%s contar2=%lu\n", argv[1], contar2);
    escribir(arbol, tam, &myfile);
    //printf("Archivo escrito\n");
    myfile.close();
    Borrar(arbol);
    //printf("Paso en %s dio %lu soluciones\n", argv[1], contar2);
    return 0;
}
