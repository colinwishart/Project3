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

//DFS function
pair<double, vector<int>> AdjList::findPaths(map<int, vector<pair<int, double>>> &graph, int start, double distance, int curr_node,
                        double &dist_traversed, vector<int> &curr_path, vector<pair<double, vector<int>>> &paths,
                        map<int, bool> &visited, int &step_count, double &bound_factor) {
    //To be returned
    pair<double, vector<int>> best_path;
    //To recursively save distance traversed
    double temp_travers = dist_traversed;

    //Iterates through each connection to the current node
    for (int i = 0; i < graph[curr_node].size(); i++) {
        //next is the id of the connection
        int next = graph[curr_node][i].first;
        //dist_to_next is the distance to the connection
        double dist_to_next = graph[curr_node][i].second;

        //Base case: if start node is found, and it is in the accepted range
        if (curr_node == start && dist_traversed > distance * (1 - bound_factor) && dist_traversed < distance * (1 + bound_factor)) {
            //Add path to set of paths
            paths.push_back(make_pair(dist_traversed, curr_path));
            //Adjust bounds to be as narrow as the new distance found; bounds narrow as more paths are found
            bound_factor = fabs((dist_traversed / distance) - 1);

            //best_path = paths.back();
            return paths.back();
        }

        //Connection is only visited if it hasn't been yet (start node is initially unvisited)
        if (!visited[next]) {
            //If only one step from the start, don't try connecting to the start
            //Avoids start->some node->start, which is not a loop
            if (next == start && step_count <= 1) {
                continue;
            }
            //If adding the distance to the connection is greater than the upper bound, don't consider it
            if (dist_traversed + dist_to_next >= distance * (1 + bound_factor)) {
                continue;
            }

            //Mark connection as visited
            visited[graph[curr_node][i].first] = true;
            //Update distance traversed
            dist_traversed += graph[curr_node][i].second;
            //Add node id to the path
            curr_path.push_back(graph[curr_node][i].first);
            //Update step count
            step_count++;
            //Recursively all function on the next node
            best_path = findPaths(graph, start, distance, graph[curr_node][i].first, dist_traversed, curr_path, paths, visited, step_count, bound_factor);

            //Undo all that to try other connections
            visited[graph[curr_node][i].first] = false;
            dist_traversed = temp_travers;
            curr_path.pop_back();
            step_count--;
        }
    }
    //If no path was found
    if (paths.empty()) {
        //Return a pair representing nothing
        return {0.0, {}};
    }
    else {
        //Otherwise, the path closest to the target distance is the last added to the list
        return paths.back();
    }
}
