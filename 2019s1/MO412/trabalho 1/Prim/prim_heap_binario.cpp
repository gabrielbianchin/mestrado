#include <bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

vector <vector<pair<int,float> > > grafo;
int * c;

struct NO {
    float valor;
    int parent;
    int key;
};

typedef struct {
    NO * nos;
    int tamanho = -1;
}HEAP;

void inicializacao(HEAP * h, int vertices){
    NO aux;
    h->nos = (NO *) malloc(sizeof(NO)*(vertices));
    c = (int *) malloc(sizeof(int)*(vertices));
    for(int i=0;i<vertices;i++){
        c[i] = i;
        if(i != 0)
            aux.valor = inf;
        else
            aux.valor = 0;
        aux.parent = -1;
        aux.key = i;
        (h->tamanho)++;
        h->nos[h->tamanho] = aux;
    }
}

int parent(int u){
    return (u - 1) / 2;
}

int left(int u){
    return (2 * u) + 1;
}

int right(int u){
    return (2 * u) + 2;
}

void swap(HEAP * h, int u, int v){
    NO aux = h->nos[u];
    h->nos[u] = h->nos[v];
    h->nos[v] = aux;
    c[(h->nos[u]).key] = u;
    c[(h->nos[v]).key] = v;
}

void heapify(HEAP * h, int u){
    int l = left(u);
    int r = right(u);
    int smallest = u;
    if((l <= h->tamanho) && ((h->nos[l]).valor < (h->nos[smallest]).valor))
        smallest = l;
    if((r <= h->tamanho) && ((h->nos[r]).valor < (h->nos[smallest]).valor))
        smallest = r;
    if(smallest != u){
        swap(h, u, smallest);
        heapify(h, smallest);
    }
}

void build_heap(HEAP * h){
    int n = h->tamanho;
    for(int i = (n/2); i >= 0; i--){
        heapify(h, c[i]);
    }
}

NO extract_min(HEAP * h){
    int n = h->tamanho;
    if(n < 0){
        NO aux = {-1, -1, -1};
        return aux;
    }
    else if(n == 0){
        NO aux = h->nos[0];
        (h->tamanho)--;
        return aux;
    }
    else{
        NO aux = h->nos[0];
        swap(h, 0, n);
        (h->tamanho)--;
        heapify(h, c[(h->nos[0]).key]);
        return aux;
    }
}

void decrease_key(HEAP * h, int v, float peso){
    (h->nos[v]).valor = peso;
    while(v > 0 && (h->nos[v]).valor < (h->nos[parent(v)]).valor){
        swap(h, v, parent(v));
        v = parent(v);
    }
}

int main(){
    FILE *file;
    file = fopen("teste.txt", "r");
    
    int qt_arestas, qt_vertices, a, b;
    float p, mst = 0.0;
    
    fscanf(file,"%d %d", &qt_vertices, &qt_arestas);

    HEAP h;

    inicializacao(&h, qt_vertices);

    build_heap(&h);

    grafo.assign(qt_vertices,vector<pair<int,float> >());

    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %f",&a,&b,&p);
        grafo[a].push_back(make_pair(b,p));
        grafo[b].push_back(make_pair(a,p));
    }

    while(h.tamanho >= 0){
        NO w = extract_min(&h);
        if(w.valor != -1){
            int v = w.key;
            for(int i=0;i<grafo[v].size();i++){
                int u = grafo[v][i].first;
                float peso = grafo[v][i].second;
                if(c[u] != -1 && peso < (h.nos[c[u]]).valor){
                    (h.nos[c[u]]).parent = v;
                    decrease_key(&h, c[u], peso);
                }
            }
            c[v] = -1;
            int pai = w.parent;
            if(pai != -1){
                cout<<pai<<" "<<v<<" "<<w.valor<<endl;
                mst += w.valor;
            }
        }
    }

    cout<<mst<<endl;

    return 0;
}