#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

typedef struct vertice{
    int u;
    int chave;
}vertices;

void build_min_heap(int n, vertices Q[n], int tam_heap);
void min_heapify(int n, vertices Q[n], int i, int tam_heap);
vertices extract_min(int n, vertices Q[n], int *tam_heap);
void heap_increase_key(int n, vertices Q[n], int i, int chave);
int dijkstra(int n, vertices Q[n], int M[n][n]);

int main(){
    int n;
    int i, j;
    int custo = 0;

    FILE * arq;
    arq = fopen("dij10.txt", "r");

    if(arq == 0){
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }

    fscanf(arq, "%d", &n);

    int M[n][n]; /// Matriz de adjacencia

    for(i=0; i<n; i++){
        M[i][i] = 0;
        for(j=i+1; j<n; j++){
            fscanf(arq, "%d", &M[i][j]);
            M[j][i] = M[i][j];
        }
    }

    fclose(arq);

    vertices Q[n];

    /// Iniciando as chaves de cada vertice com valor "infinito"
    for(i=0; i<n; i++){
        Q[i].u = i;

        if(i==0){
            Q[i].chave = 0;
        }
        else{
            Q[i].chave = INT_MAX;
        }
    }

    /// Construindo um heap minimo
    build_min_heap(n,Q,n-1);

    /// Caminho Minimo
    custo = dijkstra(n,Q,M);
    printf("Custo = %d", custo);

    printf("\n");
    system("PAUSE");
    return 0;
}

void build_min_heap(int n, vertices Q[n], int tam_heap){
    int i;

    for(i=floor((n-1)/2); i>=0; i--){
        min_heapify(n, Q, i, tam_heap);
    }
}

void min_heapify(int n, vertices Q[n], int i, int tam_heap){
    int L, r;
    int menor;
    vertices aux;

    L = 2*i+1; /// Filho a esquerda do no i
    r = 2*i+2; /// Filho a direita do no i

    /// Verificando qual no tem o menor valor. O pai ou os seus filhos.
    if(L<=tam_heap && Q[L].chave<Q[i].chave){
        menor = L;
    }
    else{
        menor = i;
    }
    if(r<=tam_heap && Q[r].chave<Q[menor].chave){
        menor = r;
    }

    if(menor!=i){
        /// Troca dos elementos na pos 'i' com a pos 'menor'
        aux = Q[i];
        Q[i] = Q[menor];
        Q[menor] = aux;

        min_heapify(n, Q, menor, tam_heap);
    }
}

vertices extract_min(int n, vertices Q[n], int *tam_heap){
    vertices min;
    min = Q[0];

    Q[0] = Q[*tam_heap];
    *tam_heap -= 1;
    min_heapify(n,Q,0,*tam_heap);

    return min;
}

void heap_increase_key(int n, vertices Q[n], int i, int chave){
    int pai;
    vertices aux;

    /// No pai do vertice i
    if(i%2 != 0){
        pai = floor(i/2);
    }
    else{
        pai = i/2-1;
    }

    Q[i].chave = chave;

    while(i>0 && Q[pai].chave > Q[i].chave){
        /// trocando o pai com o filho
        aux = Q[i];
        Q[i] = Q[pai];
        Q[pai] = aux;

        i = pai;

        /// No pai do vertice i
        if(i%2 != 0){
            pai = floor(i/2);
        }
        else{
            pai = i/2-1;
        }
    }
}

int dijkstra(int n, vertices Q[n], int M[n][n]){
    int i, j, k;
    int tam_heap = n-1;
    int L[n][n-1]; /// Lista de adjacencia
    vertices aux;
    int v;
    int custo = 0;

    /// Criando a lista de adjacencia
    for(i=0; i<n; i++){
        k=0;
        for(j=0; j<n; j++){
            if(i!=j){
                L[i][k] = j;
                k = k+1;
            }
        }
    }

    while(tam_heap >= 0){
        aux = extract_min(n,Q,&tam_heap);

        if(aux.u==n-1){
            return aux.chave;
        }

        for(i=0; i<n-1; i++){
            v = L[aux.u][i];
            for(j=0; j<=tam_heap; j++){
                /// Verificando se o vertice v esta no heap minimo
                if(v == Q[j].u && M[aux.u][v]+aux.chave < Q[j].chave){
                    heap_increase_key(n, Q, j, M[aux.u][v]+aux.chave);
                    break;
                }
            }
        }
    }

    return -1;
}
