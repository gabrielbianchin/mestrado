#include <bits/stdc++.h>
using namespace std;

int p[10000],a,b,peso;

struct Aresta{
    int a,b,peso;
    bool operator < (const Aresta& q) const
    {
        return peso < q.peso;
    }
};

int findset(int u){
    if(p[u]==u)
        return u;
    return p[u]=findset(p[u]);
}

void unionset(int a, int b){
    int x=findset(a),y=findset(b);
    p[y]=x;
}

int main(){
    FILE *file;
    file = fopen("teste.txt", "r");
    vector <Aresta> grafo;
    int qt_vertices,qt_arestas,mst=0;
    fscanf(file,"%d %d",&qt_vertices,&qt_arestas);
    for(int i=1;i<=qt_vertices;i++){
        p[i]=i;
    }
    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %d",&a,&b,&peso);
        Aresta aux={a,b,peso};
        grafo.push_back(aux);
    }
    sort(grafo.begin(),grafo.end());
    for(int i=0;i<qt_arestas;i++){
        int a=grafo[i].a,b=grafo[i].b,p=grafo[i].peso;
        if(findset(a)!=findset(b)){
            unionset(a,b);
            mst+=p;
        }
    }
    cout<<"Valor da mst = " <<mst<<endl;
    return 0;
}
