#include "Graph.h"

#define VERTS 10

int main (int argc, char *argv[]) {
    /*
    Graph g(50, true);
    for (int i=0; i<=VERTS; i++) {
        g.add_vert();
    }

    g.add_edge(3, 1);
    g.add_edge(0, 3);
    g.add_edge(0, 1);
    g.add_edge(1, 0);
    g.add_edge(4, 1);
    g.add_edge(4, 0);
    g.add_edge(3, 5);
    g.add_edge(5, 4);
    g.add_edge(1, 5);

    Graph* h = g.clone();
    auto e = h->reduce_transitivity_exp();
    e->print_csacademy();
    */

    Graph *g = Graph::from_file(50, "test.txt", true);
    g->print_csacademy();
    return 0;
}
