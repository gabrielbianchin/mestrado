#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

vector <vector<pair<int,float> > > grafo;

typedef struct{
    float * d;
    int * pai;
}NO;

void inicializacao(NO * no, int vertices, int s){
    no->d = (float *)malloc(sizeof(float)*(vertices));
    no->pai = (int *)malloc(sizeof(int)*(vertices));
    for(int i=0;i<vertices;i++){
        no->d[i] = inf;
        no->pai[i] = -1;
    }
    no->d[s] = 0;
}

void relax(NO * no, int u, int v, float peso){
    if(no->d[v] > (no->d[u] + peso)){
        no->d[v] = no->d[u] + peso;
        no->pai[v] = u;
    }
}

void backtracking(NO no, int u){
    if(no.pai[u]==-1){
        cout<<u<<" ";
        return;
    }
    int pai = no.pai[u];
    backtracking(no, pai);
    cout<<u<<" ";
}

int main(){
    FILE *file;
    file = fopen("teste.txt","r");

    int qt_vertices, qt_arestas,s,a,b;
    float p;
    fscanf(file,"%d %d",&qt_vertices,&qt_arestas);
    fscanf(file,"%d",&s);

    grafo.assign(qt_vertices,vector<pair<int,float> >());

    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %f",&a,&b,&p);
        grafo[a].push_back(make_pair(b,p));
    }

    NO no;

    inicializacao(&no,qt_vertices,s);

    //primeiro for O(V)
	for(int k=0;k<qt_vertices;k++){
	    //segundo for ve a lista de adj de todos os vertices - O(E)
	    for(int i=0;i<qt_vertices;i++){
	        for(int j=0;j<grafo[i].size();j++){
	            int u = grafo[i][j].first;
	            float peso = grafo[i][j].second;
	            relax(&no, i, u, peso);
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
        for(int i=0;i<qt_vertices;i++){
            cout<<no.d[i]<<" ";
            backtracking(no, i);
            cout<<endl;
        }
    }

    return 0;
}