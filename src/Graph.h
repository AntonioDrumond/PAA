#ifndef GRAPH_H
#define GRAPH_H

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <queue>
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

    void reduce_transitivity_longest_path() {
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
            for(int nei : vert_neighbors(i)) {
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

class WeightedGraph{

private:

    int n; // maximum capacity
    int last_vert; //current size
    bool directed; 
    std::vector<std::unordered_map<int, std::vector<double>>> arr; //adjacency list
    std::vector<std::string> label;

public:

    //Constructor
    WeightedGraph(int n, bool directed = false)
    : n(n), last_vert(0), directed(directed), arr(n), label(n) {}

    //Destructor
    ~WeightedGraph() = default;

    WeightedGraph* clone(void) {
        WeightedGraph* sub = new WeightedGraph(this->n);
        sub->last_vert = this->last_vert;
        sub->directed = this->directed;
        sub->arr = this->arr;
        sub->label = this->label;
        return (sub);
    }

    bool add_vert() {
        if (last_vert < n) {
            last_vert++;
            return true;
        }
        return false;
    }

    bool add_vert(std::string label) {
        if (last_vert < n) {
            this->label[last_vert++] = label;
            return true;
        }
        return false;
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

    bool add_edge(int vert1, int vert2, double weight){
        if(vert1 <= last_vert && vert2 <= last_vert){

            // Verifica se a aresta ja existe ou se esse peso ja existe
            if ( (arr[vert1].count(vert2) == 0) || (std::count(arr[vert1][vert2].begin(), arr[vert1][vert2].end (), weight) == 0) )
            { 
                arr[vert1][vert2].push_back(weight);
                if (!directed) {
                    arr[vert2][vert1].push_back(weight);
                }
                return true; // Aresta adicionada
            }
            else return false; // Vertices validos, mas ja ha aresta ou peso
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

    bool remove_edge(int vert1, int vert2)
    {
        if (check_edge(vert1, vert2)) {
            arr[vert1].erase(vert2);
            if (!directed) {
                arr[vert2].erase(vert1);
            }
            return true;
        }
        else return false;
    }

    std::vector<double> get_weight (int vert1, int vert2) {
        std::vector<double> w = std::vector<double>();
        if (check_edge(vert1, vert2)) {
            w = arr[vert1][vert2];
        }
        return (w);
    }

    bool remove_edge(int vert1, int vert2, double weight) 
    {
        if(check_edge(vert1, vert2)) 
        {
            if (arr[vert1][vert2].size() > 1) {
                std::vector<double>* weight_list = &arr[vert1][vert2];
                auto it = std::find(weight_list->begin(), weight_list->end(), weight);

                if (it != weight_list->end()) {
                    *it = weight_list->back();
                    weight_list->pop_back();
                }
                else return false; // There is no edge with the given weight

                if (!directed) {
                    std::vector<double>* weight_list = &arr[vert2][vert1];
                    auto it = std::find(weight_list->begin(), weight_list->end(), weight);

                    *it = weight_list->back();	 	// Assuming that the weight was found based on the previous check
                    weight_list->pop_back();
                }
                return true;
            }
            else if (arr[vert1][vert2][0] == weight)
            {
                arr[vert1].erase(vert2);
                if (!directed) {
                    arr[vert2].erase(vert1);
                }
                return true;
            }
        }
        return false;
    }

    int edge_number (int vert) {
        return (arr[vert].size());
    }

    std::unordered_map<int, std::vector<double>> vert_neighbors(int vert) {
        if(vert <= last_vert){
            return arr[vert];
        }
        else {
            throw std::invalid_argument("Vertex does not exist");
        }
    }

    void print_csacademy() const {
        // char *color = new char[10];
        for (int i = 0; i < last_vert; i++) {
            // snprintf(color, 9, "0x%02x%02x%02x", this->pix_color[i].r, this->pix_color[i].g, this->pix_color[i].b);
            // std::cout << i << " " << color << "\n";
            std::cout << i << "\n";
        }
        // delete[] color;
        for (int i = 0; i < last_vert; i++) {
            for (auto [neighbor, weight] : arr[i]) {
                std::cout << i << " " << neighbor << " " << weight[0] << "\n";
            }
        }
    }

    void print_raw() const {
        for (int i = 0; i < last_vert; i++) {
            if (label[i] != "") {
                std::cout << i << " : \"" << label[i] << "\" ) | ";
            }
            else {
                std::cout << i << " ) | ";
            }
            for (auto [neighbor, weight_list] : arr[i]) {
                std::cout << neighbor << "{";
                int n = weight_list.size();
                std::cout << weight_list[0];
                for (int i = 1; i < n; i++) {
                    std::cout << ", " << weight_list[i];
                }
                std::cout << "} | ";
            }
            std::cout << "\n";
        }
    }

};

#endif
