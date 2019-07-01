#include <iostream>
#include <climits>
#include <queue>
#include <list>

using namespace std;

#define N 1000

typedef struct Node Node;
typedef struct Aresta Aresta;


struct Aresta {
   int u;
   int v;
   float peso;
};    

struct Node{
	float key;
	Node * pi;
	int value;
	bool flag;

   bool operator < (const Node &o) const
   {
      return key < o.key;
   }
};

list<Aresta> Adj[N];
vector<Aresta> arestas;
Node vetorNode[N];

void inicializacao(int src, int sink, float peso){
    Adj[src].push_back((Aresta){src,sink,peso});
}

void backtracking(int u){
    if(vetorNode[u].pi == NULL){
        cout<<u<<" ";
        return;
    }
    int pai = vetorNode[u].pi->value;
    backtracking(pai);
    cout<<u<<" ";
}

void dijkstra(int r, int qt_vertices){
   float sp = 0.0;
	vetorNode[r].key = 0.0;

   priority_queue <Node*> Q;
   
   Q.push(&vetorNode[r]);
   Node* u = new Node();
   
   while(!Q.empty()){
      Node* u = new Node();
		u = Q.top();
      u->flag = true;
      Q.pop();
      
      list<Aresta>::iterator it;
      for(it = Adj[u->value].begin(); it != Adj[u->value].end(); it++){
         if(it->peso + u->key < vetorNode[it->v].key){
            vetorNode[it->v].pi = u;
            vetorNode[it->v].key = it->peso + u->key;
            if (!vetorNode[it->v].flag){
               Q.push(&vetorNode[it->v]);   
            }
         }  
      }
   }


   for (int i = 0; i < qt_vertices; i++){
      cout<<vetorNode[i].key<<" ";
      backtracking(i);
      cout<<endl;      
   }
}


int main(){
   FILE *file;
   
   file = fopen("teste.txt", "r");
   int qt_arestas, qt_vertices, a, b, s;
   float p,sh;
   //cout<<"aaa";
   fscanf(file,"%d", &qt_vertices);
   fscanf(file,"%d", &qt_arestas);
   fscanf(file, "%d", &s);

   for(int i = 0; i < qt_vertices; i++){
      vetorNode[i].key = INT_MAX;
      vetorNode[i].pi = NULL;
      vetorNode[i].value = i;
      vetorNode[i].flag = false;
   }
   
   for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %f",&a,&b,&p);
        inicializacao(a,b,p);
   }
   
   dijkstra(s,qt_vertices);
   
   
	return 0;
}