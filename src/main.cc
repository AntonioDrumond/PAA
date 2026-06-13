#include "Graph.h"
#include "GraphGenerator.h"

#define VERTS 12

int main (int argc, char *argv[]) {
    randomGraphGenerator(6, "graph.txt");
    Graph *g = Graph::from_file(8, "graph.txt", true);
    g->print_csacademy();
    auto e = g->reduce_transitivity_exp();
    g->reduce_transitivity_bfs();

    if (g->edge_count() == e->edge_count()) {
        g->print_csacademy();
        std::cout << "Equal";
    }
    else {
        g->print_csacademy();
        std::cout << "===================\n";
        e->print_csacademy();
        std::cout << "Different: Ours = " << g->edge_count() << " | Other = " << e->edge_count();
    }
    return 0;
}
