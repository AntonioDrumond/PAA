#include "Graph.h"
#include "GraphGenerator.h"

#define VERTS 12

int main (int argc, char *argv[]) {
    // Graph *g = new Graph(10, true);
    randomGraphGenerator(10, "graph.txt");
    Graph *g = Graph::from_file(10, "graph.txt", true);
    g->print_csacademy();
    g->reduce_transitivity_bfs();
    g->print_csacademy_labels();

    delete g;
    /*
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
    */
    return 0;
}
