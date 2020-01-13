//
//  SRHfunciones.h
//  
//
//  Created by Víctor Andrés Hernández Patiño on 12/01/20.
//

#ifndef SRHfunciones_h
#define SRHfunciones_h
int buscarpar(int *lineas, int etapa, int a, int b){
    int i;
    b=(1<<b);
    b|=(1<<a);
    for (i=0; i<etapa; i++)
        if ((lineas[i]&b)==b)
            return ENCONTRADO;
    return NO_ENCONTRADO;
}
void graficar(graph *g, int tgrafica, int *lineas, int terminado, int nvertices, int tlinea){
    int i, j, k;
    EMPTYGRAPH(g,1,tgrafica);
    for (i=0; i<terminado; i++){
        for (j=0; j<nvertices; ++j){
            if (lineas[i]&(1<<j)){
                ADDONEEDGE(g, j, nvertices+i, 1);
                for (k=j+1; k<nvertices; ++k)
                    if (lineas[i]&(1<<k))
                        ADDONEEDGE(g, j, k, 1);
            }
        }
    }
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
void llenarlineas(int *lineas, int *lab, int *ptn, int *orbits, int nvertices, int tlinea, int etapa, struct nodo **arbol, int *contar, int salto){
    unsigned long hash;
    int k=1, i, e;
    int linea[tlinea], S[tlinea];
    graph solucion[nvertices+etapa+1], canon[nvertices+etapa+1];
    static DEFAULTOPTIONS_GRAPH(options);
    options.getcanon = TRUE;
    options.defaultptn=TRUE;
    statsblk stats;
    for (i=0; i<nvertices && (!(lineas[etapa-1]&(1<<i))); ++i);
    while (buscarvertice(lineas, etapa, i, tlinea)==ENCONTRADO){
        ++i;
    }
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
                        hash=obtenerhash(canon, nvertices+etapa+1);
                        //hash=hashgraph(canon, 1, nvertices+etapa+1,0);
                        e=encontrar_o_agregar(arbol, hash, &lineas[tlinea], etapa-tlinea+1);
                        if (e!=ENCONTRADO){
                            ++*contar;
                            if(etapa+1==nvertices)
                                imprimirlineas(nvertices, tlinea, lineas, etapa+1);
                            //printf(" %lu\n", hash);
                        }
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
#endif /* SRHfunciones_h */
