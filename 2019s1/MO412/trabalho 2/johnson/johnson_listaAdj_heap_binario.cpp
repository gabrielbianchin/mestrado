#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

vector<vector<pair<int, float> > > grafo;
int * c;

typedef struct{
    float * d;
    int * pai;
}NO;

typedef struct{
    float valor;
    int parent;
    int key;
} NOD;

typedef struct {
    NOD * nos;
    int tamanho = -1;
}HEAP;

void inicializacaoD(HEAP * h, int vertices, int s){
    NOD aux;
    h->nos = (NOD *) malloc(sizeof(NOD)*(vertices));
    c = (int *) malloc(sizeof(int)*(vertices));
    for(int i=0;i<vertices;i++){
        c[i] = i;
        if(i != s)
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
    NOD aux = h->nos[u];
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

NOD extract_min(HEAP * h){
    int n = h->tamanho;
    if(n < 0){
        NOD aux = {-1, -1, -1};
        return aux;
    }
    else if(n == 0){
        NOD aux = h->nos[0];
        (h->tamanho)--;
        return aux;
    }
    else{
        NOD aux = h->nos[0];
        swap(h, 0, n);
        (h->tamanho)--;
        heapify(h, c[(h->nos[0]).key]);
        return aux;
    }
}

void relax(HEAP * h, int v, float peso){
    (h->nos[v]).valor = peso;
    while(v > 0 && (h->nos[v]).valor < (h->nos[parent(v)]).valor){
        swap(h, v, parent(v));
        v = parent(v);
    }
}

void backtracking(int vet[], int u){
    if(vet[u]==-1){
        cout<<u-1<<" ";
        return;
    }
    int pai = vet[u];
    backtracking(vet, pai);
    cout<<u-1<<" ";
}

void inicializacao(NO * no, int vertices){
    no->d = (float *)malloc(sizeof(float)*(vertices));
    no->pai = (int *)malloc(sizeof(int)*(vertices));
    for(int i=0;i<vertices;i++){
        no->d[i] = inf;
        no->pai[i] = -1;
    }
    no->d[0] = 0;
}

void relax1(NO * no, int u, int v, float peso){
    if(no->d[v] > (no->d[u] + peso)){
        no->d[v] = no->d[u] + peso;
        no->pai[v] = u;
    }
}

int main(){

	FILE *file;
    file = fopen("teste.txt","r");

	int qt_vertices, qt_arestas, a, b,s;
	float p;
    fscanf(file,"%d",&qt_vertices);
    fscanf(file,"%d",&qt_arestas);
    fscanf(file, "%d", &s);

    qt_vertices++;
    //cout<<qt_vertices<<endl;

	grafo.assign(qt_vertices, vector<pair<int, float> >());

	for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %f",&a,&b,&p);
        grafo[a+1].push_back(make_pair(b+1,p));
    }

    for(int i=1;i<=qt_vertices;i++)
    	grafo[0].push_back(make_pair(i,0.0));

    NO no;

    inicializacao(&no, qt_vertices);

	for(int k=0;k<qt_vertices;k++){
	    for(int i=0;i<qt_vertices;i++){
	        for(int j=0;j<grafo[i].size();j++){
	            int u = grafo[i][j].first;
	            float peso = grafo[i][j].second;
                    if(no.d[u] > (no.d[i] + peso)){
                        no.d[u] = no.d[i] + peso;
                        no.pai[u] = i;
                    }
                //relax1(&no, i, u, peso);
	        }
	    }
	}

	bool ciclo = false;

    for(int i=0;i<qt_vertices;i++){
        for(int j=0;j<grafo[i].size();j++){
            int u = grafo[i][j].first;
            float peso = grafo[i][j].second;
            if(no.d[u] > (no.d[i] + peso))
                ciclo = true;
        }
    }

    if(ciclo){
        cout<<"Grafo contem um ciclo negativo"<<endl;
    }

     else{
         for(int i=1;i<qt_vertices;i++){
             for(int j=0;j<grafo[i].size();j++){
                 int u = grafo[i][j].first;
                 float peso = grafo[i][j].second;
                 grafo[i][j].second = grafo[i][j].second + no.d[i] - no.d[u];
             }
         }



         for(int qt=1;qt<qt_vertices;qt++){
             HEAP h;

            inicializacaoD(&h, qt_vertices, qt);

            build_heap(&h);

            float saida[qt_vertices];
            int saidapais[qt_vertices];
            for(int i=1;i<qt_vertices;i++){
                saidapais[i] = -1;
                saida[i] = inf;
            }

            saida[qt] = 0;

            while(h.tamanho >= 0){
                 NOD w = extract_min(&h);
                 if(w.valor != -1){
                     int v = w.key;
                     for(int i=0;i<grafo[v].size();i++){
                         int u = grafo[v][i].first;
                         float peso = grafo[v][i].second;
                         if(saida[v] + peso < (h.nos[c[u]]).valor){
                             (h.nos[c[u]]).parent = v;
                             saida[u] = saida[v] + peso;
                             saidapais[u] = v;
                             relax(&h, c[u], (w.valor + peso));
                         }
                     }
                     c[v] = -1;
                 }
             }

             for(int i=1;i<qt_vertices;i++){
                if(saida[i] == inf)
                     cout<<"INFINITO "<<qt-1<<" "<<i-1<<endl;
                 else{
                     cout<<saida[i]<<" ";
                     backtracking(saidapais, i);
                     cout<<endl;
                 }
             }
	    }
     }

	return 0;
}