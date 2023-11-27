//
// Created by Colin Wishart on 11/27/2023.
//

#include <iostream>
#include <iomanip>
#include "AdjList.h"

AdjList::AdjList()
{
    numNodes = 0; // starting number of nodes is zero
}

// cited from lecture slides 8a (slide 56)
void AdjList::insertEdge(const int& from, const int& to, const int& weight)
{
    adjList[from].push_back(make_pair(to, weight)); // add from->to pair to adjList

    // If the from vertex doesn't exist in the adjList yet, add it and initialize its vector so all vertices exist in all maps
    if (adjList.find(to)==adjList.end())
    {
        adjList[to] = {};
        numNodes++;
    }
}

void AdjList::printGraph()
{
    for(auto& vertex : adjList)
    {
        cout << vertex.first << ": ";
        for (int i = 0; i < vertex.second.size(); i++)
        {
            cout << vertex.second[i].first << "(" <<vertex.second[i].second << ") ";
        }
        cout << endl;
    }
}

void AdjList::numEdges()
{
    int tot = 0;
    for(auto& vertex : adjList)
    {
        tot += vertex.second.size();
    }
    cout << "Number of edges: " << tot << endl;
}
