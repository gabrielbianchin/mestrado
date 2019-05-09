#include <bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

vector <int> vis;
priority_queue<pair<int,int> > pq;
vector <vector<pair<int,int> > > grafo;


struct NO {
    int valor;
    int parent;
    int key;
};

typedef struct {
    NO * nos;
    int tamanho = -1;
}HEAP;

void inicializacao(HEAP * h, int vertices){
    NO aux;
    h->nos = (NO *) malloc(sizeof(NO)*(vertices+1));
    for(int i=0;i<vertices;i++){
        if(i != 0)
            aux.valor = inf;
        else
            aux.valor = 0;
        aux.parent = NULL;
        aux.key = i;
        (h->tamanho)++;
        h->nos[h->tamanho] = aux;
        //cout<<(h->nos)->key<<endl;
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

void heapify(HEAP * h, int u){
    u = (h->nos[u]).key;
    int l = left(u);
    int r = right(u);
    int smallest = u;
    if(l <= h->tamanho && (h->nos[l]).valor < (h->nos[smallest]).valor)
        smallest = l;
    if(r <= h->tamanho && (h->nos[r]).valor < (h->nos[smallest]).valor)
        smallest = r;
    if(smallest != u){
        NO aux = h->nos[u];
        h->nos[u] = h->nos[smallest];
        h->nos[smallest] = aux;
        heapify(h, smallest);
    }
}

void build_heap(HEAP * h){
    int n = h->tamanho;
    for(int i = (n/2) - 1; i >= 0; i--){
        heapify(h, i);
    }
}

NO extract_min(HEAP * h){
    int n = h->tamanho;
    if(n < 1){
        NO aux = {-1, -1, -1};
        return aux;
    }
    else{
        NO aux = h->nos[0];
        h->nos[0] = h->nos[n-1];
        (h->tamanho)--;
        heapify(h, 0);
        return aux;
    }
}

void decrease_key(HEAP * h, int v, int peso){
    (h->nos[v]).valor = peso;
    int pai = (h->nos[v]).parent;
    while(v > 0 && (h->nos[v]).valor < (h->nos[pai]).valor){
        NO aux = h->nos[v];
        h->nos[v] = h->nos[pai];
        h->nos[pai] = aux;
        v = pai;
    }
}


int main(){
    FILE *file;
    file = fopen("teste.txt", "r");
    
    int qt_arestas, qt_vertices, a, b, p;
    fscanf(file,"%d %d", &qt_vertices, &qt_arestas);

    HEAP h;
    int c[1000], mst = 0;

    for(int i=0;i<qt_vertices;i++)
        c[i] = -1;

    inicializacao(&h, qt_vertices);

    build_heap(&h);

    grafo.assign(qt_vertices,vector<pair<int,int> >());

    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %d",&a,&b,&p);
        grafo[a].push_back(make_pair(b,p));
        grafo[b].push_back(make_pair(a,p));
    }

    while(h.tamanho > 0){
        NO w = extract_min(&h);
        if(w.valor != -1){
            int v = w.key;
            for(int i=0;i<grafo[v].size();i++){
                int u = grafo[v][i].first, peso = grafo[v][i].second;

                if(c[u] != -1 && peso < (h.nos[u]).valor){
                    (h.nos[u]).parent = v;
                    decrease_key(&h, u, peso);
                }
            }
            c[v] = 0;
            int pai = w.parent;
            
            cout<<pai<<" "<<grafo[pai][v].first<<" "<<grafo[pai][v].second<<endl;
            mst += grafo[pai][v].second;
        }
    }

    cout<<mst<<endl;

    
    return 0;
}