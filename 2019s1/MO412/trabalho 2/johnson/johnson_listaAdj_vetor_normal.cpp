#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

vector<vector<pair<int, float> > > grafo;
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

void backtracking(int vet[], int u){
    if(vet[u] == -1){
        cout<<u-1<<" ";
        return;
    }
    backtracking(vet, vet[u]);
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

	int qt_vertices, qt_arestas, a, b, s;
	float p;
    fscanf(file,"%d",&qt_vertices);
    fscanf(file,"%d",&qt_arestas);
    fscanf(file,"%d",&s);

    qt_vertices++;

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
        for(int i=1;i<qt_vertices;i++){
            for(int j=0;j<grafo[i].size();j++){
                int u = grafo[i][j].first;
                float peso = grafo[i][j].second;
                grafo[i][j].second = grafo[i][j].second + no.d[i] - no.d[u];
            }
        }

        for(int qt=1;qt<qt_vertices;qt++){
            FILA f;

            inicializacaoD(&f, qt_vertices, qt);

            float saida[qt_vertices];
             int saidapais[qt_vertices];
            for(int i=1;i<qt_vertices;i++){
                saidapais[i] = -1;
                saida[i] = inf;
            }

            saida[qt] = 0;

            while(true){
                NOD w = extract_min(&f, qt_vertices);
                cout<<w.key-1<<" "<<w.valor<<endl;
                if(w.valor != -1){
                    int v = w.key;
                    float pesov = w.valor;
                    for(int i=0;i<grafo[v].size();i++){
                        int u = grafo[v][i].first;
                        float peso = grafo[v][i].second;
  
                        if(saida[u] > saida[v] + peso){
                            saida[u] = saida[v] + peso;
                            saidapais[u] = v;
                            (f.nos[u]).parent = v;
                            relax(&f, u, saida[v] + peso);
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
                    cout<<saida[i]<<" ";
                    backtracking(saidapais, i);
                    cout<<endl;
                }
            }
	   }
    }

	return 0;
}