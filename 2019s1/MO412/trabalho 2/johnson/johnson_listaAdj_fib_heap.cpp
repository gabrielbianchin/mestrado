#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

typedef struct Edge_{
    int a;
    int b;
    float weight;
} Edge_;

typedef struct{
    float * d;
    int * pai;
}NO;

typedef struct NO_{
    NO_ * parent;
    float key;
    int degree;
    bool marked;
    NO_ * child;
    NO_ * left;
    NO_ * right;
    vector <Edge_ *> connexions;
    Edge_ * best;
    int i;
    bool gone;
} NO_;

vector < NO_ * > sommets;
NO_ * A[100000];

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

void initNO_(NO_ * n, float key, int i){
    n->parent = NULL;
    n->key = key;
    n->child = NULL;
    n->degree = 0;
    n->marked = false;
    n->left = n;
    n->right = n;
    n->i = i;
    n->best = NULL;
    n->gone = false;
}

NO_ * merge(NO_ *a, NO_ *b){
    if(a == NULL && b == NULL)
        return NULL;
    if(a == NULL && b != NULL)
        return b;
    if(a != NULL && b == NULL)
        return a;

    NO_ * leftA = a->left;
    NO_ * leftB = b->left;
    NO_ * aa = a;
    NO_ * bb = b;

    aa->left = leftB;
    leftB->right = aa;
    leftA->right = bb;
    bb->left = leftA;

    if(a->key < b->key)
        return a;
    else
        return b;
}

static void mergeTree( NO_ * x, NO_ * y){
        NO_ * prev_y = y->left;
        NO_ * next_y = y->right;

        prev_y->right = next_y;
        next_y->left = prev_y;

        if(x->child != NULL){
            y->parent = x;
            y->left = x->child->left;
            y->right = x->child;
            y->marked = false;
            x->child->left = y;
            y->left->right = y;
            x->child = y;
            x->degree++;
        }
        else{
            x->child = y;
            x->degree++;
            y->parent = x;
            y->left = y;
            y->right = y;
        }
}

static NO_ * consolidate(NO_ * heap){
    memset(A, 0, sizeof(int) * 100);
    NO_ * depart = heap;
    NO_ * x = heap;
    NO_ * y;
    int d;
    do{
        if(x->key == float(inf))
            goto next;
        d = x->degree;
        while(A[d] != NULL){
            y = A[d];
            NO_ * tmp = y;
            A[d] = NULL;
            if(x == y)
                goto fin;
            if(x->key > y->key){
                NO_ * tmp = x;
                x = y;
                y = tmp;
            }
            mergeTree(x, y);
            d++;
        }
        A[d] = x;
next:
        x = x->right;
    }while (true);

fin:
    depart = x;
    NO_ * min = x;
    do{
        if (x->key < min->key)
            min = x;
        x = x->right;
    }while(x != depart);
    
    return min;
}

NO_* extract_min_fib(NO_ * heap){
    if (heap == NULL)
        return NULL;
    NO_ * min = heap;
    NO_ *prev = min->left;
    NO_ *next = min->right;
    prev->right = next;
    next->left = prev;
    if(heap == prev && heap->child == NULL)
        return NULL;
    A[min->degree] = NULL;

    NO_ * childd = min->child;
    
    if(childd != NULL){
        do{
            childd->parent = NULL;
            childd = childd->right;
        }while(childd != min->child);
    }

    NO_ * aux = min->right;
    if(min == aux) 
        aux = NULL;

    aux = merge(childd, aux);
    NO_ * newmin = consolidate(aux);

    min->left = NULL;
    min->right = NULL;

    return newmin;
}


void Cut(NO_ * heap, NO_ * x, NO_ * y){
    y->degree--;

    if(y->child == x){
        if(x->left !=x)
            y->child = x->left;
        else if(x->left == x)
            y->child = NULL;
    }

    NO_ *prev = x->left;
    NO_ *next = x->right;
    prev->right = next;
    next->left = prev;

    x->left = x;
    x->right = x;
    x->parent = NULL;
    x->marked = false;

    merge(heap, x);
}



void CutCascade(NO_ * heap, NO_ * y){
    NO_ * z = y->parent;
    if(z != NULL){
        if(y->marked == false)
            y->marked = true;

        else{
            Cut(heap, y, z);
            CutCascade(heap, z);
        }
    }
}

NO_ * relax(NO_ * heap, NO_ * x, float newValue){
	if(newValue > x->key){
        return heap;
    }

    x->key = newValue;

    NO_ * y = x->parent;

    if (y != NULL && x->key < y->key) {
        Cut(heap, x, y);
        CutCascade(heap, y);
    }

    if (x->key < heap->key)
        return x;

	return heap;
}

void backtracking(int saidaA[], int u){
	int pai = saidaA[u];
    if(pai == -1){
        cout<<u<<" ";
        return;
    }
    pai = saidaA[u];
    backtracking(saidaA, pai);
    cout<<u<<" ";
}

int main(){
    FILE *file;
    file = fopen("teste.txt", "r");
    
    int qt_arestas, qt_vertices, k, l, s, a, b;
    float p, mst = 0.0;

    NO_ * min = NULL;
    
    fscanf(file,"%d", &qt_vertices);
    fscanf(file,"%d", &qt_arestas);
    fscanf(file, "%d", &s);

    qt_vertices++;

    vector<vector<pair<int, float> > > grafo;
    grafo.assign(qt_vertices, vector<pair<int, float> >());

    sommets.resize(qt_vertices-1);

    for(int i=0;i<qt_arestas;i++){
        fscanf(file, "%d %d %f", &a, &b, &p);
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

        for(int k=1;k<qt_vertices;k++){

            for(int i=1;i<qt_vertices;i++){
                NO_ * no = new NO_;
                initNO_(no, float(inf), i-1);
                sommets[i-1] = no;
                min = merge(min, no);
            }
            for(int i=1;i<qt_vertices;i++){
                for(int j=0;j<grafo[i].size();j++){
                    int u = grafo[i][j].first;
                    float peso = grafo[i][j].second;
                    Edge_ * Edge_Lue = new Edge_;
                    Edge_Lue->a = i-1;
                    Edge_Lue->b = u-1;
                    Edge_Lue->weight = peso + no.d[i] - no.d[u];
                    sommets[i-1]->connexions.push_back(Edge_Lue);
                }
            }

            NO_ * current = sommets[k-1];
            NO_ * destination;

            current->key = -500.0;
            min = current;

            int saidaA[qt_vertices];
            float saidaP[qt_vertices];
            
            for(int i=1;i<qt_vertices;i++){
            	saidaP[i-1] = inf;
            	saidaA[i-1] = -1;
            }

            saidaP[k-1] = 0;

            while (!current->gone){
                for(int i = 0; i < current->connexions.size(); i++){
                    Edge_ * connexionCurrent = current->connexions[i];
                    int s1 = connexionCurrent->a;
                    int s2 = connexionCurrent->b;
                    float weightConnexion = connexionCurrent->weight + saidaP[current->i];

                    int sommetDestination = s1 != current->i ? s1 : s2;
                    destination = sommets[sommetDestination];

                    if(!destination->gone){
                        if(weightConnexion < destination->key){
                        	saidaP[destination->i] = weightConnexion;
                        	saidaA[destination->i] = current->i;
                            destination->best = connexionCurrent;
                            min = relax(current, destination, weightConnexion);
                        }
                    }
                }
                current->gone = true;
                min = extract_min_fib(current);
                current = min;
                if (min == NULL)
                    break;
            }
            for(int i=1;i<qt_vertices;i++){
            	cout<<saidaP[i-1]<<" ";
            	backtracking(saidaA, i-1);
            	cout<<endl;
            }
        }
    }
    return 0;
}