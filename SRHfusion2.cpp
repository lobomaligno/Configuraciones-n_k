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
#include <climits>
#include "SRHfunciones.h"
void Borrar(struct nodo *pi){
    if(pi!=NULL){
        Borrar(pi->mayores);
        Borrar(pi->menores);
        free(pi);
    }
}
/*
void escribir(struct nodo *pi, int tam, ofstream *myfile){
    if(pi!=NULL){
        for (int i=0; i<tam; ++i)
            *myfile<<pi->grafica[i]<<" ";
        *myfile<<pi->hash<<"\n";
        escribir(pi->mayores, tam, myfile);
        escribir(pi->menores, tam, myfile);
    }
}
*/
void alinear(struct nodo *P, struct nodo **soluciones, int *contar){
    if (P!=NULL) {
        alinear(P->mayores, soluciones, contar);
        alinear(P->menores, soluciones, contar);
        soluciones[*contar]=P;
        ++(*contar);
    }
}
int main(int argc, const char * argv[]) {
    FILE *finput[72];
    char name[50];
    int nvertices, tlinea, tam, archivos;
    unsigned long ns1, ns2, i, contador=0;
    unsigned long ntam[72];
    sprintf(name, "soluciones0.txt");
    finput[0] = fopen(name,"r");
    if(finput[0]==NULL){
        printf("Fusion: No esta el archivo %s\n", name);
        system("pause");
        exit(23);
    }
    fscanf(finput[0],"%d",&nvertices);
    fscanf(finput[0],"%d",&tlinea);
    fscanf(finput[0],"%d",&tam);
    fscanf(finput[0],"%lu",&ntam[0]);
    int j;
    unsigned long hash;
    /*
	for (i=0; i<ns1; ++i){
	    for (j=0; j<tam; ++j)
		fscanf(finput1,"%d",&lineas[j]);
	    fscanf(finput1,"%lu",&hash);
	    if(encontrar_o_agregar(&arbol, hash, lineas, tam)==NO_ENCONTRADO)
		++contador;
	}
    */
    //printf("Contador=%d\n", contador);
    for (archivos=1; archivos<72; ++archivos) {
        sprintf(name, "soluciones%d.txt", archivos);
        //printf("Fusion: Integrando el archivo %s\n", name);
        finput[archivos]=fopen(name,"r");
        if(finput[archivos]==NULL){
            printf("No esta el archivo\n");
            system("pause");
            exit(23);
        }
        fscanf(finput[archivos],"%d",&j);
        if (j!=nvertices){
            printf("Error, el numero de vertices es incoerente.\n");
            system("pause");
            exit(23);
        }
        fscanf(finput[archivos],"%d",&j);
        if (j!=tlinea){
            printf("Error, el numero de vertices por linea es incoerente.\n");
            system("pause");
            exit(23);
        }
        fscanf(finput[archivos],"%d",&j);
        if (j!=tam){
            printf("Error, el tamaño de las soluciones parciales es incoerente.\n");
            system("pause");
            exit(23);
        }
        fscanf(finput[archivos],"%lu",&ntam[archivos]);
	/*
	    for (i=0; i<ns2; ++i){
		for (j=0; j<tam; ++j)
		    fscanf(finput2,"%d",&lineas[j]);
		fscanf(finput2,"%lu",&hash);
		if(encontrar_o_agregar(&arbol, hash, lineas, tam)==NO_ENCONTRADO)
		    ++contador;
	    }
	*/
    }
    unsigned long minHash;
    int lineas[72][tam];
    unsigned long hashM[72];
    bool libre;
    for(archivos=0; archivos<72; archivos++){
	for (j=0; j<tam; ++j){
	    fscanf(finput[archivos],"%d",&lineas[archivos][j]);
	}
	fscanf(finput[archivos],"%lu",&hashM[archivos]);
    }
    ofstream myfile;
    sprintf(name, "SOLUCION.txt");
    myfile.open (name);
    //myfile<<nvertices<<" "<<tlinea<<" "<<tam<<" "<<contador<<"\n";
    do{
	minHash=ULONG_MAX;
	for(archivos=0; archivos<72; archivos++){
	    if(ntam[archivos]>0){
		if(hashM[archivos]<minHash){
		    minHash=hashM[archivos];
		    //printf("Hash=%lu\n", minHash);
                }
	    }
	}
	libre=true;
	for(archivos=0; archivos<72; archivos++){
	    if(hashM[archivos]==minHash){
		if(libre){
		    libre=false;
		    for (j=0; j<tam; ++j){
			myfile<<lineas[archivos][j]<<" ";
		    }
		    myfile<<hashM[archivos]<<"\n";
		    ++contador;
		}
		if(ntam[archivos]>1){
		    for (j=0; j<tam; ++j){
			fscanf(finput[archivos],"%d",&lineas[archivos][j]);
		    }
		    fscanf(finput[archivos],"%lu",&hashM[archivos]);
		}
		ntam[archivos]--;
		//printf("El tama%co del archivo %i es %d\n", 164, archivos, ntam[archivos]);
	    }
	}	
    }while(minHash<ULONG_MAX);
    /*
    for(archivos=0; archivos<72; archivos++){
	printf("El tama%co del archivo es %d\n", 164, ntam[archivos]);
    }
    */
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
    myfile.close();
    ofstream myfile2;
    sprintf(name, "soluciones0.txt");
    myfile2.open (name);
    std::ifstream ifile("SOLUCION.txt");
    if (!ifile.is_open()) {
        printf("Fusion 2: No esta el archivo SOLUCION.txt\n", name);
        system("pause");
        exit(23);	
    }
    myfile2<<nvertices<<" "<<tlinea<<" "<<tam<<" "<<contador<<"\n";
    myfile2<<ifile.rdbuf();
    sprintf(name, "rm SOLUCION.txt");
    system(name);
}
