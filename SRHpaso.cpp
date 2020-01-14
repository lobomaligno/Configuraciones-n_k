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
        for (int i=0; i<tam; ++i)
            *myfile<<pi->grafica[i]<<" ";
        *myfile<<pi->hash<<"\n";
        escribir(pi->mayores, tam, myfile);
        escribir(pi->menores, tam, myfile);
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
    FILE *finput;
    char name[50];
    int nvertices, tlinea, tam, i, a, avance;
    unsigned long k, contar1, contar2;
    sprintf(name, "solucion%s.txt", argv[1]);
    finput = fopen(name,"r");
    if(finput==NULL){
        printf("Paso: No esta el archivo %s\n", name);
        system("pause");
        exit(23);
    }
    fscanf(finput,"%d",&nvertices);
    fscanf(finput,"%d",&tlinea);
    fscanf(finput,"%d",&tam);
    fscanf(finput,"%lu",&contar1);
    //printf("nvertices=%d, tlinea=%d, tam=%d y contar=%d\n", nvertices, tlinea, tam, contar1);
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
    //Vercion 1
    /*
    if (tam<nvertices-6) {
        for (k=0; k<contar1; ++k){
            for (a=0; a<tam; ++a)
                fscanf(finput,"%d",&lineas[a+tlinea]);
            llenarlineas(lineas, lab, ptn, orbits, nvertices, tlinea, tlinea+tam, &arbol, &contar2, 0);
        }
        sprintf(name, "soluciones%s.txt", argv[1]);
        ofstream myfile;
        myfile.open (name);
        myfile<<nvertices<<" "<<tlinea<<" "<<tam+1<<" "<<contar2<<"\n";
        escribir(arbol, tam+1, &myfile);
        myfile.close();
    }
    else{
        for (k=0; k<contar1; ++k){
            for (a=0; a<tam; ++a)
                fscanf(finput,"%d",&lineas[a+tlinea]);
            llenarlineas(lineas, lab, ptn, orbits, nvertices, tlinea, tlinea+tam, &arbol, &contar2, 2);
        }
        sprintf(name, "S%stotales.txt", argv[1]);
        ofstream myfile;
        myfile.open (name);
        escribir2(arbol, &myfile);
        myfile.close();
    }
    */
    //Vercion 2
    if (tam<nvertices-2*tlinea) {
        avance=1;
    }
    else{
        avance=tlinea;
    }
    for (k=0; k<contar1; ++k){
        for (a=0; a<tam; ++a){
            fscanf(finput,"%d",&lineas[a+tlinea]);
        }
	//imprimirlineas(nvertices, tlinea, lineas, tlinea+tam);
        llenarlineas(lineas, lab, ptn, orbits, nvertices, tlinea, tlinea+tam, &arbol, &contar2, avance-1);
    }
    sprintf(name, "soluciones%s.txt", argv[1]);
    ofstream myfile;
    myfile.open (name);
    myfile<<nvertices<<" "<<tlinea<<" "<<tam+avance<<" "<<contar2<<"\n";
    escribir(arbol, tam+avance, &myfile);
    myfile.close();
    Borrar(arbol);
    sprintf(name, "rm solucion%s.txt", argv[1]);
    system(name);
    //printf("Paso en %s dio %lu soluciones\n", argv[1], contar2);
    return 0;
}
