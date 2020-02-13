#define ENCONTRADO 1
#define NO_ENCONTRADO 0
#define VALIDO 0
#define INVALIDO 1
#define MENOR -1
#define IGUAL 0
#define MAYOR 1
#include "nauty.h"
#include "profile_time.h"
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
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
void alinear(struct nodo *P, struct nodo **soluciones, unsigned long *contar){
    if (P!=NULL) {
        alinear(P->mayores, soluciones, contar);
        alinear(P->menores, soluciones, contar);
        soluciones[*contar]=P;
        ++(*contar);
    }
}
void SRP(int nvertices, int tlinea){
    int lineas[nvertices], lab[2*nvertices], ptn[2*nvertices], orbits[2*nvertices];
    int a, etapa;
    unsigned long contar1=0, contar2, k, i;
    char ejecutar[25];
    double temp;
    struct nodo *arbol1=NULL, *arbol2=NULL, *cambio;
    struct nodo **soluciones;
    for (i=0, k=0; i<tlinea; ++i) {
        lineas[i]=0;
        lineas[i]|=(1<<0);
        for (a=1; a<tlinea; ++a)
            lineas[i]|=(1<<++k);
    }
    for (i=0; i<2*nvertices; i++)
        lab[i]=i;
    llenarlineas(lineas, lab, ptn, orbits, nvertices, tlinea, tlinea, &arbol1, &contar1, 0);
    printf("%i soluciones parciales en la etapa %i\n", contar1, 1);
    if(nvertices<12||(tlinea==4 && nvertices<17)){
        for (etapa=tlinea+1; etapa<nvertices-tlinea+1; ++etapa){
            soluciones=(struct nodo**)malloc(sizeof(struct nodo*)*contar1);
            contar1=0;
            alinear(arbol1, soluciones, &contar1);
            for (k=0, contar2=0; k<contar1; ++k){
                for (a=tlinea; a<etapa; ++a)
                    lineas[a]=soluciones[k]->grafica[a-tlinea];
                if (etapa<nvertices-tlinea)
                    llenarlineas(lineas, lab, ptn, orbits, nvertices, tlinea, etapa, &arbol2, &contar2, 0);
                else
                    llenarlineas(lineas, lab, ptn, orbits, nvertices, tlinea, etapa, &arbol2, &contar2, tlinea-1);
            }
            if (etapa<nvertices-tlinea){
                printf("%i soluciones parciales en la etapa %i\n", contar2, etapa-tlinea+1);
            }
            else{
                printf("%i soluciones totales.\n", contar2);
            }
            contar1=contar2;
            cambio=arbol1;
            arbol1=arbol2;
            arbol2=cambio;
            Borrar(arbol2);
            arbol2=NULL;
            free(soluciones);
        }
        Borrar(arbol1);
    }
    else{
        for (etapa=tlinea+1; etapa<8; ++etapa){
            soluciones=(struct nodo**)malloc(sizeof(struct nodo*)*contar1);
            contar1=0;
            alinear(arbol1, soluciones, &contar1);
            for (k=0, contar2=0; k<contar1; ++k){
                for (a=tlinea; a<etapa; ++a)
                    lineas[a]=soluciones[k]->grafica[a-tlinea];
                llenarlineas(lineas, lab, ptn, orbits, nvertices, tlinea, etapa, &arbol2, &contar2, 0);
            }
            if (etapa<8)
                printf("%i soluciones parciales en la etapa %i\n", contar2, etapa-tlinea+1);
            else
                printf("%i soluciones guardadas.\n", contar2);
            contar1=contar2;
            cambio=arbol1;
            arbol1=arbol2;
            arbol2=cambio;
            Borrar(arbol2);
            arbol2=NULL;
            free(soluciones);
        }
        soluciones=(struct nodo**)malloc(sizeof(struct nodo*)*contar1);
        contar1=0;
        alinear(arbol1, soluciones, &contar1);
        ofstream myfile;
        temp=contar1;
        k=floor(temp/72);
        //printf("k=%i\n", k);
        for (i=0; i<contar1; ++i) {
            if (i%k==0 && i/k<72){
                myfile.close();
                myfile.open ("solucion"+std::to_string(i/k)+".txt");
                //printf("solucion%i.txt abieto\n", (i/k));
                myfile<<nvertices<<" "<<tlinea<<" "<<8-tlinea<<" ";
                if (i/k<71)
                    myfile<<k<<"\n";
                else
                    myfile<<contar1-i<<"\n";
            }
            for (a=0; a<8-tlinea; ++a)
                myfile<<soluciones[i]->grafica[a]<<" ";
            myfile<<"\n";
        }
        myfile.close();
        free(soluciones);
        Borrar(arbol1);
        //sprintf(ejecutar, "./ciclosolucion1.sh %i %i %i", nvertices, 8-tlinea, tlinea);
        //system(ejecutar);
    }
}
int main(int argc, const char * argv[]) {
    int nvertices, tlinea;
    char continuar='s';
    nauty_check(WORDSIZE,1,MAXN,NAUTYVERSIONID);
    struct rusage ru_begin;
    struct rusage ru_end;
    struct timeval tv_elapsed;
    while(continuar=='s'){
        do{
            printf("¿Cuantos vertices?\n");
            scanf("%d", &nvertices);
	    while(getchar() != '\n');
            if (nvertices>20 || nvertices<7)
                printf("Solo valores entre 7 y 20");
        }while(nvertices>20 || nvertices<7);
        do{
            printf("¿Cuantos vertices por linea?\n");
            scanf("%d", &tlinea);
	    while(getchar() != '\n');
            if(tlinea!=3 && tlinea!=4)
                printf("Solo 3 o 4");
        }while (tlinea!=3 && tlinea!=4);
        getrusage(RUSAGE_SELF, &ru_begin);
        SRP(nvertices, tlinea);
        getrusage(RUSAGE_SELF, &ru_end);
        timeval_subtract(&tv_elapsed, &ru_end.ru_utime, &ru_begin.ru_utime);
        printf("Proceso terminado tardo %g ms.\n", (tv_elapsed.tv_sec + (tv_elapsed.tv_usec/1000000.0))*1000.0);
        printf("¿Desea continuar\n");
        scanf("%s", &continuar);
    }
    return 0;
}
