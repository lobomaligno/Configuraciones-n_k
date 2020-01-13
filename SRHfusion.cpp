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
void alinear(struct nodo *P, struct nodo **soluciones, int *contar){
    if (P!=NULL) {
        alinear(P->mayores, soluciones, contar);
        alinear(P->menores, soluciones, contar);
        soluciones[*contar]=P;
        ++(*contar);
    }
}
int main(int argc, const char * argv[]) {
    FILE *finput1;
    FILE *finput2;
    char name[50];
    int nvertices, tlinea, tam, ns1, ns2, archivos;
    sprintf(name, "soluciones0.txt");
    finput1 = fopen(name,"r");
    if(finput1==NULL){
        printf("Fusion: No esta el archivo %s\n", name);
        system("pause");
        exit(23);
    }
    fscanf(finput1,"%d",&nvertices);
    fscanf(finput1,"%d",&tlinea);
    fscanf(finput1,"%d",&tam);
    fscanf(finput1,"%d",&ns1);
    int i, j, contador=0;
    int lineas[tam];
    unsigned long hash;
    struct nodo*arbol=NULL;
    for (i=0; i<ns1; ++i){
        for (j=0; j<tam; ++j)
            fscanf(finput1,"%d",&lineas[j]);
        fscanf(finput1,"%lu",&hash);
        if(encontrar_o_agregar(&arbol, hash, lineas, tam)==NO_ENCONTRADO)
            ++contador;
    }
    //printf("Contador=%d\n", contador);
    for (archivos=1; archivos<72; ++archivos) {
        sprintf(name, "soluciones%d.txt", archivos);
        //printf("Fusion: Integrando el archivo %s\n", name);
        finput2 = fopen(name,"r");
        if(finput2==NULL){
            printf("No esta el archivo\n");
            system("pause");
            exit(23);
        }
        fscanf(finput2,"%d",&ns2);
        if (ns2!=nvertices){
            printf("Error, el numero de vertices es incoerente.\n");
            system("pause");
            exit(23);
        }
        fscanf(finput2,"%d",&ns2);
        if (ns2!=tlinea){
            printf("Error, el numero de vertices por linea es incoerente.\n");
            system("pause");
            exit(23);
        }
        fscanf(finput2,"%d",&ns2);
        if (ns2!=tam){
            printf("Error, el tamaño de las soluciones parciales es incoerente.\n");
            system("pause");
            exit(23);
        }
        fscanf(finput2,"%d",&ns2);
        for (i=0; i<ns2; ++i){
            for (j=0; j<tam; ++j)
                fscanf(finput2,"%d",&lineas[j]);
            fscanf(finput2,"%lu",&hash);
            if(encontrar_o_agregar(&arbol, hash, lineas, tam)==NO_ENCONTRADO)
                ++contador;
        }
        //printf("Contador=%d\n", contador);
        //printf("lotes fusionados\n");
    }
    //printf("Fusion: tam=%i", tam);
    if(tam<nvertices-tlinea){
        printf("%d soluciones parciales en la etapa %d\n", contador, tam);
    }
    else{
        printf("Existen %d configuraciones %d_%d\n", contador, nvertices, tlinea);
    }
    for (archivos=0; archivos<72; ++archivos) {
        sprintf(name, "rm soluciones%d.txt", archivos);
        system(name);
    }
    ofstream myfile;
    sprintf(name, "soluciones0.txt");
    myfile.open (name);
    myfile<<nvertices<<" "<<tlinea<<" "<<tam<<" "<<contador<<"\n";
    escribir(arbol, tam, &myfile);
    myfile.close();
    Borrar(arbol);
}
