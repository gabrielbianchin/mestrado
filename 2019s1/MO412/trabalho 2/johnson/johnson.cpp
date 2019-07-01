#include <bits/stdc++.h>

using namespace std;

#define inf 0x3f3f3f3f

int main(){
	int n, m, u, v, s;
	float w;

	FILE *file;
	file = fopen("teste.txt","r");

	fscanf(file,"%d",&n);
    fscanf(file,"%d",&m);
    fscanf(file,"%d",&s);

    float grafo[n][n];

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i == j)
                grafo[i][j] = 0;
            else
                grafo[i][j] = inf;
        }
    }

    for(int i = 0; i < m; i++){
        fscanf(file,"%d %d %f",&u,&v,&w);
        grafo[u][v] = w;
    }

    for(int k = 0; k < n; k++){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                grafo[i][j] = min(grafo[i][j], grafo[i][k] + grafo[k][j]);
            }
        }
    }

    for(int i = 0; i < n; i++){
    	for(int j = 0; j < n; j++){
    		printf("%f ", grafo[i][j]);
    	}
    	printf("\n");
    }

	return 0;
}