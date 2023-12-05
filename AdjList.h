//
// Created by Colin Wishart on 11/27/2023.
//

#ifndef PROJECT3_ADJLIST_H
#define PROJECT3_ADJLIST_H

/***************************DEPENDENCIES***************************/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <math.h>
#include <vector>
#include <unordered_set>


using namespace std;

/*************************CLASS DEFINITION*************************/

class AdjList {

public:

    map<int, vector<pair<int, int>>> adjList;

    void insertEdge(const int& from, const int& to, const int& weight); // adds a new link between two nodes
    void printGraph();
    void numEdges();
    void dijkstra(int start, unordered_map<int, int>& dist, unordered_map<int, int>& predecessor);
    pair<int, vector<int>> dijkstraHelper(int start, int target, unordered_set<int>& visitedNodes);
    pair<int, vector<int>> dijLoop(int start, int target);

    pair<int, vector<int>> dfsLoop(map<int, vector<pair<int, int>>> &graph, int start,
                                      int distance, int curr_node, int& dist_traversed,
                                      vector<int> &curr_path, vector<pair<int, vector<int>>> &paths,
                                      map<int, bool> &visited, int &step_count, double &bound_factor);
};


#endif //PROJECT3_ADJLIST_H
