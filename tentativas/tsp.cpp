#/*-------UNIVERSIDADE TECNOLÓGICA FEDERAL DO PARANÁ-------*/
/*----------BACHARELADO EM SISTEMAS DE INFORMAÇÃO---------*/
/*-------------PROJETO E ANÁLISE DE ALGORITMOS------------*/

/* ALUNO: SAULO GREGÓRIO DE ANDRADE BERGAMO - RA 2023997  */

/*________________________________________________________*/
/*-------------PROBLEMA DO CAIXEIRO VIAJANTE--------------*/
/*  Algoritmo de aproximação para o problema do caixeiro  */
/* 	viajante. Encontrar o caminho hamiltoniano de custo   */
/*     mínimo dentre n pontos, utilizando a distância     */
/*   euclidiana como peso das arestas, Árvore Geradora    */
/*                  Mínima através do                     */
/*  algoritmo de PRIM e busca em profundidade no subgrafo */

#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>
#include<bits/stdc++.h> 

using namespace std;

#define N 30
#define FILHOE(x) (2 * x) + 1
#define FILHOD(x) (2 * x) + 2
#define PAI(x) (x - 1) / 2


static int tam;

typedef struct vertice{
    int x;
    int y;
}Vertice;

typedef struct aresta{
    Vertice atual;
    float peso_dist;
    Vertice* pai;
}Aresta;

typedef struct heap{
    int tam_h;
    int pos;
    Aresta* topo;
}Heap;

vector<Vertice*> abrir_arquivo(char* arquivo);
void imprime_vetor(vector<Vertice*> vetor);
priority_queue<Aresta> cria_min_heap(vector<Vertice*> vetor);

Heap* cria_heap();
void insere_heap(Heap* heap, Aresta no);
void insere_tudo_heap(Heap* heap, vector<Vertice*> vetor);
void heapfY(Heap* heap, int pos);
void heapfyd(Aresta* topo, int novo, int pos);
Aresta* remove_heap(Heap* heap);
void libera_heap(Heap* heap);




int main(int argc, char* argv[]){
    
    vector<Vertice*> vetor = abrir_arquivo(argv[1]);
    imprime_vetor(vetor);

    Heap* grafoheap = cria_heap();

    //insere_tudo_heap(grafoheap, vetor);
    for(int i = 0; i < vetor.size(); i++){
        Aresta nova;
        nova.atual.x = vetor[i]->x;
        nova.atual.y = vetor[i]->y;
        nova.pai = vetor[0];
        nova.peso_dist = sqrt(pow(vetor[0]->x - vetor[i]->x, 2) + pow(vetor[0]->y - vetor[i]->y , 2));  
        //Aresta* nova = new Aresta;
        /* nova->atual.x = vetor[i]->x;
        nova->atual.y = vetor[i]->y;
        nova->pai = vetor[0];
        nova->peso_dist = sqrt(pow(vetor[0]->x - vetor[i]->x, 2) + pow(vetor[0]->y - vetor[i]->y , 2));  */
        grafoheap->topo[grafoheap->pos] = nova;
        heapfY(grafoheap, grafoheap->pos);
        grafoheap->pos++;
    }

    return 0;
}

vector<Vertice*> abrir_arquivo(char* arquivo){
    FILE* entrada = fopen(arquivo, "r");
    char line[N];
    char* e;
    if(entrada){
        fgets(line, N, entrada);
        tam = atoi(line);
    }
    else{
        cout << "Não foi possível abrir o arquivo";
        exit(-1);
    }
    vector<Vertice*> vetor;
    int x, y;
    
    for(int i = 0; i < tam; i++){
        fgets(line, N, entrada);
        Vertice* temp = new Vertice;
        temp->x = (int)strtof(line, &e);
        temp->y = (int)strtof(e, NULL);
        vetor.push_back(temp);
    }
    fclose(entrada);
    return vetor;
}

void imprime_vetor(vector<Vertice*> vetor){
    for(int i = 0; i < tam; i++){
        cout << "(" << vetor[i]->x << ", " << vetor[i]->y << ") ";
    }
    cout << endl;
}

priority_queue<Aresta> cria_min_heap(vector<Vertice*> vetor){
    
   /*  priority_queue<Aresta> heap_min;
    Aresta arco;
    for(int i = 1; i < tam; i++){
        arco.atual = *vetor[i];
        arco.pai = *vetor[0];
        arco.peso_dist = sqrt(pow(vetor[0]->x - vetor[i]->x, 2) + pow(vetor[0]->x - vetor[i]->y, 2));
        heap_min.push(arco);
    }    
    return heap_min; */
}

Heap* cria_heap(){
    Heap* heap = new(Heap);
    heap->tam_h = tam;
    heap->pos = 0;
    heap->topo = NULL;
    return heap;
}


void heapfY(Heap* heap, int pos){
    int pai;
    while(pos > 0){
        pai = PAI(pos);
        if(heap->topo[pai].peso_dist < heap->topo->peso_dist){
            Aresta temp = heap->topo[pos];
            heap->topo[pos] = heap->topo[pai];
            heap->topo[pai] = temp;
        }
        else break;
        pos = pai; 
    }
}

void heapfyd(Aresta* topo, int novo, int pos){
    int pai = novo;
    int fe, fd, f;
    while(FILHOE(pai) < pos){
        fe = FILHOE(pai);
        fd = FILHOD(pai);
        if(fd >= pos)fd = fe;
        if(topo[fe].peso_dist > topo[fd].peso_dist) f = fe;
        else f = fd;
        if(topo[pai].peso_dist < topo[f].peso_dist){
            Aresta temp = topo[pai];
            topo[pai] = topo[f];
            topo[f] = temp;
        }
        else break;
        pai = f;
    }
}

void insere_heap(Heap* heap, Aresta no){
    if(heap->pos < heap->tam_h){
        heap->topo[heap->pos] = no;
        heapfY(heap, heap->pos);
        heap->pos++;
    }
    else{
        cout << "Erro: Heap cheio" << endl;
    }
}

void insere_tudo_heap(Heap* heap, vector<Vertice*> vetor){
    Aresta *temp = new(Aresta);
    
    //temp->atual = (Vertice)vetor.begin() ;
    //temp.pai = vetor[0];
    for(int i = 1; i < vetor.size(); i++){
    /*     temp.pai = (Vertice)vetor[i];
        temp.peso_dist = sqrt(pow(vetor[0]->x - vetor[i]->x, 2) + pow(vetor[0]->y - vetor[i]->y , 2));
        heap->topo[heap->pos] = temp; */
        heapfY(heap, heap->pos);
        heap->pos++;
    }
}

Aresta* remove_heap(Heap* heap){
    
    if(heap->pos > 0){
        Aresta* temp = heap->topo;//[0];
        heap->topo[0] = heap->topo[heap->pos-1];
        heap->pos--;
        heapfyd(heap->topo, 0, heap->pos);
        return temp;
    }
    else{
        cout << "Erro: Heap vazio" << endl;
        exit(-1);
    }
}

void libera_heap(Heap* heap);