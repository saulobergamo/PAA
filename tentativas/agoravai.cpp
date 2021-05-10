#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>
#include<bits/stdc++.h> 


using namespace std;

static int tam;

class Node {
    public:
        int nodeX;
        int nodeY;

        Node(int x, int y) {
            nodeX = x;
            nodeY = y;
        }
        get_x()return nodeX;

};

class Graph{
    public: 
        vector<vector<float>> matrix;
};

void imprimevetor(vector<Node*> vetor){
    for(auto i = vetor.begin(); i != vetor.end(); i++){
        cout << (*i)->nodeX << ","<<(*i)->nodeY<< endl; 
    }

}

vector<Node*> openfile(char* arquivo){
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


    vector<Node*> vetor;
    
    for(int i = 0; i < tam; i++){
        fgets(line, n, entrada);
        int x = (int)strtof(line, &space);
        int y = (int)strtof(space, NULL);
        Node* teste = new Node (x,y);
        vetor.push_back(teste);
    }

    fclose(entrada);
    return vetor;
}

void imprimematriz(Graph* grafomatrix){
     
     for(auto i = grafomatrix->matrix.begin(); i != grafomatrix->matrix.end(); i++){
        for(auto j = (*i).begin(); j != (*i).end(); j++){
            cout << (*j) << endl;
        }
    }
}

void listarMatriz(Graph* grafomatrix){
     
     for(auto i = grafomatrix->matrix.begin(); i != grafomatrix->matrix.end(); i++){
        for(auto j = (*i).begin(); j != (*i).end(); j++){
           //ação
        }
    }
}

Graph* gerarMatriz(vector<Node*> vetor){
    Graph* grafo = new Graph();
    
    int countRow = 0;
    int countColumn = 0;

    for(auto i = vetor.begin(); i != vetor.end(); i++){
        vector<float> adj;
        for(auto j = i + 1 ; j != vetor.end(); j++){
            adj.push_back(sqrt(pow((*i)->nodeX - (*j)->nodeX, 2) + pow((*i)->nodeY - (*j)->nodeY, 2)));
            countColumn++;
        }
        grafo->matrix.push_back(adj);
        countRow++;
    }
    return grafo;
};




/* void prim() {
    int total_weight = 0;
    
    vector<bool> selected(n, false);
    vector<Edge> min_e(n);
    min_e[0].w = 0;

    for (int i=0; i<n; ++i) {
        int v = -1;
        for (int j = 0; j < n; ++j) {
            if (!selected[j] && (v == -1 || min_e[j].w < min_e[v].w))
                v = j;
        }

        if (min_e[v].w == INF) {
            cout << "Sem MST!" << endl;
            exit(0);
        }

        selected[v] = true;
        total_weight += min_e[v].w;
        if (min_e[v].to != -1)
            cout << v << " " << min_e[v].to << endl;

        for (int to = 0; to < n; ++to) {
            if (adj[v][to] < min_e[to].w)
                min_e[to] = {adj[v][to], v};
        }
    }

    cout << total_weight << endl;
} */

int main(int argc, char* argv[]){
    //abrir o arquivo com os pontos
        
    //Criar node 
    //Gerar Edges
    //Criar grafo
    
    vector<Node*> novo = openfile(argv[1]);
    imprimevetor(novo);
    /* Graph* grafo = gerarMatriz(novo);
    imprimematriz(grafo);
    cout << "Grafo: " <<grafo->matrix[0][2] << endl;
 */
    make_heap(novo.begin(), novo.end());
    cout << endl; 
    imprimevetor(novo);
    
    return 0;
}
        