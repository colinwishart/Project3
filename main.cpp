#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <iomanip>
#include <functional>
#include <chrono>
#include "AdjList.cpp"

using namespace std;

bool randBool() {
    // Seed the random number generator only once
    static std::mt19937 gen(std::time(0));

    // Create a distribution for boolean values
    std::uniform_int_distribution<> dis(0, 1);

    // Generate and return a random boolean
    return dis(gen) == 1;
}

vector<tuple<int, int, int>> generateData() {

    // calculate density to have 100,000 edges for 15-mile radius
    int dist = 10;
    int grid[325][325] = {};

    int row = 325;
    int col = 325;

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            bool rand = randBool();
            if(rand){
                grid[i][j] = 325 * i + j + 1;
            }
            cout << setw(10) << grid[i][j] << " ";
        }
        cout << "\n";
    }

    vector<tuple<int, int, int>> edges;

    for(int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            int px = 1;
            int nx = 1;
            int py = 1;
            int ny = 1;
            if(grid[i][j] != 0) {
                //cout << "{(" << j << "," << i << ")/" << grid[i][j] << "}, ";
                while (true) {
                    if (i + py >= row) {
                        break;
                    }
                    if (grid[i + py][j] != 0) {
                        edges.push_back({grid[i][j], grid[i + py][j], py * 254});
                        break;
                    }
                    py++;
                }
                while (true) {
                    if (i - ny < 0) {
                        break;
                    }
                    if (grid[i - ny][j] != 0) {
                        edges.push_back({grid[i][j], grid[i - ny][j], ny * 254});
                        break;
                    }
                    ny++;
                }
                while (true) {
                    if (j + px >= col) {
                        break;
                    }
                    if (grid[i][j + px] != 0) {
                        edges.push_back({grid[i][j], grid[i][j + px], px * 254});
                        break;
                    }
                    px++;
                }
                while (true) {
                    if (j - nx < 0) {
                        break;
                    }
                    if (grid[i][j - nx] != 0) {
                        edges.push_back({grid[i][j], grid[i][j - nx], nx * 254});
                        break;
                    }
                    nx++;
                }
            }
        }
    }
    /*
    for (const auto& edge : edges) {
        int node1, node2, distance;
        tie(node1, node2, distance) = edge;

        cout << "Node " << node1 << " to Node " << node2
             << ", Distance: " << distance << endl;
    }
     */

    return edges;
}

int main() {

    vector<tuple<int, int, int>> data = generateData();
    //cout << "\n\n"


    AdjList graph;


    for(int i = 0; i < data.size(); i ++)
    {
        //cout << get<0>(data[i]) << "/" << get<1>(data[i]) << "/" << get<2>(data[i]) << ", ";
        graph.insertEdge(get<0>(data[i]), get<1>(data[i]), get<2>(data[i]));
    }

    //graph.printGraph();
    graph.numEdges();

    map<int, bool> visited;
    int start;
    double distance;


    cout << "Enter start node: ";
    cin >> start;

    while (true) {
        auto it = graph.adjList.find(start);
        if (it != graph.adjList.end()) {
            break;
        }
        else {
            cout << "Node is not an intersection. Please select an intersection as the starting point." << endl;
            cout << "Enter start node: ";
            cin >> start;
        }
    }

    cout << "Enter run distance: ";
    cin >> distance;

    vector<int> path;
    path.push_back(start);

    vector<pair<double, vector<int>>> paths;

    double dist_traversed = 0.0;
    int step_count = 0;
    double bound_factor = 0.2;

    pair<double, vector<int>> best_path = graph.dfsLoop(graph.adjList, start, distance, start, dist_traversed, path, paths, visited, step_count, bound_factor);

    if (best_path.second.empty()) {
        cout << "No paths" << endl;
    }
    else {
        cout << "Best Path: (" << best_path.first << ", [";
        for (size_t i = 0; i < best_path.second.size(); ++i) {
            cout << best_path.second[i];
            if (i < best_path.second.size() - 1) {
                cout << ", ";
            }
        }
        cout << "])" << endl;
    }

    return 0;
}
