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

/*Define a coloração para visita em cada vértice durante a busca em profundidade*/
#define BRANCO 0
#define CINZA 1
#define PRETO 2

/*Variável para armazenar o tamanho do grafo e vetor auxiliar utilizado pelas
funções de preenchimento e impressão de vetores, matrizes e listas de adjacência*/
static int tam;

/*Estrutura do nó/vértice utilizado*/
typedef struct node{
    int x;	   	 //Ponto é representado por x e y -> P = (x, y)
    int y;
    int pai;      	 //Atributsos utilizados para a função meuPrim -> que computa a AGM 
    float chave;   	 //chave é o peso da aresta, a distância euclidiana do vértice atual para
    bool agm;      	 //o vértice mais próximo. Chave é atualizada a cada passagem de PRIM  
    int cor;	    	 //Atributo cor utilizado para busca em profundidade -> DFS
    int ordem;	   	 //Ordem final da busca em profundidade		
    int pos;	   	 //Armazea a posição inicial de cada ponto P = (x, y)
    struct node* prox;	 //Ponteiro para o próximo nó/vértice -> para listas de adjacência
}Node;

/*Estrutura para cada aresta, comtempla o nó/vértice corrente, um ponteiro para o próximo
nó/vértice a ser utilizado em uma lista de adjacências e o peso da aresta/arco. Esta estrutura 
não foi utilizada, optou-se por utilizar a estrutura nó com atributos adicionais para cumprir o mesmo papel*/
typedef struct edge{
    Node current;
    float peso;
    Node* prox;
}Edge;

/*Estrutura de um grafo  */
typedef struct grafo{
    int a;
    int v;
    Node** lista;
}Grafo;

/* definição de funções */
float custoAGM(Node* vetor);
void imprime_AGM(Node* vetor);
void imprime_matriz(Edge** matriz);
void imprime_vetor(Node* vetor);
void meuPrim(Node* vetor, Edge** matriz);
int chave_m(Node* vetor);
Edge** preenche_matriz(Node* vetor);
Node* recebe_nodes(char* arquivo);

void imprime_grafoAGM(Grafo* grafo);
void incluir_arestas(Grafo* grafo, Node* vetor);
void free_Grafo(Grafo* grafo);
Grafo* criar_grafo();

void visita_DFS(Grafo* grafo, Node* aux, int i, int* visita){
    aux[i].cor = CINZA;
    *visita = *visita + 1;
    aux[i].ordem = *visita;
    Node* temp;
    for(temp = grafo->lista[i]; temp != NULL; temp = temp->prox){
        if(aux[temp->pos].cor == BRANCO){
            aux[temp->pos].pai = i;// atualiza o pai aqui????
            visita_DFS(grafo, aux, temp->pos, visita);
        }
    }
    aux[i].cor = PRETO;		
}

void busca_DFS(Grafo* grafo){
    int visita = 0;

    Node* aux = (Node*)malloc(sizeof(Node));

    for(int i = 0; i < tam; i++){
        aux[i].cor = BRANCO;
        aux[i].pai = -1;
        aux[i] = *grafo->lista[i];
    }
    for(int i = 0; i < tam; i++){
        if(aux[i].cor == BRANCO){
            visita_DFS(grafo, aux, i, &visita);
        }
    }
    //retornar o que?? // NÃO ESTÁ FUNCIONANDO ESTA MERDA
    for(int i = 0; i < tam; i++){
        printf("%d(%d , %d) : ordem %d : pai %d(%d , %d)\n", i, aux[i].x, aux[i].y, aux[i].ordem, aux[i].pai, aux[aux[i].ordem].x, aux[aux[i].ordem].y);
    }
}

/*Função principal - recebe uma entrada tipo .txt contendo os pontos de interesse para o porblema do caixeiro viajante (uma aproximação)*/
int main(int argc, char* argv[]){
    Node* vetor;
    Edge** matriz;
    clock_t tempo = clock();
        vetor = recebe_nodes(argv[1]);
    matriz = preenche_matriz(vetor);
    imprime_vetor(vetor);
    imprime_matriz(matriz);
    meuPrim(vetor, matriz);
    imprime_AGM(vetor);
    printf("%.6f %.6f\n\n", ((float)(clock()-tempo))/CLOCKS_PER_SEC, (custoAGM(vetor) + vetor[1].chave + vetor[3].chave)*100);
    
    printf("TESTE GRAFO LSITA DE ADJACÊNCIA\n");
    Grafo* grafo = criar_grafo();
    incluir_arestas(grafo, vetor);
    imprime_grafoAGM(grafo);
    printf("TESTE BUSCA EM PROFUNDIDADE - DFS\n");
    busca_DFS(grafo);

    return 0;
}

/*Função que abre o arquivo de entrada (input.txt), contendo n pontos P = (x , y). Guarda a primeira linha que contém o número
de pontos em uma variável global estática (static tam) que poderá ser acessada por todas as funções a qualquer tempo, inicializa
os atributos pai/chave/agm/pos que serão utilizados na função meuPrim para computar a Árvore Geradora Mínima, fecha o arquivo
e retorna o um vetor contendo todos os pontos P = (x, y)*/
Node* recebe_nodes(char* arquivo){
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
        vetor[i].pai = 0;
        vetor[i].chave = __FLT_MAX__;
        vetor[i].agm = false;
	veto[i].pos = i;
    }

    fclose(entrada);
    return vetor;   
}

/* preenche uma matriz com os custos(distância euclidiana entre dois pontos) calcula menos da metade da matriz, sendo
E < v²/2. Um grafo denso e não direcional pode ser representado por uma matriz de adjacências, nesse caso uma matriz 
simétrica, por este motivo é preciso apenas a passagem em n*(n-1)/2 vértices/arcos*/
Edge** preenche_matriz(Node* vetor){
    Edge **m;
    m = (Edge**)malloc(tam * sizeof(Edge*));
    for(int i = 0; i < tam; i++){
        m[i] = (Edge*)malloc(tam * sizeof(Edge));
    }
    float dist;
    for(int i = 0; i < tam; i++){                    /* executa n vezes */
        for(int j = i + 1; j < tam; j++){            /* executa n-1 vezes e à cada passagem o alcance diminui em 1*/
            if(i == j) m[i][j].peso = 0;             /* (n -1)+(n -2)+(n - 3)...+(n-n-1)+(n-n), invertendo a ordem da*/
                                                     /* soma observa-se uma soma de uma progressão aritmética com fórmula fechada n*(n-1)/2 */
            dist = sqrt(pow(vetor[i].x - vetor[j].x, 2) + pow(vetor[i].y - vetor[j].y, 2));
            m[i][j].peso = dist;
            m[j][i].peso = dist;
        }
    }    
    return m;
}

int chave_m(Node* vetor){
    float min = __FLT_MAX__;
    int min2;
    for(int i = 0; i < tam; i++){
        if(vetor[i].agm == false && vetor[i].chave < min){
            min = vetor[i].chave;
            min2 = i;
        }
    }
    return min2;
}

/*Algoritmo de Prim aplicado à matriz de adjacência contendo chaves/pesos que são atualizados à cada passagem*/
void meuPrim(Node* vetor, Edge** matriz){
    vetor[0].chave = 0;
    vetor[0].pai = -1;
    for(int i = 0; i < tam -1; i++){
        int u = chave_m(vetor);
        vetor[u].agm = true;
        for(int v = 0; v < tam; v++){
            if(matriz[u][v].peso != 0 && vetor[v].agm == false && matriz[u][v].peso < vetor[v].chave){
                vetor[v].pai = u;
                vetor[v].chave = matriz[u][v].peso;
            }
        }
    }
}

void imprime_vetor(Node* vetor){
    for(int i = 0; i< tam; i++){
        printf("(%d, %d) ", vetor[i].x, vetor[i].y);
    }
    printf("\n");
}

void imprime_matriz(Edge** matriz){
    for(int i = 0; i< tam; i++){
        for(int j = 0; j < tam; j++){
            printf("%.3f ", matriz[i][j].peso);
        }
        printf("\n");
    }
    printf("\n");
}

void imprime_AGM(Node* vetor){
    for(int i = 0; i< tam; i++){
        printf("%d -> (%d, %d)\n", vetor[i].pai,vetor[i].x, vetor[i].y);
    }
    printf("\n");
}

float custoAGM(Node* vetor){
    float custo = 0;
    for(int i = 0; i < tam; i++){
        custo = vetor[i].chave + custo;
    }
    return custo;
}


Grafo* criar_grafo(){
    Grafo* temp = (Grafo*)malloc(sizeof(Grafo));
    temp->a = 0;
    temp->v = tam;
    temp->lista = (Node**)malloc(tam * sizeof(Node*));
    for(int i = 0; i < tam; i++){
        temp->lista[i] = NULL;
    }
    return temp;
}

void free_Grafo(Grafo* grafo){
    for(int i = 0; i < grafo->v; i++){
        if(grafo->lista != NULL){
            free(grafo->lista[i]);
        }
    }
    free(grafo->lista);
    free(grafo);
}

void incluir_arestas(Grafo* grafo, Node* vetor){
    Node *temp = NULL;
    Node* ultimo = NULL;

    for(int i = 0; i < tam; i++){
        grafo->lista[i] = &vetor[i];
    }


    for(int i = 1; i < tam; i++){
        for(temp = grafo->lista[vetor[i].pai]; temp != NULL; temp = temp->prox){
            if(temp->x == vetor[i].x && temp->y == vetor[i].y){
                continue; 
            }
            ultimo = temp;
        }
        Node* novo_node = (Node*)malloc(sizeof(Node));
        novo_node->x = vetor[i].x;
        novo_node->y = vetor[i].y;
        novo_node->chave = vetor[i].chave;
        novo_node->pos = i;
        novo_node->cor = BRANCO;
        novo_node->prox = NULL;
        if(ultimo != NULL){
            ultimo->prox = novo_node;
        }
        else{
            grafo->lista[i] = novo_node;
        }
    }   
}

void imprime_grafoAGM(Grafo* grafo){
    for(int i = 0; i < tam; i++){
        if(grafo->lista[i] != NULL){
            Node* aux;
            printf("%d", i);
            for(aux = grafo->lista[i]; aux != NULL; aux = aux->prox){
                if(aux->prox != NULL)printf("(%d , %d) -> %d", aux->x, aux->y, aux->prox->pos);
                else printf("(%d , %d)", aux->x, aux->y);
            }
            printf("\n");
        }
    }
}
