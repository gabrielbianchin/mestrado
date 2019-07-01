#include <bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

vector <vector<pair<int,float> > > grafo;

struct NO{
    float valor;
    int parent;
    int usado;
    int key;
};

typedef struct{
    NO * nos;
}FILA;

void inicializacao(FILA * f, int vertices, int s){
    NO aux;
    f->nos = (NO *) malloc(sizeof(NO)*(vertices));
    for(int i=0;i<vertices;i++){
        if(i != s)
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
        NO aux;
        aux.usado = -1;
        aux.key = -1;
        aux.parent = -1;
        aux.valor = -1;
        return aux;
    }
}

void relax(FILA * f, int v, float peso){
    (f->nos[v]).valor = peso;
}

void backtracking(FILA f, int u){
    if((f.nos[u]).parent == -1){
        cout<<u<<" ";
        return;
    }
    int pai = (f.nos[u]).parent;
    backtracking(f, pai);
    cout<<u<<" ";
}

int main(){
    FILE *file;
    file = fopen("teste.txt", "r");
    
    int qt_arestas, qt_vertices, a, b, s;
    float p, mst = 0.0;
    
    fscanf(file,"%d", &qt_vertices);
    fscanf(file, "%d", &qt_arestas);
    fscanf(file, "%d", &s);

    FILA f;

    inicializacao(&f, qt_vertices, s);

    grafo.assign(qt_vertices,vector<pair<int,float> >());

    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %f",&a,&b,&p);
        grafo[a].push_back(make_pair(b,p));
    }

    while(true){
        NO w = extract_min(&f, qt_vertices);
        if(w.valor != -1){
            int v = w.key;
            float pesov = w.valor;
            for(int i=0;i<grafo[v].size();i++){
                int u = grafo[v][i].first;
                float peso = grafo[v][i].second;
                if((f.nos[u]).usado != 0 && (f.nos[u].valor) > (pesov + peso)){
                    (f.nos[u]).parent = v;
                    relax(&f, u, pesov + peso);
                }
            }
            (f.nos[v]).usado = 0;
        }
        else
            break;
    }


    for(int i=0;i<qt_vertices;i++){
    	cout<<(f.nos[i]).valor<<" ";
    	backtracking(f, i);
    	cout<<endl;
    }

    return 0;
}