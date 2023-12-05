//
// Created by Colin Wishart on 11/27/2023.
//

#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <queue>
#include "AdjList.h"
#include <vector>
#include <algorithm>


// cited from lecture slides 8a (slide 56)
void AdjList::insertEdge(const int& from, const int& to, const int& weight)
{
    adjList[from].push_back(make_pair(to, weight)); // add from->to pair to adjList

    // If the from vertex doesn't exist in the adjList yet, add it and initialize its vector so all vertices exist in all maps
    if (adjList.find(to)==adjList.end())
    {
        adjList[to] = {};
    }
}

// Prints the console representation of the nodes
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
// Prints the number of edges in the graph
void AdjList::numEdges()
{
    int tot = 0;
    for(auto& vertex : adjList)
    {
        tot += vertex.second.size();
    }
    cout << "Number of edges: " << tot << endl;
}

pair<int, vector<int>> AdjList::dfsLoop(map<int, vector<pair<int, int>>> &graph, int start,
                                           int distance, int curr_node, int &dist_traversed,
                                           vector<int> &curr_path, vector<pair<int,
        vector<int>>> &paths, map<int, bool> &visited,
                                           int &step_count, double &bound_factor) {

    //To be returned
    pair<int, vector<int>> best_path;
    //To recursively save distance traversed
    int temp_travers = dist_traversed;

    //Iterates through each connection to the current node
    for (int i = 0; i < graph[curr_node].size(); i++) {
        //next is the id of the connection
        int next = graph[curr_node][i].first;
        //dist_to_next is the distance to the connection
        int dist_to_next = graph[curr_node][i].second;

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
            best_path = dfsLoop(graph, start, distance, graph[curr_node][i].first,
                                dist_traversed, curr_path, paths, visited, step_count,
                                bound_factor);

            //Undo all that to try other connections
            visited[graph[curr_node][i].first] = false;
            dist_traversed = temp_travers;
            curr_path.pop_back();
            step_count--;
        }
    }
    if (paths.empty()) {
        return {0.0, {}};
    }
    else {
        return paths.back();
    }
}


// Helper function that adds to the min distance to map and predecessor map, computes Dijkstras shortest path from the starting point
void AdjList::dijkstra(int start, unordered_map<int, int>& dist, unordered_map<int, int>& predecessor) {
    unordered_set<int> visited; // visited vertices
    unordered_set<int> toVisit; // vertices to visit

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min; // min heap for relaxing edges

    // iterates through the adjacency list and fills toVisit, predecessor, and dist structures
    for(const auto i: adjList) {
        toVisit.insert(i.first);
        predecessor[i.first] = -1;
        if(i.first == start) {
            dist[i.first] = 0;
        }
        else {
            dist[i.first] = INT_MAX;
        }
    }
    // add starting value to min heap
    min.push(make_pair(0, start));

    // run until min heap is empty
    while(!toVisit.empty()) {
        // extract min and pop, mark visited
        int current = min.top().second;
        visited.insert(current);
        toVisit.erase(current);
        min.pop();

        // For all the current vertices, if unvisited, try to relax
        for(int i = 0; i < adjList[current].size(); i++){
            if(visited.find(adjList[current][i].first) == visited.end()) {
                if (dist[adjList[current][i].first] > dist[current] + adjList[current][i].second) {
                    dist[adjList[current][i].first] = dist[current] + adjList[current][i].second;
                    predecessor[adjList[current][i].first] = current;
                    min.push(make_pair(dist[adjList[current][i].first], adjList[current][i].first));
                }
            }
        }
    }
}

// Helper function that adds computes Dijkstras shortest path from a point to the start, excluding the nodes already visited in the path to the point
pair<int, vector<int>> AdjList::dijkstraHelper(int start, int target, unordered_set<int>& visitedNodes) {
    unordered_map<int, int> dist; // distance to vertices
    unordered_map<int, int> predecessor; // predecessor of vertices
    unordered_set<int> visited; // visited vertices
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min; // min heap

    // iterates through the adjacency list and fills predecessor and dist structures
    for(const auto& i: adjList) {
        predecessor[i.first] = -1;
        if(i.first == start) {
            dist[i.first] = 0;
        }
        else {
            dist[i.first] = INT_MAX;
        }
    }
    // pushes first vertex to min heap
    min.push(make_pair(0, start));

    // runs until the min heap is empty
    while(!min.empty()) {
        // extract min and pop, mark visited
        int current = min.top().second;
        visited.insert(current);
        min.pop();

        // skip already visited nodes
        if(visitedNodes.count(current)) {
            continue;
        }

        // For all the current vertices, if unvisited, try to relax
        for(int i = 0; i < adjList[current].size(); i++){
            pair<int, int> adjNode = adjList[current][i];
            if (dist[adjNode.first] > dist[current] + adjNode.second && !visited.count(adjNode.first)) {
                dist[adjNode.first] = dist[current] + adjNode.second;
                predecessor[adjNode.first] = current;
                min.push(make_pair(dist[adjNode.first], adjNode.first));
            }
        }
    }
    // Creates a pair with the distance of the path and the order of the vertices in the path, tracing back through predecessors
    pair<int, vector<int>> path;
    path.first = dist[target];
    int current = target;
    while(current != start) {
        path.second.push_back(predecessor[current]);
        current = predecessor[current];
    }
    return path;
}
// Returns the first loop in a graph withing a 10% threshold of the target distance
pair<int, vector<int>> AdjList::dijLoop(int start, int target) {

    // initializing the return graph
    vector<int> init;
    pair<int, vector<int>> path = make_pair(INT_MAX, init);

    unordered_map<int, int> dist; // shortest distance to all nodes in the graph
    unordered_map<int, int> predecessor; // predecessors of nodes in the shortest distance paths

    // gets the dijkstra's shortest path
    dijkstra(start, dist, predecessor);

    // iterates through every vertex in the graph
    for(auto i : dist) {
        unordered_set<int> visited; // holds nodes visit in the first "half" of the loop
        pair<int, vector<int>> fromStart; // holds distance and vertices in the first "half" of the loop.

        // Creates a pair with the distance of the path and the order of the vertices in the path, tracing back through predecessors
        fromStart.first = i.second;
        fromStart.second.push_back(i.first);
        int current = i.first;
        while(current != start) {
            visited.insert(current);
            fromStart.second.push_back(predecessor[current]);
            current = predecessor[current];
        }

        // reverse the order of the vertices in the path to be in correct order
        reverse(fromStart.second.begin(), fromStart.second.end());

        // gets the second "half" of the path, from a vertex on the graph to the start
        pair<int, vector<int>> toStart = dijkstraHelper(start, i.first, visited);
        // adds the second half of the path to the first half of the path
        fromStart.second.insert( fromStart.second.end(), toStart.second.begin(), toStart.second.end());
        // if the total distance of the path is within the 10% threshold and the path is valid, return the path
        if(toStart.first + fromStart.first > target * 0.9 && toStart.first + fromStart.first < target * 1.1) {
            path = make_pair(toStart.first + fromStart.first, fromStart.second);
            if(path.second.size() > 3) {
                return path;
            }
        }
    }
    path.second = {};
    return path;
}
