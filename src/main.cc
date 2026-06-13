#include "Graph.h"

#define VERTS 12

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
