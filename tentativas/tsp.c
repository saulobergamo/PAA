#/*-------UNIVERSIDADE TECNOLÓGICA FEDERAL DO PARANÁ-------*/
/*----------BACHARELADO EM SISTEMAS DE INFORMAÇÃO---------*/
/*-------------PROJETO E ANÁLISE DE ALGORITMOS------------*/

/* ALUNO: SAULO GREGÓRIO DE ANDRADE BERGAMO - RA 2023997  */

/*________________________________________________________*/
/*-------------PROBLEMA DO CAIXEIRO VIAJANTE--------------*/
/*  Algoritmo de aproximação para o problema do caixeiro  */
/* 	viajante. Utilizando a distância euclidiana como peso */
/*      das arestas, Árvore Geradora Mínima através do    */
/*  algoritmo de PRIM e busca em profundidade no subgrafo */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define FILHOE(x) (x+1) * 2 - 1
#define FILHOD(x) (x+1) * 2
#define PAI(x) (x - 1) / 2

#define BRANCO 0
#define CINZA 1
#define PRETO 2

static int tam;
static int tam_heap;

//estrutura do nó utilizado
typedef struct node{
    int x;
    int y;
    int name;
}Node;

typedef struct edge{
    Node current;
    float peso;
    Node father;
}Edge;


typedef struct grafo{
    int a;
    int v;
    Edge** lista_adg;
}Grafo;


float distancia(Node a, Node b);
void prim(Edge* vetor);
void imprime_vetor(Edge* vetor);
Edge *preenche_vetor(char* arquivo);
Edge extract_min(Edge* heap);

Node* preenche_vetor_node(char* arquivo){
    FILE* entrada = fopen(arquivo, "r");
    int n = 30;
    char line[n];
    char* space;

    if(entrada){
        fgets(line, n, entrada);
        tam = atoi(line);
    }
    else{
        printf("ERROR: Unable to open archive 'input.txt'");
		exit(-1);
    }
      Node* vetor = (Node*)malloc(tam * sizeof(Node));

    for(int i = 0; i < tam; i++){
        fgets(line, n, entrada);
        vetor[i].x = (int)strtof(line, &space);
        vetor[i].y = (int)strtof(space, NULL);
        vetor[i].name = i;
    }

    fclose(entrada);
    return vetor;   
}
/* preenche uma matriz parcialmente com os custos(distância euclidiana entre dois pontos)
 utiliza menos da metade da matriz, sendo E < v²/2 */
Edge **preenche_matriz(Node vetor[]){//, int tam){
    Edge **m;
    m = (Edge**)malloc(tam * sizeof(Edge*));
    for(int i = 0; i < tam; i++){
        m[i] = (Edge*)malloc(tam * sizeof(Edge));
    }
    float dist;
    for(int i = 0; i < tam; i++){
        for(int j = i + 1; j < tam; j++){
            if(i == j) m[i][j].peso = 0;
            dist = sqrt(pow(vetor[i].x - vetor[j].x, 2) + pow(vetor[i].y - vetor[j].y, 2));
            m[i][j].peso = dist;
            m[j][i].peso = dist;
        }
    }    
    return m;
}
/* imprime uma matriz */
void imprime_matriz(Edge **matrix){//, int tam){
    for(int i = 0; i< tam; i++){
        for(int j = 0; j < tam; j++){
            printf("%.3f ", matrix[i][j].peso);
        }
        printf("\n");
    }
    printf("\n");

}

void primMST(Edge** matrix){
    int  i, v, pai[tam];
    float chave[tam];
    bool mstSet[tam];
    for(i = 0; i < tam-1; i++){
        chave[i] = __FLT_MAX__;
        mstSet[i] = false;
        pai[i] = 0;
    }
    chave[0] = 0;
    pai[0] = -1;
    for(i = 0; i < tam - 1; i++){
        int u = minKey(chave, mstSet);
        mstSet[u] = true;
        for(v = 0; v< tam; v++){
            if(matrix[u][v].peso !=0 && mstSet[v]==false && matrix[u][v].peso < chave[v]){
                pai[v] = u;
                chave[v] = matrix[u][v].peso;
            }
        }
    }
    printMST(pai, tam, matrix);
}

int minKey(int key[], bool mstSet[]){
     int v; int min = __INT_MAX__;
     int min_index;
     for(v = 0; v < tam; v++){
         if(mstSet[v] == false && key[v]<min){
             min = key[v];
             min_index = v;
         }
     }
     return min_index;
 }

int printMST(int pai[], int n, int matrix[][n]){
    for(int i = 0; i < tam; i++){
        printf("%d - %d - %d\n", pai[i], i, matrix[i][pai[i]]);
    }
 }


int main(int argc, char* argv[]){

    Edge** novo;
    Node* novono;
    novono = preenche_vetor_node(argv[1]);
    for(int i = 0; i < tam; i++){
        printf("%d. (%d, %d)\n", novono[i].name, novono[i].x, novono[i].y);
    }
    novo = preenche_matriz(novono);
    imprime_matriz(novo);

    clock_t time = clock();
    /* 
    novo = preenche_vetor(argv[1]);
    printf("tam: %d\n", tam);
    imprime_vetor(novo);

    prim(novo);
    imprime_vetor(novo);
 */
    printf("tam: %d\ntempo: %.3f\n\n", tam, ((float)(clock()-time))/CLOCKS_PER_SEC);


    return 0;
}

float distancia(Node a, Node b){
    float temp = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    return temp;
}

/* preenche uma matriz parcialmente com os custos(distância euclidiana entre dois pontos)
 utiliza menos da metade da matriz, sendo E < v²/2 */
/* Node **preenche_matriz(Node vetor[]){//, int tam){
    Node **m;
    m = (Node**)malloc(tam * sizeof(Node*));
    for(int i = 0; i < tam; i++){
        m[i] = (Node*)malloc(tam * sizeof(Node));
    }
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            m[i][j].dist = -1;
        }
    }

    for (int j = 0; j < tam; j++){
        for(int k = j + 1; k < tam; k++){
            m[j][k].dist = (sqrt(pow(vetor[j].x - vetor[k].x, 2) + pow(vetor[j].y - vetor[k].y, 2));
        }
    }
    
    return m;
}
 */
/* imprime um vetor */
void imprime_vetor(Edge* vetor){
    printf("{ ");
    for(int i = 0; i< tam; i++){
        printf("%d. (%d , %d) || dist: %.3f || pai: (%d , %d)\n", vetor[i].current.name, vetor[i].current.x, vetor[i].current.y, vetor[i].peso, vetor[i].father.x, vetor[i].father.y);
        
    }
    printf("}\n\n");
}

/* imprime uma matriz */
/* void imprime_matriz(Node **matrix){//, int tam){
    for(int i = 0; i< tam; i++){
        for(int j = 0; j < tam; j++){
            if(j > i){
                printf("%.3f ", matrix[i][j].dist);
            }
            else{
                printf("   .    " );
            }
        }
        printf("\n");
    }
    printf("\n");

} */


/* abre o arquivo, copia cada par ordenado para um vetor do tipo node */
Edge *preenche_vetor(char* arquivo){
    FILE* entrada = fopen(arquivo, "r");
    int n = 30;
    char line[n];
    char* space;

    if(entrada){
        fgets(line, n, entrada);
        tam = atoi(line);
    }
    else{
        printf("ERROR: Unable to open archive 'input.txt'");
		exit(-1);
    }
    /*   Node* vetor = (Node*)malloc(tam * sizeof(Node));

    for(int i = 0; i < tam; i++){
        fgets(line, n, entrada);
        vetor[i].x = (int)strtof(line, &space);
        vetor[i].y = (int)strtof(space, NULL);
    } */

    Edge* vetor = (Edge*)malloc(tam * sizeof(Edge));
    Node a, b;
    fgets(line, n, entrada);
    a.x = (int)strtof(line, &space);
    a.y = (int)strtof(space, NULL);
    a.name = 0;

    vetor[0].current = a;
    //vetor[0].father = NULL;
    vetor[0].peso = 0;
    
    for(int i = 1; i < tam; i++){
        fgets(line, n, entrada);
        b.x = (int)strtof(line, &space);
        b.y = (int)strtof(space, NULL);
        b.name = i;
        vetor[i].current = b;
        vetor[i].father = a;
        vetor[i].peso = distancia(a, b);
    }
    fclose(entrada);
    return vetor;    
}


void corrigir_heap(Edge* heap, int a){
    int p = a;
    int fe, fd, f;
    Edge temp;
    while(FILHOE(p) < tam_heap){
        fe = FILHOE(p);
        fd = FILHOD(p);
        if(fd >= tam_heap) fd = fe;
        if(heap[fe].peso > heap[fd].peso){
            f = fe;
        }
        else{
            f = fd;
        }
        if(heap[p].peso < heap[f].peso){
            temp = heap[p];
            heap[p] = heap[f];
            heap[f] = temp;
        }
        else break;
        p = f;
    }
}

Edge extract_min(Edge* heap){
    Edge temp = heap[0];
    heap[0] = heap[tam_heap-1];
    tam_heap--;
    corrigir_heap(heap, 0);
    return temp;
}


void prim(Edge* vetor){

    Edge* min_heap = (Edge*)malloc(sizeof(Edge));
    min_heap = vetor;
    tam_heap = tam;
   
    for(int k = tam; k> 0; k--){
        int j = 0;
        vetor[j] = extract_min(min_heap);
        for(int i = 1; i < tam_heap; i++ ){
            if(distancia(min_heap[i].current, vetor[j].current) < min_heap[i].peso){
                        printf("ENTROU AQUI");

                min_heap[i].father.x = vetor[j].current.x;
                min_heap[i].father.y = vetor[j].current.y;
                
                min_heap[i].peso = distancia(min_heap[i].current, vetor->current);
            }
        }
        
        j++;
    }


}
 