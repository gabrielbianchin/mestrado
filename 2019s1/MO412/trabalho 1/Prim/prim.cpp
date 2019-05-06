#include <bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

vector <int> vis;
priority_queue<pair<int,int> > pq;
vector <vector<pair<int,int> > > grafo;


typedef struct {
    int * valor;
    int * parent;
    int tamanho = 0;
}HEAP;



/*



void decrease_key()



void process(int u){
    vis[u]=1;
    for(int i=0;i< grafo[u].size();i++){
        int v = grafo[u][i].first,peso = grafo [u][i].second;
        if(vis[v]==0)
            pq.push(make_pair(-peso,v));
    }
}

int main(){
    FILE *file;
    int qt_arestas,qt_vertices,a,b,c,mst=0;
    file = fopen("teste.txt", "r");
    fscanf(file,"%d %d",&qt_vertices,&qt_arestas);
    vis.assign(qt_vertices+1,0);
    grafo.assign(qt_vertices+1,vector<pair<int,int> >());
    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %d",&a,&b,&c);
        grafo[a].push_back(make_pair(b,c));
        grafo[b].push_back(make_pair(a,c));
    }
    for(int i=1;i<=qt_vertices;i++){
        if(vis[i]==0){
            process(i);
            while(!pq.empty()){
                pair<int,int> aux = pq.top();
                pq.pop();
                int v = aux.second,peso = -aux.first;
                if(vis[v]==0){
                    mst+=peso;
                    process(v);
                }
            }
        }
    }
    cout<<"valor da mst = "<<mst<<endl;
    fclose(file);
    return 0;
}

*/

void inicializacao(HEAP * h, int vertices){
    h->valor = (int*) malloc(sizeof(int)*(vertices+1));
    h->parent = (int*) malloc(sizeof(int)*(vertices+1));
    for(int i=0;i<=vertices;i++){
        (h->tamanho)++;
        h->valor[h->tamanho] = inf;
        h->parent[h->tamanho] = -1;
    }
    h->valor[1] = 0;
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
    int l = left(u);
    int r = right(u);
    int smallest = u;
    if(l <= h->tamanho && h->valor[l] < h->valor[smallest])
        smallest = l;
    if(r <= h->tamanho && h->valor[r] < h->valor[smallest])
        smallest = r;
    if(smallest != u){
        int aux = h->valor[u];
        h->valor[u] = h->valor[smallest];
        h->valor[smallest] = aux;
        heapify(h, smallest);
    }
}

void build_heap(HEAP * h){
    int n = h->tamanho;
    for(int i = (n/2) - 1; i >= 0; i--){
        heapify(h, i);
    }
}

int extract_min(HEAP * h){
    int n = h->tamanho;
    if(n < 1)
        return -inf;
    else{
        int min = h->valor[0];
        h->valor[0] = h->valor[n-1];
        (h->tamanho)--;
        heapify(h, 0);
        return min;
    }
}


int main(){
    FILE *file;
    file = fopen("teste.txt", "r");
    
    int qt_arestas, qt_vertices, a, b, c;
    fscanf(file,"%d %d", &qt_vertices, &qt_arestas);

    HEAP h;

    inicializacao(&h, qt_vertices);

    build_heap(&h);

    grafo.assign(qt_vertices+1,vector<pair<int,int> >());

    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %d",&a,&b,&c);
        grafo[a].push_back(make_pair(b,c));
        grafo[b].push_back(make_pair(a,c));
    }

    while(h.tamanho > 0){
        int u = extract_min(&h);
        if(u != -inf){
            //nao ta funcionando ainda ate o fim do if(u != inf)
            cout<<u<<endl;
        }
    }

    //funcionando
    /*
    for(int i=0;i<=qt_vertices;i++){
        cout<<"i: "<<i<<endl;
        for(int u=0;u<grafo[i].size();u++){
            cout<<grafo[i][u].first<<" "<<grafo[i][u].second<<endl;
        }
    }
    */
    return 0;
}