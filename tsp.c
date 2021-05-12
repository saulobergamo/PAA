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
/*  e cálculo de um ciclo hamiltoniano como caminho mais  */
/*   curto entre todos os pontos recebidos como entrada.  */


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
    int x;	   	     //Cada ponto é representado por x e y -> P = (x, y)
    int y;
    int pai;      	 //Atributos utilizados para a função meuPrim -> que computa a AGM 
    float chave;   	 //chave é o peso da aresta, a distância euclidiana do vértice atual para
    bool agm;      	 //o vértice mais próximo. Chave é atualizada a cada passagem de PRIM  
    int cor;	     //Atributo cor utilizado para busca em profundidade -> DFS
    int ordem;	   	 //Ordem final da busca em profundidade		
    int pos;	   	 //Armazea a posição inicial de cada ponto P = (x, y)
    struct node* prox;	 //Ponteiro para o próximo nó/vértice -> para listas de adjacência
}Node;

/*Estrutura de um grafo  */
typedef struct grafo{
    int a;          //Número de artestas
    int v;          //Número de vértices
    Node** lista;   //Ponteiro para a lista de adjacências
}Grafo;

/* definição de funções */
float custoAGM(Node* vetor);
void imprime_matriz(float** matriz);
void imprime_vetor(Node* vetor);
void meuPrim(Node* vetor, float** matriz);
int chave_m(Node* vetor);
float** preenche_matriz(Node* vetor);
Node* recebe_nodes(char* arquivo);
void imprime_grafoAGM(Grafo* grafo);
void incluir_arestas(Grafo* grafo, Node* vetor);
void free_Grafo(Grafo* grafo);
Grafo* criar_grafo();
void intercalarmerge(Node array[], int ini, int fim, int meio);
void ordenaMergeSort(Node array[], int ini, int fim);
void grava_AGM(Node vetor[], Node busca[]);
void visita_DFS(Grafo* grafo, Node* aux, int i, int* visita);
Node* busca_DFS(Grafo* grafo);
void grava_Ciclo(Node vetor[]);


/*Função principal - recebe uma entrada tipo .txt contendo os pontos de interesse para o porblema do caixeiro viajante (uma aproximação)*/
int main(int argc, char* argv[]){
    Node* vetor;
    float** matriz;
    clock_t tempo = clock();
    
    vetor = recebe_nodes(argv[1]);                        //srecebe em 'vetor' os pontos do arquivo de entrada
    matriz = preenche_matriz(vetor);                      //recebe em 'matriz' os pesos das arestas (todos para todos)
  
    meuPrim(vetor, matriz);                               //calcula a Árvore Geradora Mínimo(AGM) pelo algoritmo de Prim em uma matriz de adjacências

    Grafo* grafo = criar_grafo();                         //cria um grafo
    incluir_arestas(grafo, vetor);                        //inclui as arestas da AGM no grafo representado em lista de adjacências

    Node* busca = (Node*)malloc(tam * sizeof(Node));      
    busca = busca_DFS(grafo);                             //guarda em um vetor a busca em profundidade no grafo gerado pela AGM
    ordenaMergeSort(busca, 0, tam);                       //ordena, a partir da ordem de visitação, o vetor da busca
    
    grava_AGM(vetor, busca);                              //registra o arquivo de saída para a AGM
    grava_Ciclo(busca);                                   //registra o arquivo de saída para o ciclo

    printf("%.6f %.6f\n\n", ((float)(clock()-tempo))/CLOCKS_PER_SEC, (custoAGM(busca)*100));//imprime a saída com tempo total e custo do caminho encontrado
   
    return 0;
}

/*Função que abre o arquivo de entrada (input.txt), contendo n pontos P = (x , y). Guarda a primeira linha que contém o número
de pontos em uma variável global estática (static tam) que poderá ser acessada por todas ou qualquer uma das funções a qualquer tempo, 
inicializa os atributos pai/chave/agm/pos que serão utilizados na função meuPrim para computar a Árvore Geradora Mínima, fecha o arquivo
e retorna o um vetor contendo todos os pontos P = (x, y)*/
Node* recebe_nodes(char* arquivo){
    FILE* entrada = fopen(arquivo, "r");    //Cria um ponteiro tipo FILE e abre o arquivo recebido como parâmetro em modo leitura
    int n = 30;                             //Define n = 30 para criar um strings/vetor char com um número máximo de caracteres a ser lido em cada linha
    char line[n];
    char* space;                            //Ponteiro que aponta para o espaço entre os caracteres que serão lidos em cada linha

    if(entrada){                            //Verifica se o arquivo abriu corretamente
        fgets(line, n, entrada);            //Recebe a primeira linha com o número de pontos do arquivo
        tam = atoi(line);                   //Registra o número de pontos na variável global static 'tam' que poderá ser acessada a qualquer tempo
    }
    else{                                   //Em caso de erro mostra a mensagem e encerra
        printf("ERRO: Não foi possível abrir o arquivo de entrada.");
		exit(-1);
    }
    Node* vetor = (Node*)malloc(tam * sizeof(Node));//Cria um ponteiro para uma variável Node e aloca memória suficiente para a quantidade de pontos recebidos

    for(int i = 0; i < tam; i++){               //Laço que percorre todas as 'n' linhas restantes do arquivo
        fgets(line, n, entrada);                //Lê os caracteres de cada linha
        vetor[i].x = (int)strtof(line, &space); //Armazena cada ponto P = (x, y)
        vetor[i].y = (int)strtof(space, NULL);
        vetor[i].pai = 0;                       //Inicializa demais atributos
        vetor[i].chave = __FLT_MAX__;
        vetor[i].agm = false;
	    vetor[i].pos = i;
    }

    fclose(entrada);                            //Fecha o arquivo
    return vetor;                               //Retorna o vetor contendo todos os pontos
}

/* Preenche uma matriz tipo float com os custos(distância euclidiana entre dois pontos) calcula menos da metade da matriz, sendo
E < v²/2. Um grafo denso e não direcional pode ser representado por uma matriz de adjacências, nesse caso uma matriz 
simétrica, por este motivo é preciso apenas a passagem em n*(n-1)/2 vértices/arcos*/
float** preenche_matriz(Node* vetor){
    float **m;
    m = (float**)malloc(tam * sizeof(float*));
    for(int i = 0; i < tam; i++){
        m[i] = (float*)malloc(tam * sizeof(float));
    }
    float dist;
    for(int i = 0; i < tam; i++){                    // Executa n vezes
        for(int j = i + 1; j < tam; j++){            // Executa n-1 vezes e à cada passagem o alcance diminui em 1
            if(i == j) m[i][j] = 0;                  // (n -1)+(n -2)+(n - 3)...+(n-n-1)+(n-n), invertendo a ordem da soma
                                                     // Observa-se uma soma de uma progressão aritmética com fórmula fechada n*(n-1)/2 
            dist = sqrt(pow(vetor[i].x - vetor[j].x, 2) + pow(vetor[i].y - vetor[j].y, 2)); //Calcula efetivamente a distância euclidiana entre os pontos
            m[i][j] = dist;                          //Inclui de maneira simétrica o valor retornado em 'dist' na matriz
            m[j][i] = dist;
        }
    }    
    return m;
}

/* Função utilizada dentro do algortitmo de Prim para calcular a menor chave de um vetor retornando o índice do vetor
índice em que se encontra a menor chave */
int chave_m(Node* vetor){
    float min = __FLT_MAX__;                                //Inicializa o valor 'min' com um número muito alto
    int min2;
    for(int i = 0; i < tam; i++){                          //Um laço de repetição que executa n vezes, complexidade O(n)
        if(vetor[i].agm == false && vetor[i].chave < min){ //Percorre o vetor verificando se já faz parte da AGM(true) e se a chave é menor que 'min'
            min = vetor[i].chave;                          //Atualiza 'min' para a menor chave
            min2 = i;                                      //Atualiza o índice que será retornado ao final
        }
    }
    return min2;
}

/*Algoritmo de Prim aplicado à matriz de adjacência contendo chaves/pesos que são atualizados à cada passagem*/
void meuPrim(Node* vetor, float** matriz){
    vetor[0].chave = matriz[0][1];                         //Inicializa a chave do primeiro ponto com a primeira chave da matriz
    vetor[0].pai = -1;                                     //Pai do primeiro ponto é -1 -> não tem pai
    for(int i = 0; i < tam -1; i++){                       //Excuta o laço n vezes
        int u = chave_m(vetor);                            //Busca o índice em que o vértice/nó/ponto tem a menor chave/peso
        vetor[u].agm = true;                               //Marca o vértice encontrado como TRUE -> já utilizado
        for(int v = 0; v < tam; v++){                      //Executa n vezes para percorrer cada coluna da matriz
            if(matriz[u][v] != 0 && vetor[v].agm == false && matriz[u][v] < vetor[v].chave){
                                                           //Verifica se o peso é differente de 0, o vértice ainda não foi utilizado e o pedo á menor do que a chave no índice v do vetor
                vetor[v].pai = u;                          //Atualiza o pai do vértice[v]
                vetor[v].chave = matriz[u][v];             //Atualiza a chave/peso do vértice  
            }
        }                                                  //Complexidade O(n²) contando dois laços encadeados para percorrer a matriz
    }                                                      //O algoritmo de Prim pode ser implementado em O(mlogn) utilizando um Heap mínimo como estrutura de dados
}
/* Função para imprimir os pontos P = (x , y) de um vetor tipo Node */
void imprime_vetor(Node* vetor){
    for(int i = 0; i< tam; i++){
        printf("(%d, %d) ", vetor[i].x, vetor[i].y);
    }
    printf("\n");
}

/* Função para imprimir os pesos/chaves em uma matriz de adjacências */
void imprime_matriz(float** matriz){
    for(int i = 0; i< tam; i++){
        for(int j = 0; j < tam; j++){
            printf("%.3f ", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Função para retornar o custo total de um ciclo indicado por um vetor de pontos P = (x , y) */
float custoAGM(Node* vetor){
    float custo = 0;
    for(int i = 0; i < tam; i++){
        custo = vetor[i].chave + custo;
    }
    custo = vetor[0].chave + custo;
    return custo;
}

/* Função para criar um grafo */
Grafo* criar_grafo(){
    Grafo* temp = (Grafo*)malloc(sizeof(Grafo));        //Cria um ponteiro para Grafo e aloca o espaço suficiente para armazenar seus vértices e arestas
    temp->a = 0;
    temp->v = tam;
    temp->lista = (Node**)malloc(tam * sizeof(Node*));
    for(int i = 0; i < tam; i++){
        temp->lista[i] = NULL;
    }
    return temp;
}

/* Libera a memória alocada para o grafo */
void free_Grafo(Grafo* grafo){
    for(int i = 0; i < grafo->v; i++){
        if(grafo->lista != NULL){
            free(grafo->lista[i]);
        }
    }
    free(grafo->lista);
    free(grafo);
}

/* Função para incluir arestas em um grafo. Recebe o grafo e um vetor com n vértices como parâmetros */
void incluir_arestas(Grafo* grafo, Node* vetor){
    Node *temp = NULL;                                              //Cria ponteiros para nós/vértices auxiliares
    Node* ultimo = NULL;

    for(int i = 0; i < tam; i++){                                   //Recebe o vetor com n pontos em grafo->lista
        grafo->lista[i] = &vetor[i];
    }


    for(int i = 1; i < tam; i++){                                   
        for(temp = grafo->lista[vetor[i].pai]; temp != NULL; temp = temp->prox){
            if(temp->x == vetor[i].x && temp->y == vetor[i].y){    //Percorre cada vértice por índice e seus adjacentes
                continue; 
            }
            ultimo = temp;
        }
        Node* novo_node = (Node*)malloc(sizeof(Node));             //Cria um novo vértice e aloca memória
        novo_node->x = vetor[i].x;                                 //Atualiza os atributos
        novo_node->y = vetor[i].y;
        novo_node->chave = vetor[i].chave;
        novo_node->pos = i;
        novo_node->cor = BRANCO;
        novo_node->prox = NULL;
        if(ultimo != NULL){                                         //Inclui o novo vértice no grafo
            ultimo->prox = novo_node;
        }
        else{
            grafo->lista[i] = novo_node;
        }
        free(novo_node);                                            //Libera o espaço alocado
        grafo->a += grafo->a;                                       //Atualiza o número de arestas criadas
    }   
}

/* funçãõ imprime um grafo, particularmente o grafo gerado após executar e calcular a AGM por Prim */
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

/*Função para conquistar e combinar o problemas subdivididos por mergesort */
void intercalarmerge(Node array[], int ini, int fim, int meio){

    int i = ini;
	int j = meio;
	int k = 0;

    Node aux[fim + 1 - ini];

    while(i < meio && j < fim){
        if(array[i].ordem < array[j].ordem){
            aux[k] = array[i];
            i++;
        }
        else{
            aux[k] = array[j];
            j++;
        }
        k++;
    }

    while(i < meio){
        aux[k] = array[i];
        i++;
        k++;
    }

    while(j < fim){
        aux[k] = array[j];
        j++;
        k++;
    }

    for(i = ini; i < fim; i++){
        array[i] = aux[i - ini];
    }
}

/*Função mergesort para ordenação de dados em um vetor, algoritmo estáel que ordena em complexidade O(nlgn)
dividindo recursivamente o problema em subproblemas menores*/
void ordenaMergeSort(Node array[], int ini, int fim){
    int meio;
    if(ini < fim - 1){
        meio = (fim + ini)/2;
        ordenaMergeSort(array, ini, meio);
        ordenaMergeSort(array, meio, fim);   
        intercalarmerge(array, ini, fim, meio);
    }
}

/* Função para gravar o arquivo de saíde contendo os pontos relatisvos à AGM calculada anteriormente */
void grava_AGM(Node vetor[], Node busca[]){
    FILE* saidaAGM = fopen("tree.txt", "w");                        //Cria um ponteiro tipo FILE e abre o arquivo recebido como parâmetro em modo escrita
    if(!saidaAGM){                                                  //Verifica se o arquivo foi aberto corretamente
        printf("Não foi possível acessar o arquivo 'tree.txt' para gravação\n");
        exit(-1);
    }
    for(int i = 1; i < tam; i++){                                   //Percorre o vetor de entradas escrevendo no arquivo de saída os pontos P = (x , y) em cada linha
        fprintf(saidaAGM, "%d %d\n%d %d\n", vetor[busca[i].pai].x, vetor[busca[i].pai].y, busca[i].x, busca[i].y);
    }
    fclose(saidaAGM);
}

/* Função de visita para a busca em profundidade - chamadas recursivas */
void visita_DFS(Grafo* grafo, Node* aux, int i, int* visita){
    aux[i].cor = CINZA;                                            //Pinta de cinza o vértice visitado pela primeira vez
    *visita = *visita + 1;                                         //Incrementa o númera de visita, que será a ordem de saída para a busca
    aux[i].ordem = *visita;                                        //Atualiza a ordem do vértice visitado
    Node* temp;                                                    //Cria um novo vértice
    for(temp = grafo->lista[i]; temp != NULL; temp = temp->prox){  //Laço de repetição executa uma vez para cada aresta, complexidade O(E)
        if(aux[temp->pos].cor == BRANCO){                          //Verifica se cada nó adjacente já foi visitado
            aux[temp->pos].pai = i;                                //Atusaliza o pai do nó que será visitado com o índive do nó atual
            visita_DFS(grafo, aux, temp->pos, visita);             //Chamada recursiva para visita
        }
    }
    free(temp);
    aux[i].cor = PRETO;		
}

/* Função Busca em profundidade */
Node* busca_DFS(Grafo* grafo){
    int visita = 0;                                               //Cria uma variável para armazenar os número de cada visita e inicializa em 0

    Node* aux = (Node*)malloc(tam * sizeof(Node));                //Cria um novo vértice e aloca memória suficiente para armazenar os dados dos vértices

    for(int i = 0; i < tam; i++){                                 //Laço de repetição executa uma vez para cada vértice para atualizar atributos do vetor auxiliar
        aux[i].cor = BRANCO;                                      //Complexidade O(V)
        aux[i].pai = -1;
        aux[i] = *grafo->lista[i];
    }                       
    for(int i = 0; i < tam; i++){                                 //Laço de repetição para chamadas da função de visita para a busca
        if(aux[i].cor == BRANCO){                                 //Complexidade O(V)
            visita_DFS(grafo, aux, i, &visita);
        }
    }

    return aux;                                                   //Comlexidade O(V+E) = O(n)
}

/* Função para gravar o arquivo de saída para o ciclo calculado */
void grava_Ciclo(Node vetor[]){
    FILE* ciclo = fopen("cycle.txt", "w");                        //Cria um ponteiro tipo FILE e abre o arquivo recebido como parâmetro em modo escrita
    if(!ciclo){                                                   //Verifica se o arquivo foi aberto corretamente 
        printf("Não foi possível acessar o arquivo 'tree.txt' para gravação\n");
        exit(-1);
    }
    for(int i = 0; i < tam; i++){
        fprintf(ciclo, "%d %d\n", vetor[i].x, vetor[i].y);        //Percorre o vetor de entradas escrevendo no arquivo de saída os pontos P = (x , y) em cada linha
    }
    fprintf(ciclo, "%d %d\n", vetor[0].x, vetor[0].y);            //Repete o primeiro ponto para fechar o ciclo
    fclose(ciclo);
}