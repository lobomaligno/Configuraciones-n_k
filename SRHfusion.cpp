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
using namespace std;
unsigned long obtenerhash(graph *g, int tam){
    hash<string> h;
    string Hash=string((char*)g, tam*sizeof(graph));
    return h(Hash);
}
struct nodo{
    unsigned long hash;
    int *grafica;
    struct nodo *menores;
    struct nodo *mayores;
};
int compararhash(unsigned long hash1, unsigned long hash2){
    if (hash1 < hash2)
        return MENOR;
    if (hash1 > hash2)
        return MAYOR;
    return IGUAL;
}
int buscarhash(struct nodo *pi,unsigned long hash, struct nodo **padre, int *comparacion){
    while (pi!=NULL) {
        *padre=pi;
        *comparacion=compararhash(hash, pi->hash);
        if (*comparacion==IGUAL)
            return ENCONTRADO;
        if (*comparacion==MAYOR)
            pi=pi->mayores;
        else if (*comparacion==MENOR)
            pi=pi->menores;
    }
    return NO_ENCONTRADO;
}
int encontrar_o_agregar(struct nodo **raiz, unsigned long hash, int *grafica, int tgrafica){
    struct nodo *padre;
    int comparacion;
    if(buscarhash(*raiz, hash, &padre, &comparacion)==NO_ENCONTRADO){
        int k;
        struct nodo *N= (struct nodo *)malloc(sizeof(struct nodo));
        if (N==NULL)
            return 2;
        N->hash=hash;
        N->grafica=(int *)malloc(sizeof(int)*tgrafica);
        for (k = 0; k < tgrafica; ++k)
            N->grafica[k]=grafica[k];
        N->menores=NULL;
        N->mayores=NULL;
        if (*raiz==NULL)
            *raiz=N;
        else{
            if (comparacion==MAYOR)
                padre->mayores=N;
            else
                padre->menores=N;
        }
        return NO_ENCONTRADO;
    }
    return ENCONTRADO;
}
void Borrar(struct nodo *pi){
    if(pi!=NULL){
        Borrar(pi->mayores);
        Borrar(pi->menores);
        free(pi);
    }
}
void imprimirlineas(int nvertices, int tlinea, int *lineas, int tam, ofstream *myfile){
    int i, j, nencontrados;
    for (i=0; i<tam; ++i) {
        //*myfile<<"(";
        printf("(");
        for (j=0, nencontrados=0; j<nvertices && nencontrados<tlinea; ++j){
            if (lineas[i]&(1<<j)){
                ++nencontrados;
                //*myfile<<j+1;
                printf("%i", j+1);
                if (nencontrados<tlinea)
                    *myfile<<" ";
            }
        }
        //*myfile<<")";
        printf(")");
    }
    //*myfile<<"\n";
    printf("\n");
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
int buscarpar(int *lineas, int etapa, int a, int b){
    int i;
    b=(1<<b);
    b|=(1<<a);
    for (i=0; i<etapa; i++)
        if ((lineas[i]&b)==b)
            return ENCONTRADO;
    return NO_ENCONTRADO;
}
int buscarvertice(int *lineas, int etapa, int vertice, int tlinea){
    int i, contador;
    for (i=0, contador=0; i<etapa; i++)
        if (lineas[i]&(1<<vertice)){
            contador++;
            if (contador==tlinea)
                return ENCONTRADO;
        }
    return NO_ENCONTRADO;
}
int validar(int *lineas, int *linea, int k, int vertice, int paso, int tlinea){
    if(buscarvertice(lineas, paso, vertice, tlinea)==ENCONTRADO)
        return INVALIDO;
    for (int i=0; i<k; ++i)
        if (buscarpar(lineas, paso, linea[i], vertice)==ENCONTRADO)
            return INVALIDO;
    return VALIDO;
}
void graficar(graph *g, int tgrafica, int *lineas, int terminado, int nvertices, int tlinea){
    int i, j, k;
    EMPTYGRAPH(g,1,tgrafica);
    for (i=0; i<terminado; i++)
        for (j=0; j<nvertices; ++j)
            if (lineas[i]&(1<<j)){
                ADDONEEDGE(g, j, nvertices+i, 1);
                for (k=j+1; k<nvertices; ++k)
                    if (lineas[i]&(1<<k))
                        ADDONEEDGE(g, j, k, 1);
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
    sprintf(name, "soluciones%s.txt", argv[1]);
    finput1 = fopen(name,"r");
    if(finput1==NULL){
        printf("No esta el archivo\n");
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
    for (archivos=2; archivos<argc; ++archivos) {
        sprintf(name, "soluciones%s.txt", argv[archivos]);
        finput2 = fopen(name,"r");
        if(finput2==NULL){
            printf("No esta el archivo\n");
            system("pause");
            exit(23);
        }
        fscanf(finput2,"%d",&ns2);
        if (ns2!=nvertices){
            printf("Error\n");
            system("pause");
            exit(23);
        }
        fscanf(finput2,"%d",&ns2);
        if (ns2!=tlinea){
            printf("Error\n");
            system("pause");
            exit(23);
        }
        fscanf(finput2,"%d",&ns2);
        if (ns2!=tam){
            printf("Error\n");
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
        printf("lotes %s y %s fusionados\n", argv[1], argv[archivos]);
    }
    ofstream myfile;
    sprintf(name, "soluciones%s.txt", argv[1]);
    myfile.open (name);
    myfile<<nvertices<<" "<<tlinea<<" "<<tam<<" "<<contador<<"\n";
    escribir(arbol, tam, &myfile);
    myfile.close();
    Borrar(arbol);
}
