#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

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
    fscanf(file,"%d",&qt_vertices);
    fscanf(file,"%d",&qt_arestas);
    fscanf(file,"%d",&s);

    float grafo[qt_vertices][qt_vertices];

    for(int i=0;i<qt_vertices;i++){
        for(int j=0;j<qt_vertices;j++){
            if(i == j)
                grafo[i][j] = 0;
            else
                grafo[i][j] = inf;
        }
    }

    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %f",&a,&b,&p);
        grafo[a][b] = p;
    }

    NO no;

    inicializacao(&no,qt_vertices,s);

    //primeiro for - O(V)
    for(int k=0;k<qt_vertices;k++){
        //verifica toda matriz de adj O(V²)
        for(int i=0;i<qt_vertices;i++){
            for(int j=0;j<qt_vertices;j++)
                relax(&no, i, j, grafo[i][j]);
        }
    }

    bool ciclo = false;

    for(int i=0;i<qt_vertices;i++){
        for(int j=0;j<qt_vertices;j++){
            float peso = grafo[i][j];
            if(no.d[j] > (no.d[i] + peso))
                ciclo = true;
        }
    }

    if(ciclo)
        cout<<"Grafo contem um ciclo negativo"<<endl;

    else{
        for(int i=0;i<qt_vertices;i++){
            cout<<no.d[i]<<" ";
            backtracking(no, i);
            cout<<endl;
        }
    }

    return 0;
}

/*
0.1 0 2
10.2 1 0 2
0 2
1.8 3 0 2
3.2 4 0 2
3.2 5 0 2

0.1 2 0 
10.2 2 0 1 
0 2 
1.8 2 0 3 
3.2 2 0 4 
3.2 2 0 5 

*/