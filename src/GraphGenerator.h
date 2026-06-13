#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

void randomGraphGenerator(int verticesqtd, std::string filename){
  std::srand(std::time(nullptr));
  std::ofstream arquivo(filename);

  if(arquivo.is_open()){
    int vertice1, vertice2;
    const int minlimit = verticesqtd;
    const int maxlimit = (verticesqtd*(verticesqtd-1)) / 2;
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

