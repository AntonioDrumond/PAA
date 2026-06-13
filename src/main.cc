#include "Graph.h"

int main (int argc, char *argv[]) {
    Graph g(10, true);
    g.add_vert();
    g.add_vert();
    g.add_vert();
    g.add_vert();
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(2, 3);
    g.add_edge(3, 1);
    Graph* h = g.clone();
    h->reduce_transitivity_bfs();
    h->print_csacademy();
    return 0;
}
