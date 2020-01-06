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
#include "naututil.h"
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
        if (N->grafica==NULL){
            free(N);
            return 2;
        }
        N->menores=NULL;
        N->mayores=NULL;
        for (k = 0; k < tgrafica; ++k)
            N->grafica[k]=grafica[k];
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
        free(pi->grafica);
        free(pi);
    }
}
void imprimirlineas(int nvertices, int tlinea, int *lineas, int tam, ofstream *myfile){
    int i, j, nencontrados;
    for (i=0; i<tam; ++i) {
        *myfile<<"(";
        for (j=0, nencontrados=0; j<nvertices && nencontrados<tlinea; ++j){
            if (lineas[i]&(1<<j)){
                ++nencontrados;
                *myfile<<j+1;
                if (nencontrados<tlinea)
                    *myfile<<" ";
            }
        }
        *myfile<<")";
    }
    *myfile<<"\n";
}
void registrarlinea(int *lineas, int tam, ofstream *myfile, unsigned long hash){
    for (int i=0; i<tam; ++i)
        *myfile<<lineas[i]<<" ";
    *myfile<<hash<<"\n";
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
void llenarlineas(int *lineas, int *lab, int *ptn, int *orbits, int nvertices, int tlinea, int etapa, struct nodo **arbol, int *contar, int salto){
    unsigned long hash;
    int k=1, i, e;
    int linea[tlinea], S[tlinea];
    graph solucion[2*nvertices], canon[2*nvertices];
    static DEFAULTOPTIONS_GRAPH(options);
    options.getcanon = TRUE;
    options.defaultptn=TRUE;
    statsblk stats;
    for (i=0; i<nvertices && (!(lineas[etapa-1]&(1<<i))); ++i);
    while (buscarvertice(lineas, etapa, i, tlinea)==ENCONTRADO)
    ++i;
    linea[0]=i;
    S[1]=linea[0]+1;
    while (k>0) {
        while (S[k]<=nvertices-tlinea+k) {
            linea[k]=S[k];
            ++S[k];
            if (validar(lineas, linea, k, linea[k], etapa, tlinea)==VALIDO) {
                if (k==tlinea-1){
                    lineas[etapa]=0;
                    for (i=0; i<tlinea; ++i)
                    lineas[etapa]|=(1<<linea[i]);
                    if (salto==0){
                        graficar(solucion, nvertices+etapa+1, lineas, etapa+1, nvertices, tlinea);
                        densenauty(solucion, lab, ptn, orbits, &options, &stats, 1, nvertices+etapa+1, canon);
                        //hash=hashgraph(canon, 1, nvertices+etapa+1,0);
                        hash=obtenerhash(canon, nvertices+etapa+1);
                        e=encontrar_o_agregar(arbol, hash, &lineas[tlinea], nvertices+etapa+1);
                        if (e!=ENCONTRADO)
                            ++*contar;
                    }
                    else
                        llenarlineas(lineas, lab, ptn, orbits, nvertices, tlinea, etapa+1, arbol, contar, salto-1);
                    
                }
                else{
                    ++k;
                    S[k]=S[k-1];
                }
            }
        }
        --k;
    }
}
int main(int argc, const char * argv[]) {
    FILE *finput;
    char name[50];
    int nvertices, tlinea, tam, i, k, a, contar1, contar2, avance;
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
    fscanf(finput,"%d",&contar1);
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
        avance=1
    }
    else{
        avance=tlinea;
    }
    for (k=0; k<contar1; ++k){
        for (a=0; a<tam; ++a){
            fscanf(finput,"%d",&lineas[a+tlinea]);
        }
        llenarlineas(lineas, lab, ptn, orbits, nvertices, tlinea, tlinea+tam, &arbol, &contar2, avance-1);
    }
    sprintf(name, "soluciones%s.txt", argv[1]);
    ofstream myfile;
    myfile.open (name);
    myfile<<nvertices<<" "<<tlinea<<" "<<tam+avance<<" "<<contar2<<"\n";
    escribir(arbol, tam+avance, &myfile);
    myfile.close();
    Borrar(arbol);
    //printf("Paso %s\n", argv[1]);
    return 0;
}
