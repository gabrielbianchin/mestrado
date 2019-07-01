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
    Adj[sink].push_back((Aresta){sink,src,peso});
}

float AGM_PRIM(int r, int qt_vertices){
   float mst = 0.0;
	vetorNode[r].key = 0.0;

   priority_queue <Node*> Q;
   
   Q.push(&vetorNode[r]);
   Node* u = new Node();
   
   while(!Q.empty()){
      Node* u = new Node();
		
      u = Q.top();
		
      u->flag = false;

      Q.pop();
      
      list<Aresta>::iterator it;
		for(it = Adj[u->value].begin(); it != Adj[u->value].end(); it++){
         if(it->peso < vetorNode[it->v].key){
            vetorNode[it->v].pi = u;
            vetorNode[it->v].key = it->peso;
            Q.push(&vetorNode[it->v]);
         }
      }
   }


   for (int i = 0; i < qt_vertices; i++){
      mst += vetorNode[i].key;
   }
   
   return mst;
}


int main(){
   FILE *file;
   file = fopen("teste.txt", "r");
   int qt_arestas, qt_vertices, a, b;
   float p,mst;
   fscanf(file,"%d %d", &qt_vertices, &qt_arestas);

   for(int i = 0; i < qt_vertices; i++){
      vetorNode[i].key = INT_MAX;
      vetorNode[i].pi = NULL;
      vetorNode[i].value = i;
      vetorNode[i].flag = true;
   }

   for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %f",&a,&b,&p);
        inicializacao(a,b,p);
   }
   mst = AGM_PRIM(0,qt_vertices);
   
   cout<<"MST = "<< mst<<endl;

	return 0;
}