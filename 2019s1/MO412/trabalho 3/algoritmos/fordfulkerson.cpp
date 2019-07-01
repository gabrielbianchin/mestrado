#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
#define V 10000

int residual[V][V], p[V], flow[V][V];
vector<vector<int> > grafo;

void dfs(int u){
    for(int i=0;i<grafo[u].size();i++){
        int v = grafo[u][i];
        if(p[v] == -1 && residual[u][v] != 0){
            p[v] = u;
            dfs(v);
        }
    }
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

    while(true){

    	memset(p, -1, sizeof p);

    	dfs(s);

    	if(p[t] == -1)
    		break;

    	int f = inf;

    	for(v = t;v != s; v = p[v]){
            int aux = p[v];
    		f = min(f, residual[aux][v]);
    	}

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