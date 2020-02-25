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
    //FILE *finput1;
    //FILE *finput2;
    char name[50];
    int nvertices, tlinea, tam, archivos;
    unsigned long ns1, ns2, i, contador=0;
    sprintf(name, "soluciones0.bin");
    //finput1 = fopen(name,"r");
    ifstream finput1 (name, ios::in|ios::binary);
    if(!finput1.is_open()){
        printf("Fusion: No esta el archivo %s\n", name);
        system("pause");
        exit(23);
    }
    /*
    fscanf(finput1,"%d",&nvertices);
    fscanf(finput1,"%d",&tlinea);
    fscanf(finput1,"%d",&tam);
    fscanf(finput1,"%lu",&ns1);
     */
    finput1.read((char*)&nvertices, sizeof(int));
    finput1.read((char*)&tlinea, sizeof(int));
    finput1.read((char*)&tam, sizeof(int));
    finput1.read((char*)&contar1, sizeof(long));
    int j;
    int lineas[tam];
    unsigned long hash;
    struct nodo*arbol=NULL;
    for (i=0; i<ns1; ++i){
        for (j=0; j<tam; ++j){
            //fscanf(finput1,"%d",&lineas[j]);
            finput1.read((char*)&lineas[j], sizeof(int));
        }
        //fscanf(finput1,"%lu",&hash);
        finput1.read((char*)&hash, sizeof(long));
        if(encontrar_o_agregar(&arbol, hash, lineas, tam)==NO_ENCONTRADO)
            ++contador;
    }
    ifstream finput2;
    for (archivos=1; archivos<72; ++archivos) {
        sprintf(name, "soluciones%d.bin ", archivos);
        //printf("Fusion: Integrando el archivo %s\n", name);
        //finput2 = fopen(name,"r");
        std::ifstream input2(name);
        if(!finput2.is_open()){
            printf("No esta el archivo\n");
            system("pause");
            exit(23);
        }
        //fscanf(finput2,"%d",&j);
        finput2.read((char*)&j, sizeof(int));
        if (j!=nvertices){
            printf("Error, el numero de vertices es incoerente.\n");
            system("pause");
            exit(23);
        }
        //fscanf(finput2,"%d",&j);
        finput2.read((char*)&j, sizeof(int));
        if (j!=tlinea){
            printf("Error, el numero de vertices por linea es incoerente.\n");
            system("pause");
            exit(23);
        }
        //fscanf(finput2,"%d",&j);
        finput2.read((char*)&j, sizeof(int));
        if (j!=tam){
            printf("Error, el tamaño de las soluciones parciales es incoerente.\n");
            system("pause");
            exit(23);
        }
        //fscanf(finput2,"%lu",&ns2);
        finput2.read((char*)&ns2, sizeof(long));
        for (i=0; i<ns2; ++i){
            for (j=0; j<tam; ++j){
                //fscanf(finput2,"%d",&lineas[j]);
                finput2.read((char*)&lineas[j], sizeof(int));
            }
            //fscanf(finput2,"%lu",&hash);
            finput2.read((char*)&hash, sizeof(long));
            if(encontrar_o_agregar(&arbol, hash, lineas, tam)==NO_ENCONTRADO)
                ++contador;
        }
    }
    if(tam<nvertices-tlinea){
        printf("%lu soluciones parciales en la etapa %d\n", contador, tam);
    }
    else{
        printf("Existen %lu configuraciones %d_%d\n", contador, nvertices, tlinea);
    }
    for (archivos=0; archivos<72; ++archivos) {
        sprintf(name, "rm soluciones%d.txt", archivos);
        system(name);
    }
    ofstream myfile;
    sprintf(name, "soluciones0.txt");
    //myfile.open (name);
    myfile.open (name, ios::out | ios::binary);
    //myfile<<nvertices<<" "<<tlinea<<" "<<tam<<" "<<contador<<"\n";
    myfile.write((char*)&nvertices, sizeof(int));
    myfile.write((char*)&tlinea, sizeof(int));
    myfile.write((char*)&tam, sizeof(int));
    myfile.write((char*)&contador, sizeof(long));
    escribir(arbol, tam, &myfile);
    myfile.close();
    Borrar(arbol);
}
