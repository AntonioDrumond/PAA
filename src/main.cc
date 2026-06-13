#include "Graph.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

#define VERTS 12

void randomGraphGenerator(int verticesqtd, std::string filename){
  std::srand(std::time(nullptr));
  std::ofstream arquivo(filename);

  if(arquivo.is_open()){
    int vertice1, vertice2;
    const int minlimit = verticesqtd;
    const int maxlimit = verticesqtd*(verticesqtd-1);
    int arestasqtd = (minlimit + rand()%(maxlimit+minlimit));
    for(int i = 0; i < arestasqtd; i++){
      vertice1 = rand() % verticesqtd;
      do {
        vertice2 = rand() % verticesqtd;
      }while (vertice2 == vertice1);
      arquivo << vertice1 << " " << vertice2 << std::endl;
    }
    arquivo.close();
  } else {
    std::cout << "Error";
  }
}

int main (int argc, char *argv[]) {
    Graph g(50, true);
    for (int i=0; i<=VERTS; i++) {
        g.add_vert();
    }

    g.add_edge(0, 1);
    g.add_edge(2, 0);
    g.add_edge(2, 3);
    g.add_edge(3, 2);
    g.add_edge(4, 3);
    g.add_edge(4, 2);
    g.add_edge(0, 5);
    g.add_edge(5, 4);
    g.add_edge(3, 5);
    g.add_edge(6, 0);
    g.add_edge(6, 4);
    g.add_edge(6, 9);
    g.add_edge(7, 8);
    g.add_edge(8, 7);
    g.add_edge(7, 6);
    g.add_edge(9, 10);
    g.add_edge(10, 12);
    g.add_edge(11, 12);
    g.add_edge(12, 9);
    g.add_edge(9, 11);
    g.add_edge(11, 4);
    g.add_edge(8, 9);

    Graph* h = g.clone();
    h->reduce_transitivity_bfs();
    h->print_csacademy();
    return 0;
}
