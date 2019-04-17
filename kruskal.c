#include <stdio.h>
#include <stdlib.h>

typedef struct arestas{
    int u;
    int v;
    int peso;
}arestas;

void quickSort(int n, arestas vetor[n], int I, int r);
int partition(int n, arestas vetor[n], int inicio, int fim);
int Kruskal(int n, int tam, arestas e[tam]);

int main(){
    int n;
    int i, j, k;
    int custo;

    FILE * arq;
    arq = fopen("dij10.txt", "r");

    if(arq == 0){
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }

    fscanf(arq, "%d", &n);

    int M[n][n]; /// Matriz de adjacencia
    int tam = 0;

    for(i=0; i<n; i++){
        M[i][i] = 0;
        for(j=i+1; j<n; j++){
            fscanf(arq, "%d", &M[i][j]);
            M[j][i] = M[i][j];
            tam = tam+1;
        }
    }

    fclose(arq);

    arestas e[tam];

    k=0;
    for(i=0; i<n; i++){
        for(j=i+1; j<n; j++){
            e[k].peso = M[i][j];
            e[k].u = i;
            e[k].v = j;
            k=k+1;
        }
    }

    /// Ordenando as arestas pelo peso
    quickSort(tam, e, 0, tam-1);

    /// Arvore espalhada minima
    custo = Kruskal(n, tam, e);
    printf("Custo = %d", custo);

    printf("\n\n");
    system("PAUSE");
    return 0;
}

void quickSort(int n, arestas vetor[n], int I, int r){
    int q;

    if(I<r){
        q = partition(n, vetor, I, r);
        quickSort(n, vetor, I, q-1);
        quickSort(n, vetor, q+1, r);
    }
}

int partition(int n, arestas vetor[n], int inicio, int fim){
    int pivo, i, j;
    arestas aux;

    pivo = vetor[inicio].peso;
    i = inicio - 1;
    j = fim + 1;

    while(1){
        do{
            i = i+1;
        }while(vetor[i].peso <= pivo);

        do{
            j = j-1;
        }while(vetor[j].peso > pivo);

        if(i >= j){
            break;
        }

        /// Fazendo a troca do elemento em i pelo elemento em j
        aux = vetor[i];
        vetor[i].peso = vetor[j].peso;
        vetor[i].u = vetor[j].u;
        vetor[i].v = vetor[j].v;
        vetor[j].peso = aux.peso;
        vetor[j].u = aux.u;
        vetor[j].v = aux.v;
    }

    /// Fazendo a troca do elemento em inicio pelo elemento em j
    aux = vetor[inicio];
    vetor[inicio].peso = vetor[j].peso;
    vetor[inicio].u = vetor[j].u;
    vetor[inicio].v = vetor[j].v;
    vetor[j].peso = aux.peso;
    vetor[j].u = aux.u;
    vetor[j].v = aux.v;

    return j;
}


int Kruskal(int n, int tam, arestas e[tam]){
    int i,j,pos;
    int custo=0;
    int raiz_u, raiz_v;
    int ind_u; /// variavel auxiliar que obtem a posicao que o vertice v sera add no conjunto u
    int ind_v; /// variavel auxiliar que obtem a posicao que o vertice u sera add no conjunto v (variavel auxiliar)
    int A[n];
    int L[n][n];
    int aux[n][2];

    /// Criando os conjuntos das ávores
    for(i=0; i<n-1; i++){
        for(j=0; j<n-1; j++){
            L[i][j] = -1;
        }
    }

    /// Matriz auxiliar de dimensao nx2
    for(i=0; i<n; i++){
        aux[i][0] = 1; /// comprimento de arvore de raiz i
        aux[i][1] = 0; /// Posicao em que um vertice u sera add no conjunto de um vertice v
    }

    /// Iniciando cada vertice como uma arvore
    for(i=0; i<n; i++){
        A[i] = i;
    }

    for(i=0; i<tam; i++){
        if(A[e[i].u] != A[e[i].v]){
            custo = custo + e[i].peso;

            raiz_u = A[e[i].u]; /// raiz da arvore que o vertice u esta
            raiz_v = A[e[i].v]; /// raiz da arvore que o vertice v esta

            int comp_u = aux[raiz_u][0]; /// Comprimento da arvore com raiz u
            int comp_v = aux[raiz_v][0]; /// Comprimento da arvore com raiz v

            if(comp_u >= comp_v){
                ind_u = aux[raiz_u][1];

                if(raiz_v != e[i].v){
                    A[raiz_v] = A[e[i].u];
                    comp_u += 1;

                    L[raiz_u][ind_u] = raiz_v; /// Adicionando o vertice raiz v arvore com raiz u
                    ind_u += 1;
                }

                A[e[i].v] = A[e[i].u]; /// Ligando vertices u e v de arvores distintas
                comp_u += 1;  /// Aumentando o comprimento da arvore de raiz u

                L[raiz_u][ind_u] = e[i].v; /// Adicionando o vertice v na arvore de raiz u
                ind_u += 1;

                /// Percorrendo o conjunto do vertice v
                ind_v = aux[raiz_v][1];
                for(j=0; j<ind_v; j++){
                    pos = L[raiz_v][j];
                    if(pos != e[i].v){
                        A[pos] = A[e[i].u]; /// Ligando os demais vertices da arvore de raiz v com  a arvore de raiz u
                        comp_u += 1;

                        L[raiz_u][ind_u] = pos; /// Adicionando os vertices da arvore de raiz v no conjunto da arvore de raiz u
                        ind_u += 1;
                    }
                }

                aux[raiz_u][0] = comp_u;
                aux[raiz_u][1] = ind_u;

                /// Todos os vertices ja foram adicionados
                if(comp_u == n){
                    break;
                }
            }
            else{
                ind_v = aux[raiz_v][1];

                if(raiz_u != e[i].u){
                    A[raiz_u] = A[e[i].v];
                    comp_v += 1;

                    ind_v = aux[raiz_v][1];
                    L[raiz_v][ind_v] = raiz_u; /// Adicionando o vertice raiz u arvore com raiz v
                    ind_v += 1;
                }

                A[e[i].u] = A[e[i].v]; /// Ligando vertices u e v de arvores distintas
                comp_v += 1;  /// Aumentando o comprimento da arvore de raiz v

                L[raiz_v][ind_v] = e[i].u; /// Adicionando o vertice u na arvore de raiz v
                ind_v += 1;

                /// Percorrendo o conjunto do vertice u
                ind_u = aux[raiz_u][1];
                for(j=0; j<ind_u; j++){
                    pos = L[raiz_u][j];
                    if(pos != e[i].u){
                        A[pos] = A[e[i].v]; /// Ligando os demais vertices da arvore de raiz u com  a arvore de raiz v
                        comp_v += 1;

                        L[raiz_v][ind_v] = pos; /// Adicionando os vertices da arvore de raiz u no conjunto da arvore de raiz v
                        ind_v += 1;
                    }
                }

                aux[raiz_v][0] = comp_v;
                aux[raiz_v][1] = ind_v;

                /// Todos os vertices ja foram adicionados
                if(comp_v == n){
                    break;
                }
            }
        }
    }

    return custo;
}
