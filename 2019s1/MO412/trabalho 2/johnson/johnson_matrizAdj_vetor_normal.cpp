#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

int * c;

typedef struct{
    float * d;
    int * pai;
}NO;

struct NOD{
    float valor;
    int parent;
    int usado;
    int key;
};

typedef struct{
    NOD * nos;
}FILA;

void inicializacaoD(FILA * f, int vertices, int s){
    NOD aux;
    f->nos = (NOD *) malloc(sizeof(NOD)*(vertices));
    for(int i=0;i<vertices;i++){
        if(i != s)
            aux.valor = inf;
        else
            aux.valor = 0;
        aux.parent = -1;
        aux.usado = 1;
        aux.key = i;
        f->nos[i] = aux;
    }
}

NOD extract_min(FILA * f, int vertices){
    int menor = inf, indice = -1;
    for(int i=0;i<vertices;i++){
        if((f->nos[i]).usado != 0 && (f->nos[i]).valor < menor){
            menor = (f->nos[i]).valor;
            indice = i;
        }
    }
    if(indice != -1){
        (f->nos[indice]).usado = 0;
        return (f->nos[indice]);
    }
    else{
        NOD aux;
        aux.usado = -1;
        aux.key = -1;
        aux.parent = -1;
        aux.valor = -1;
        return aux;
    }
}

void relax(FILA * f, int v, float peso){
    (f->nos[v]).valor = peso;
}

void backtracking(FILA f, int u){
    if((f.nos[u]).parent == -1){
        cout<<u-1<<" ";
        return;
    }
    int pai = (f.nos[u]).parent;
    backtracking(f, pai);
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

void relax(NO * no, int u, int v, float peso){
    if(no->d[v] > (no->d[u] + peso)){
        no->d[v] = no->d[u] + peso;
        no->pai[v] = u;
    }
}

int main(){

    FILE *file;
    file = fopen("teste.txt","r");

    int qt_vertices, qt_arestas, a, b;
    float p;
    fscanf(file,"%d",&qt_vertices);
    fscanf(file,"%d",&qt_arestas);

    qt_vertices++;

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
        grafo[a+1][b+1] = p;
    }

    for(int i=1;i<=qt_vertices;i++)
        grafo[0][i] = 0;

    NO no;

    inicializacao(&no, qt_vertices);

    for(int k=0;k<qt_vertices;k++){
        for(int i=0;i<qt_vertices;i++){
            for(int j=0;j<qt_vertices;j++){
                float peso = grafo[i][j];
                relax(&no, i, j, peso);
            }
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

    if(ciclo){
        cout<<"Grafo contem um ciclo negativo"<<endl;
    }

    else{
        for(int i=1;i<qt_vertices;i++){
            for(int j=1;j<qt_vertices;j++){
                grafo[i][j] = grafo[i][j] + no.d[i] - no.d[j];
            }
        }

        for(int qt=1;qt<qt_vertices;qt++){
            FILA f;

            inicializacaoD(&f, qt_vertices, qt);

            while(true){
                NOD w = extract_min(&f, qt_vertices);
                if(w.valor != -1){
                    int v = w.key;
                    float pesov = w.valor;
                    for(int i=1;i<qt_vertices;i++){
                        float peso = grafo[v][i];
                        if((f.nos[i].valor) > (pesov + peso)){
                            (f.nos[i]).parent = v;
                            relax(&f, i, pesov + peso);
                        }
                    }
                    (f.nos[v]).usado = 0;
                }
                else
                    break;
            }

            for(int i=1;i<qt_vertices;i++){

                if((f.nos[i]).valor == inf)
                    cout<<"INFINITO "<<qt-1<<" "<<i-1<<endl;
                else{
                    cout<<(f.nos[i]).valor<<" ";
                    backtracking(f, i);
                    cout<<endl;
                }
            }
       }
    }

    return 0;
}