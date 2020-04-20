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
    printf("tam=%d\n", tam);
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
    }
    unsigned long minHash;
    unsigned long anterior=0;
    int lineas[72][tam];
    unsigned long hashM[72];
    bool libre;
    bool falta[72];
    for(archivos=0; archivos<72; archivos++){
	for (j=0; j<tam; ++j){
	    fscanf(finput[archivos],"%d",&lineas[archivos][j]);
	}
	fscanf(finput[archivos],"%lu",&hashM[archivos]);
        falta[archivos]=true;
    }
    ofstream myfile;
    sprintf(name, "SOLUCION.txt");
    myfile.open (name);
    do{
	minHash=ULONG_MAX;
	for(archivos=0; archivos<72; archivos++){
	    if(ntam[archivos]>0){
		if(hashM[archivos]<=anterior){
		    printf("\nAlerta\n\n");
                }
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
		    myfile<<"\t";
		    myfile<<minHash<<"\n";
		    ++contador;
		    //printf("Hash=%lu\n", minHash);
		    if(minHash<=anterior){
			printf("\nAlerta\n\n");
		    }
		    anterior=minHash;
		}
                for (j=0; j<tam; ++j){
		    fscanf(finput[archivos],"%d",&lineas[archivos][j]);
                }
		fscanf(finput[archivos],"%lu",&hashM[archivos]);
		ntam[archivos]--;
		//printf("El tama%co del archivo %i es %d\n", 164, archivos, ntam[archivos]);
	    }
	}	
    }while(minHash<ULONG_MAX);
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
    sprintf(name, "datos.txt");
    myfile2.open (name);
    myfile2<<nvertices<<" "<<tlinea<<" "<<tam<<" "<<contador<<"\n";
    myfile2.close();
    rename("SOLUCION.txt", "soluciones0.txt");
}
