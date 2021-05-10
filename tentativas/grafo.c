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

static int tam;

//estrutura do nó utilizado
typedef struct node{
    int x;
    int y;
    float dist;
    int xpai;
    int ypai;
}Node;

typedef struct edge{
    float peso;

}Edge;


typedef struct grafo{
    int a;
    int v;
    Node **matrizAdj;
}Grafo;



Node **preenche_matriz(Node vetor[]);//, int tam); 
void imprime_vetor(Node vetot[]);//, int tam);
void imprime_matriz(Node **matrix);//, int tam);
Node *preenche_vetor(char* arquivo);//, int *tam);
void prim(Node matrix[][tam], Node vetor[]);

int main(int argc, char* argv[]){

    Node **matrix;
    
    Node *novo;
   
    clock_t time = clock();
    
    novo = preenche_vetor(argv[1]);//, &tam);
    printf("tam: %d\n", tam);
    imprime_vetor(novo);//, tam);
    matrix = preenche_matriz(novo);//, tam);
    imprime_matriz(matrix);//, tam);
    printf("tam: %d\ntempo: %.3f\n\n", tam, ((float)(clock()-time))/CLOCKS_PER_SEC);

   /*  //float mat[tam][tam];
    float **mat = NULL;
    mat = (float**)malloc(tam * sizeof(float*));
    for(int i; i<tam; i++){
        //mat[i]=NULL;
        mat[i] = (float*)malloc(tam*sizeof(float));
    }
    
    for(int i = 0; i<tam; i++){
        for(int j = 0; j<tam; j++){
            printf("%1.f ", mat[i][j]);
        }
        printf("\n");
    } */



    return 0;
}

/* preenche uma matriz parcialmente com os custos(distância euclidiana entre dois pontos)
 utiliza menos da metade da matriz, sendo E < v²/2 */
Node **preenche_matriz(Node vetor[]){//, int tam){
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
            m[j][k].dist = sqrt(pow(vetor[j].x - vetor[k].x, 2) + pow(vetor[j].y - vetor[k].y, 2));
        }
    }
    
    return m;
}

/* imprime um vetor */
void imprime_vetor(Node vetot[]){//, int tam){
    printf("{ ");
    for(int i = 0; i< tam; i++){
        printf("(%d , %d) || dist: %.3f || pai: (%d , %d)\n", vetot[i].x, vetot[i].y, vetot[i].dist, vetot[i].xpai, vetot[i].ypai);//|| pai: (%d , %d)
        
    }
    printf("}\n\n");
}

/* imprime uma matriz */
void imprime_matriz(Node **matrix){//, int tam){
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

}


/* abre o arquivo, copia cada par ordenado para um vetor do tipo node */
Node *preenche_vetor(char* arquivo){//, int *tam){
    FILE* entrada = fopen(arquivo, "r");
    int n = 30;
    char line[n];
    char* space;
    //char* space;
    //int qtd_pnts = 0;

    if(entrada){
        fgets(line, n, entrada);
        //qtd_pnts = atoi(line);
        //*tam = atoi(line);
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

    Node* vetor = (Node*)malloc(tam * sizeof(Node));
    fgets(line, n, entrada);
    vetor[0].x = (int)strtof(line, &space);
    vetor[0].y = (int)strtof(space, NULL);
    vetor[0].dist = 0;
    vetor[0].xpai = 0;   
    vetor[0].ypai = 0;
    for(int i = 1; i < tam; i++){
        fgets(line, n, entrada);
        vetor[i].x = (int)strtof(line, &space);
        vetor[i].y = (int)strtof(space, NULL);
        vetor[i].xpai = vetor[0].x;
        vetor[i].ypai = vetor[0].y;
        vetor[i].dist = sqrt(pow(vetor[0].x - vetor[i].x, 2) + pow(vetor[0].y - vetor[i].y, 2));
    }
    fclose(entrada);
    return vetor;    
}

void prim(Node matrix[][tam], Node vetor[]){
   /*  Node *aux = (Node*)malloc(tam * sizeof(Node));
    bool vet[tam];
    for(int i = 0; i < tam; i++){
        vet[i] = false;
    }
    
    aux[0] = matrix[0][1];

    for(int i = 0; i < tam; i++){
        int v = -1;
        for(int j = i + 1; j < tam; j++){
            if(!vet[j] && (v == -1 || aux[j].dist < aux[v].dist))
                v = j;
        }
        aux[i] = matrix[i][v];

        vet[v] = true;
        
        for(int k = 0; k < tam; k++){
            if(matrix[v][k].dist < aux[k].peso){
                //aux[k] = 
            }
        }
    } */
}