#include "Graph.h"
#include "GraphGenerator.h"
#include <cstring>

#define VERTS 12

int main (int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "random") == 0) {
            std::cout << "Generating a random graph" << std::endl;
            randomGraphGenerator(10, "graph.txt");
        }
    }

    Graph *g = Graph::from_file(10, "graph.txt", true);
    g->print_csacademy();

    auto e = g->reduce_transitivity_exp();
    auto cont = g->contract_sccs();

    cont.first->reduce_transitivity_bfs();
    std::cout << "CONTRACTED===================\n";
    cont.first->print_csacademy_labels();

    std::cout << "EXPAND===================\n";
    Graph *expand = Graph::expand_sccs(cont, g);
    expand->print_csacademy_labels();

    std::cout << "EXPO===================\n";
    e->print_csacademy_labels();

    delete e;
    delete cont.first;
    delete expand;
    delete g;

    return 0;
}
