#ifndef GRAPH_H
#define GRAPH_H

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <sstream>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>

class Graph{

private:
    int n; // maximum capacity
    int last_vert; //current size
    bool directed;
    std::vector<std::unordered_set<int>> arr; //adjacency list
    std::vector<std::string> label;

public:

    //Constructor
    Graph(int n, bool directed = false)
    : n(n), last_vert(0), directed(directed), arr(n), label(n) {}

    //Destructor
    ~Graph() = default;

    static Graph* expand_sccs(std::pair<Graph *, std::vector<std::vector<int>>> &cont, Graph* original) {
        int vert_count = 0;
        for (auto scc : cont.second)
            for (int _ : scc)
                 vert_count++;
        Graph *g = new Graph(vert_count, true);
        for (; vert_count-->0;)
            g->add_vert();

        // build cycles
        for (auto scc : cont.second){
            if (scc.size() > 1) {
                for (int v=0; v < (int)scc.size()-1; v++){
                    g->add_edge(scc[v], scc[v+1]);
                }
                g->add_edge(scc[scc.size()-1], scc[0]);
            }
        }

        std::vector<std::pair<int, int>> used_pairs;

        // Connect SCCs (Better)
        for (int v1=0; v1<original->last_vert; v1++) {
            for (int v2 : original->vert_neighbors(v1)){

                int scc_v1 = which_scc(cont.second, v1);
                int scc_v2 = which_scc(cont.second, v2);

                // Avoid connecting the same pair more than once
                std::pair<int, int> pair = std::pair(scc_v1, scc_v2);
                auto find = std::find(used_pairs.begin(), used_pairs.end(), pair);

                if (scc_v1 != scc_v2 
                        && cont.first->check_edge(scc_v1, scc_v2)
                        && find == used_pairs.end()) {

                    used_pairs.push_back(pair);
                    g->add_edge(v1, v2);
                }
            }
        }
        return g;
    }

    std::pair<Graph *, std::vector<std::vector<int>>> contract_sccs() {
        auto sccs = this->find_scc();
        Graph *g = new Graph(sccs.size(), this->directed);

        // Add verts
        for (auto scc : sccs) {
            std::stringstream scc_name_ss;
            for(int v : scc) {
                scc_name_ss << v << "_";
            }
            std::string scc_name;
            scc_name_ss >> scc_name;
            g->add_vert(scc_name);
        }

        // std::vector<std::pair<int, int>> all_edges;
        // all_edges.reserve(last_vert * (last_vert - 1));

        // Add edges to  contracted graph
        for (int v1=0; v1<last_vert; v1++) {
            for(int v2 : vert_neighbors(v1)) {
                // all_edges.push_back({v1, v2});
                int scc_v1 = which_scc(sccs, v1),
                    scc_v2 = which_scc(sccs, v2);
                if (scc_v1 != scc_v2) {
                    g->add_edge(scc_v1, scc_v2);
                }
            }
        }

        return {g, sccs};
    }

    std::vector<std::vector<int>> find_scc() {
        std::vector<std::vector<int>> sccs;
        sccs.reserve(last_vert);
        std::vector<int> disc(this->last_vert, -1);
        std::vector<int> low(this->last_vert, -1);
        std::vector<bool> inSt(this->last_vert, false);
        std::stack<int> st;
        int timer = 0;

        for (int v=0; v<this->last_vert; v++) {
            if (disc[v] == -1) find_scc_rec(v, disc, low, inSt, st, timer, sccs);
        }

        return sccs;
    }

    static Graph* from_file(int n, const char *const filename, bool directed = false) {
        Graph* g = new Graph(n, directed);
        std::ifstream f;
        f.open(filename);
        std::vector<std::pair<int, int>> all_edges;
        while (!f.eof()) {
            int v1, v2;
            f >> v1 >> v2;
            all_edges.push_back({v1, v2});
        }
        int biggest_vert = -1;
        for (std::pair<int, int> i : all_edges) {
            biggest_vert = std::max(biggest_vert, i.first);
            biggest_vert = std::max(biggest_vert, i.second);
        }
        for(int i=0; i<=biggest_vert; i++) g->add_vert();
        for (std::pair<int, int> i : all_edges) {
            g->add_edge(i.first, i.second);
        }
        return g;
    }

    void reduce_transitivity_bfs() {
        int counter = 0;
        std::vector<std::pair<int, int>> all_edges;
        all_edges.reserve(last_vert * (last_vert - 1));

        for (int i=0; i<last_vert; i++) {
            for(int nei : vert_neighbors(i)) {
                all_edges.push_back({i, nei});
            }
        }

        /*
        std::sort(all_edges.begin(), all_edges.end(),
            [this](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return comp_degree(a, b);
        });
        */

        for (std::pair<int, int> edge : all_edges) {
            if (check_edge(edge.first, edge.second)) {
                std::cout << "Evaluating (" << edge.first << ", " << edge.second << ") -> ";
                if (special_bfs(edge.first, edge.second)) {
                    std::cout << "Redundant\n";
                    remove_edge(edge.first, edge.second);
                    if (!this->directed)
                        remove_edge(edge.second, edge.first);
                    counter++;
                }
                else 
                    std::cout << "Necessary\n";
            }
        }

        std::cout << "Edges removed: " << counter << "\n";
    }

    int vert_count() {
        return last_vert;
    }

    int edge_count() {
        int counter = 0;
        for (int i=0; i<last_vert; i++) {
            for(int _ : vert_neighbors(i)) {
                counter++;
            }
        }
        return counter;
    }

    Graph* reduce_transitivity_exp() {
        std::vector<std::pair<int, int>> all_edges;
        all_edges.reserve(edge_count());

        for (int i=0; i<last_vert; i++) {
            for (int nei : arr[i]) {
                if (directed || i <= nei) {
                    all_edges.push_back({i, nei});
                }
            }
        }

        auto reachable = [](Graph* graph, int starting_vert, int target_vert) {
            if (starting_vert < 0 || target_vert < 0 ||
                starting_vert >= graph->last_vert || target_vert >= graph->last_vert) {
                return false;
            }

            if (starting_vert == target_vert) {
                return true;
            }

            std::vector<bool> visited(graph->last_vert, false);
            std::queue<int> q;

            visited[starting_vert] = true;
            q.push(starting_vert);

            while (!q.empty()) {
                int current = q.front();
                q.pop();

                for (int neighbor : graph->arr[current]) {
                    if (neighbor < 0 || neighbor >= graph->last_vert || visited[neighbor]) {
                        continue;
                    }

                    if (neighbor == target_vert) {
                        return true;
                    }

                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }

            return false;
        };

        Graph current(this->n, this->directed);
        current.last_vert = this->last_vert;
        current.label = this->label;

        Graph* best = this->clone();
        int best_edge_count = best->edge_count();

        auto search = [&](auto&& self, int edge_index) -> void {
            if (current.edge_count() >= best_edge_count) {
                return;
            }

            if (edge_index == (int) all_edges.size()) {
                for (std::pair<int, int> edge : all_edges) {
                    if (!reachable(&current, edge.first, edge.second)) {
                        return;
                    }
                }

                delete best;
                best = current.clone();
                best_edge_count = best->edge_count();
                return;
            }

            self(self, edge_index + 1);

            current.add_edge(all_edges[edge_index].first, all_edges[edge_index].second);
            self(self, edge_index + 1);
            current.remove_edge(all_edges[edge_index].first, all_edges[edge_index].second);
        };

        search(search, 0);
        return best;
    }

    Graph* clone(void) {
        Graph* sub = new Graph(this->n);
        sub->last_vert = this->last_vert;
        sub->directed = this->directed;
        sub->arr = this->arr;
        sub->label = this->label;
        return (sub);
    }

    bool add_vert(){
        if(last_vert < n){
            last_vert++;
            return true;
        }
        else return false;
    }

    bool add_vert(std::string label){
        if(last_vert < n){
            this->label[last_vert] = label;
            last_vert++;
            return true;
        }
        else return false;
    }

    void setLabel(int vert, std::string label)
    {
        if (vert <= last_vert) {
            this->label[vert] = label;
        } 
        else {
            throw std::invalid_argument("Vertex does not exist");
        }
    }

    std::string getLabel(int vert)
    {
        if (vert <= last_vert) {
            return (this->label[vert]);
        }
        else {
            throw std::invalid_argument("Vertex does not exist");
        }
    }

    void all_verts(){
        while(add_vert()); // Adicionar todos os vertices possiveis
    }

    std::unordered_set<int> vert_neighbors(int vert) {
        if(vert <= last_vert){
            return arr[vert];
        }
        else {
            throw std::invalid_argument("Vertex does not exist");
        }
    }

    bool add_edge(int vert1, int vert2){
        if(vert1 <= last_vert && vert2 <= last_vert){
            if(arr[vert1].count(vert2) == 0){ // Verifica se a aresta ja existe
                arr[vert1].insert(vert2);
                if (!directed) {
                    arr[vert2].insert(vert1);
                }
                return true; // Aresta adicionada
            }
            else return false; // Vertices validos, mas ja ha aresta
        }
        else return false; // Vertices invalidos
    }

    bool check_edge(int vert1, int vert2){
        if(vert1 <= last_vert && vert2 <= last_vert){
            if (!directed) {
                return (arr[vert1].count(vert2) > 0) && (arr[vert2].count(vert1) > 0);
            }
            else {
                return (arr[vert1].count(vert2) > 0);
            }
        }
        else return false; // Vertices invalidos
    }

    bool remove_edge(int vert1, int vert2){
        if(check_edge(vert1, vert2)){
            arr[vert1].erase(vert2);
            if (!directed) {
                arr[vert2].erase(vert1);
            }
            return true;
        }
        else return false;
    }

    int edge_number (int vert) {
        return (arr[vert].size());
    }

    void test(){
        std::cout << "n = " << this->n << "\n";
        std::cout << "last_vert = " << this->last_vert << "\n";
    }

    // Prints the graph in a format recognized by https://csacademy.com/app/graph_editor/
    void print_csacademy(){
        for(int i=0; i<last_vert; i++){
            std::cout << i << "\n";
        }
        for(int i=0; i<last_vert; i++){
            for(int neighbor : arr[i]){
                std::cout << i << " " << neighbor << "\n";
            }
        }
    }

    void print_csacademy_labels(){
        for(int i=0; i<last_vert; i++){
            std::string l = this->getLabel(i);
            if(l != "")
                std::cout << l << "\n";
            else 
                std::cout << i << "\n";
        }
        for(int i=0; i<last_vert; i++){
            for(int neighbor : arr[i]){
                std::string l1 = this->getLabel(i);
                std::string l2 = this->getLabel(neighbor);
                if (l1 != "") 
                    std::cout << l1 << " ";
                else
                    std::cout << i << " ";
                if (l2 != "") 
                    std::cout << l2 << "\n";
                else
                    std::cout << neighbor << "\n";
            }
        }
    }

    void print_raw(){
        for(int i=0; i<last_vert; i++) {
            if (label[last_vert] != "") {
                std::cout << i << " ) ";
            }
            else {
                std::cout << i << " : \"" << label[i] << "\" ) ";
            }
            printVec(arr[i]);
        }
    }

private:

    static int which_scc(std::vector<std::vector<int>> &sccs, int target) {
        for (int scc=0; scc < (int)sccs.size(); scc++) {
            for (int v : sccs[scc]) {
                if (target == v) return scc;
            }
        }
        return -1;
    }

    void find_scc_rec(
        int v, // Current vertex
        std::vector<int> &disc,
        std::vector<int> &low,
        std::vector<bool> &inSt,
        std::stack<int> &st,
        int &timer,
        std::vector<std::vector<int>> &sccs
    ) {
        disc[v] = low[v] = ++timer;
        st.push(v);
        inSt[v] = true;

        //std::cout << "vertex " << v << " -> ";
        for (int nei : this->vert_neighbors(v)) {
            //std::cout << nei << " ";
            if (disc[nei] == -1) {
                find_scc_rec(nei, disc, low, inSt, st, timer, sccs);
                low[v] = std::min(low[v], low[nei]); // Check if subtree rooted in nei
                                                // has a connection with one of v's ancestors
            }
            else if (inSt[nei]) {
                low[v] = std::min(low[v], disc[nei]);
            }
        }
        //std::cout << "\n";

        if (low[v] == disc[v]) {
            std::vector<int> scc;
            bool loop = true;
            while (loop) {
                int x = st.top();
                st.pop();
                inSt[x] = false;
                scc.push_back(x);
                if (x == v) loop = false;
            }
            sccs.push_back(scc);
        }
    }

    bool comp_degree(const std::pair<int, int>& a, const std::pair<int, int>& b) {
        int degree_a = edge_number(a.first) + edge_number(a.second);
        int degree_b = edge_number(b.first) + edge_number(b.second);
        return degree_a > degree_b;
    }

    bool special_bfs(int starting_vert, int target_vert) {
        if (starting_vert < 0 || target_vert < 0 ||
            starting_vert >= last_vert || target_vert >= last_vert) {
            return false;
        }

        if (starting_vert == target_vert) {
            return true;
        }

        std::vector<bool> visited(last_vert, false);
        std::queue<int> q;

        visited[starting_vert] = true;
        q.push(starting_vert);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int neighbor : arr[current]) {
                if (neighbor < 0 || neighbor >= last_vert) {
                    continue;
                }

                bool ignored_edge =
                    (current == starting_vert && neighbor == target_vert) ||
                    (current == target_vert && neighbor == starting_vert);

                if (ignored_edge || visited[neighbor]) {
                    continue;
                }

                if (neighbor == target_vert) {
                    return true;
                }

                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
        return false;
    }

    void printVec(std::unordered_set<int> v){
        std::cout << "| ";
        for(int neighbor : v){
            std::cout << neighbor << " | ";
        }
        std::cout << "\n";
    }
};

#endif
