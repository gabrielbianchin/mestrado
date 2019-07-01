#include <bits/stdc++.h>
using namespace std;

int p[10000], a, b, size[10000];
float peso;

struct Aresta{
    int a,b;
    float peso;
    bool operator < (const Aresta& q) const
    {
        return peso < q.peso;
    }
};

void makeset(int u){
    p[u] = u;
    size[u] = 1;
}

//findset sem path compression
int findset_normal(int u){
    if(p[u] != u)
        return findset_normal(p[u]);
    return u;
}

//findset com path compression
int findset_pathcompression(int u){
    if(p[u] != u)
        p[u] = findset_pathcompression(p[u]);
    return p[u];
}

void link(int u, int v){
    if(size[u] >= size[v]){
        p[v] = u;
        size[u] += size[v];
    }
    else{
        p[u] = v;
        size[v] += size[u];
    }
}

//unionset sem path compression
void unionset(int u, int v){
    link(findset_normal(u), findset_normal(v));
}

vector<Aresta> counting_sort( const vector<Aresta> &op ){
   if ( op.empty() )
      return vector<Aresta> {};

   auto min = *std::min_element( op.begin(), op.end() );
   auto max = *std::max_element( op.begin(), op.end() );

   vector<float> contagem( max.peso - min.peso + 1, 0 );
   for ( auto it = op.begin(); it != op.end(); it++ ){
      cout<<"hh "<<it->peso - min.peso <<endl;

        contagem[it->peso - min.peso]++;
   }

   partial_sum( contagem.begin(), contagem.end(), contagem.begin() );

   vector<Aresta> ordenado( op.size() );
   for ( auto it2 = op.rbegin(); it2 != op.rend(); it2++ ){
      ordenado[contagem[ it2->peso - min.peso ]--] = *it2;
   }

   return ordenado;
}

vector<Aresta> shellSort(vector<Aresta> arr, int n){ 
    // Start with a big gap, then reduce the gap 
    for (int gap = n/2; gap > 0; gap /= 2){ 
        for (int i = gap; i < n; i += 1){ 
            Aresta temp = arr[i]; 
            
            int j;             
            for (j = i; j >= gap && arr[j - gap].peso > temp.peso; j -= gap) 
                arr[j] = arr[j - gap]; 
              
            arr[j] = temp; 
        } 
    } 
    return arr; 
} 

int main(){

    //abrindo o arquivo
    FILE *file;
    file = fopen("testef.txt", "r");
    
    //declaracao do grafo e do conjunto A
    vector <Aresta> grafo;
    vector <Aresta> A;

    int qt_vertices, qt_arestas,n;
    float mst = 0.0;

    //leitura da quantidade de vertices e quantidade de arestas
    fscanf(file,"%d %d",&qt_vertices, &qt_arestas);
    
    //para um grafo não direcionado
    n = 2*qt_arestas;

    //makeset dos vertices
    for(int i=1;i<=qt_vertices;i++){
        makeset(i);
    }
    
    //criacao do grafo
    for(int i=0;i<qt_arestas;i++){
        fscanf(file,"%d %d %f",&a, &b, &peso);
        Aresta aux={a,b,peso};
        grafo.push_back(aux);
        aux={b,a,peso};
        grafo.push_back(aux);
    }

    //ordenacao das arestas
    grafo = shellSort(grafo, n);

    for(int i=0;i<qt_arestas;i++){
        int a=grafo[i].a,b=grafo[i].b;
        float p=grafo[i].peso;
        if(findset_normal(a)!=findset_normal(b)){
            unionset(a,b);
            Aresta aux = {a, b, p};
            A.push_back(aux);
            mst+=p;
        }
    }
    cout<<mst<<endl;
    for(int i=0;i<A.size();i++){
        cout<<A[i].a<<" "<<A[i].b<<" "<<A[i].peso<<endl;
    }
    return 0;
}
