#include "Graph.h"
#include "GraphGenerator.h"

#define VERTS 12

int main (int argc, char *argv[]) {
    // Graph *g = new Graph(10, true);
    // randomGraphGenerator(6, "graph.txt");
    Graph *g = Graph::from_file(10, "graph.txt", true);
    g->print_csacademy();
    auto sccs = g->find_scc();
    for (std::vector<int> &scc : sccs) {
        std::cout << "[ ";
        for (int j : scc) {
            std::cout << "(" << j << ") ";
        }
        std::cout << "]\n";
    }
    auto cont = g->contract_sccs();
    std::cout << "===================\n";
    cont.first->print_csacademy_labels();
    std::cout << "===================\n";
    Graph *expand = Graph::expand_sccs(cont);
    expand->print_csacademy_labels();
    delete cont.first;
    delete expand;
    /*
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
    delete g;
    return 0;
}
