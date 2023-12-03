#ifndef PROJECT3_INTERSECTIONGRAPH_H
#define PROJECT3_INTERSECTIONGRAPH_H

/***************************DEPENDENCIES***************************/
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>
#include <bits/stdc++.h>

/*************************CLASS DEFINITION*************************/
class IntersectionGraph {
private:
    struct Node {
        double lat;
        double lon;
    };

    std::unordered_map<std::string, std::vector<std::string>> edgeList; //Maps id to vector of linked ids
    std::unordered_map<std::string, Node> nodeMap; //Maps id to its corresponding node
    std::vector<std::string> idVect; //Vector of ids in graph
    int totalDeleted;
    int totalLinked;

    void InsertNode(std::string id, double lat, double lon);

    void DeleteCloseNodes();
    void LinkAllNodes();
    double DirectDistanceBetween(std::string id1, std::string id2);

public:
    IntersectionGraph();

    void PopulateFromFile(std::string filename);

    void WriteNodesToFile(std::string filename);

    void PrintNodes();
};

#endif //PROJECT3_INTERSECTIONGRAPH_H
