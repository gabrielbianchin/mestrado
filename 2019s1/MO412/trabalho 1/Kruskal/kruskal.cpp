#include <bits/stdc++.h>
using namespace std;

int p[10000], a, b, peso, size[10000];

struct Aresta{
    int a,b,peso;
    bool operator < (const Aresta& q) const
    {
        return peso < q.peso;
    }
};

void makeset(int u){
    p[u] = u;
    size[u] = 1;
}

//findset sem path compression
int findset_normal(int u){
    if(p[u] != u)
        return findset_normal(p[u]);
    return u;
}

//findset com path compression
int findset_pathcompression(int u){
    if(p[u] != u)
        p[u] = findset_pathcompression(p[u]);
    return p[u];
}

void link(int u, int v){
    if(size[u] >= size[v]){
        p[v] = u;
        size[u] += size[v];
    }
    else{
        p[u] = v;
        size[v] += size[u];
    }
}

//unionset sem path compression
void unionset(int u, int v){
    link(findset_normal(u), findset_normal(v));
}

int main(){
    //abrindo o arquivo
    FILE *file;
    file = fopen("teste.txt", "r");
    
    //declaracao do grafo e do conjunto A
    vector <Aresta> grafo;
    vector <Aresta> A;

    int qt_vertices, qt_arestas, mst = 0;

    //leitura da quantidade de vertices e quantidade de arestas
    fscanf(file,"%d %d",&qt_vertices, &qt_arestas);
    
    //makeset dos vertices
    for(int i=1;i<=qt_vertices;i++){
        makeset(i);
    }
    
    //criacao do grafo
    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %d",&a, &b, &peso);
        Aresta aux={a,b,peso};
        grafo.push_back(aux);
        aux={b,a,peso};
        grafo.push_back(aux);
    }

    //ordenacao das arestas
    sort(grafo.begin(),grafo.end());
    

    for(int i=0;i<qt_arestas;i++){
        int a=grafo[i].a,b=grafo[i].b,p=grafo[i].peso;
        if(findset_normal(a)!=findset_normal(b)){
            unionset(a,b);
            Aresta aux = {a, b, p};
            A.push_back(aux);
            mst+=p;
        }
    }
    cout<<mst<<endl;
    for(int i=0;i<A.size();i++){
        cout<<A[i].a<<" "<<A[i].b<<" "<<A[i].peso<<endl;
    }
    return 0;
}
