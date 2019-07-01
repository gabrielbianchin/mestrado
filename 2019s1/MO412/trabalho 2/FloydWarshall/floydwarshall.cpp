#include<bits/stdc++.h>
#define inf 0x3f3f3f3f
using namespace std;

int main(){
	FILE *file;
    file = fopen("teste.txt","r");

    int vertices, arestas,s;
    int a,b;
    float p;

    fscanf(file,"%d",&vertices);
    fscanf(file,"%d",&arestas);
    fscanf(file,"%d",&s);

    float pesos[vertices][vertices];
    int caminho[vertices][vertices];

    for(int i=0;i<vertices;i++){
    	for(int j=0;j<vertices;j++){
    		if(i == j)
    			pesos[i][j] = 0;
    		else
    			pesos[i][j] = inf;
    		caminho[i][j] = inf;
    	}
    }

    for(int i=0;i<arestas;i++){
    	fscanf(file,"%d %d %f",&a,&b,&p);
    	pesos[a][b] = p;
    	caminho[a][b] = b;
    }

    for(int k=0;k<vertices;k++){
    	for(int i=0;i<vertices;i++){
    		for(int j=0;j<vertices;j++){
    			if((pesos[i][k] + pesos[k][j]) < pesos[i][j]){
    				pesos[i][j] = pesos[i][k] + pesos[k][j];
    				caminho[i][j] = caminho[i][k];
    			}
    		}
    	}
    }

    for(int i=0;i<vertices;i++){
    	for(int j=0;j<vertices;j++){
    		if(i == j)
    			cout<<pesos[i][j]<<" "<<i<<endl;
    		else if(caminho[i][j] == inf || pesos[i][j] == inf)
    			cout<<"Nao existe caminho de "<<i<<" para "<<j<<endl;
    		else{
	    		cout<<pesos[i][j]<<" "<<i<<" ";
	    		int a = i, b = j;
	    		while(a != b){
	    			a = caminho[a][b];
	    			if(a == b)
	    				cout<<a<<endl;
	    			else
	    				cout<<a<<" ";
	    		}
    		}
    	}
    }

	return 0;
}