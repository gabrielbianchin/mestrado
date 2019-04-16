#include <bits/stdc++.h>
using namespace std;

vector <int> vis;
priority_queue<pair<int,int> > pq;
vector <vector<pair<int,int> > > grafo;

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
