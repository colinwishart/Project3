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

using namespace std;

/*************************CLASS DEFINITION*************************/
class AdjList {
    int numNodes; // total number of nodes for calculates

public:
    map<int, vector<pair<int, double>>> adjList;
    AdjList(); // assigns member variables default values
    void insertEdge(const int& from, const int& to, const double& weight); // adds a new link between two nodes
    void printGraph();
    void numEdges();
    pair<double, vector<int>> dfsLoop(map<int, vector<pair<int, double>>> &graph, int start,
                                        double distance, int curr_node, double& dist_traversed,
                                        vector<int> &curr_path, vector<pair<double, vector<int>>> &paths,
                                        map<int, bool> &visited, int &step_count, double &bound_factor);
};




#endif //PROJECT3_ADJLIST_H
