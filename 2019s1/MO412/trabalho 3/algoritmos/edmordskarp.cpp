#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
#define V 10000

int residual[V][V], p[V], flow[V][V], dist[V];
vector<vector<int> > grafo;

bool bfs(int s, int t){

    memset(p, -1, sizeof p);
    memset(dist, -1, sizeof dist);

    queue<int>fila;
    fila.push(s);
    dist[s] = 0;

    while(!fila.empty()){
        int u = fila.front();
        fila.pop();
        for(int i = 0 ; i < grafo[u].size(); i++){
            int v = grafo[u][i];
            if(dist[v] == -1 && residual[u][v] != 0){
                dist[v] = dist[u] + 1;
                fila.push(v);
                p[v] = u;
            }
        }
    }
    return dist[t] != -1;
}

int main(){

    int n, m, s, t, a, b, peso, v;

	FILE *file;
    file = fopen("teste.txt","r");

    fscanf(file,"%d",&n);
    fscanf(file,"%d",&m);
    fscanf(file,"%d",&s);
    fscanf(file,"%d",&t);

    grafo.assign(n, vector<int>());

    vector<int> saidaA;
    vector<int> saidaB;

    memset(flow, 0, sizeof flow);
    memset(residual, 0, sizeof residual);

    for(int i=0;i<m;i++){
    	fscanf(file,"%d %d %d", &a, &b, &peso);
    	flow[a][b] = peso;
        residual[a][b] = peso;
        saidaA.push_back(a);
        saidaB.push_back(b);
        grafo[a].push_back(b);
        grafo[b].push_back(a);
    }

    while(bfs(s, t)){

    	if(p[t] == -1)
    		break;

    	int f = inf;

    	for(v = t;v != s; v = p[v]){
            int aux = p[v];
            cout<<aux<<endl;
    		f = min(f, residual[aux][v]);
    	}

    	cout<<endl<<f<<endl<<endl;

    	for(v = t;v != s;v = p[v]){
            int aux = p[v];
            residual[aux][v] -= f;
            residual[v][aux] += f;
        }
    }

    for(int i=0;i<m;i++)
        cout<<saidaA[i]<<" "<<saidaB[i]<<" "<<flow[saidaA[i]][saidaB[i]] - residual[saidaA[i]][saidaB[i]]<<endl;

	return 0;
}