#include <bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

struct NO{
    float valor;
    int parent;
    int usado;
    int key;
};

typedef struct{
    NO * nos;
}FILA;

void inicializacao(FILA * f, int vertices){
    NO aux;
    f->nos = (NO *) malloc(sizeof(NO)*(vertices));
    for(int i=0;i<vertices;i++){
        if(i != 3)
            aux.valor = inf;
        else
            aux.valor = 0;
        aux.parent = -1;
        aux.usado = 1;
        aux.key = i;
        f->nos[i] = aux;
    }
}

NO extract_min(FILA * f, int vertices){
    int menor = inf, indice = -1;
    for(int i=0;i<vertices;i++){
        if((f->nos[i]).usado != 0 && (f->nos[i]).valor < menor){
            menor = (f->nos[i]).valor;
            indice = i;
        }
    }
    if(indice != -1){
        (f->nos[indice]).usado = 0;
        return (f->nos[indice]);
    }
    else{
        NO aux = {-1, -1, -1};
        return aux;
    }
}

void decrease_key(FILA * f, int v, float peso){
    (f->nos[v]).valor = peso;
}

int main(){
    FILE *file;
    file = fopen("teste.txt", "r");
    
    int qt_arestas, qt_vertices, a, b;
    float p, mst = 0.0;
    
    fscanf(file,"%d %d", &qt_vertices, &qt_arestas);

    float grafo[qt_vertices][qt_vertices];

    for(int i=0;i<qt_vertices;i++){
        for(int j=0;j<qt_vertices;j++){
            if(i == j)
                grafo[i][j] = 0;
            else
                grafo[i][j] = inf;
        }
    }

    FILA f;

    inicializacao(&f, qt_vertices);

    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %f",&a,&b,&p);
        grafo[a][b] = p;
        grafo[b][a] = p;
    }

    while(true){
        NO w = extract_min(&f, qt_vertices);
        if(w.valor != -1){
            int v = w.key;
            for(int i=0;i<qt_vertices;i++){
                if(i != v && grafo[i][v] != inf){
                    float peso = grafo[v][i];
                    if((f.nos[i]).usado != 0 && peso < (f.nos[i]).valor){
                        (f.nos[i]).parent = v;
                        decrease_key(&f, i, peso);
                    }
                }
            }
            (f.nos[v]).usado = 0;
            int pai = w.parent;
            if(pai != -1){
                cout<<pai<<" "<<v<<" "<<w.valor<<endl;
                mst += w.valor;
            }
        }
        else
            break;
    }

    cout<<mst<<endl;

    return 0;
}