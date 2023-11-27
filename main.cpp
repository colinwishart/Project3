#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <functional>
#include <chrono>
#include "AdjList.h"

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
    int grid[10][10] = {};

    int row = 10;
    int col = 10;

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            bool rand = randBool();
            if(rand){
                grid[i][j] = 10 * i + j + 1;
            }
        }
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
                    if (i - nx < 0) {
                        break;
                    }
                    if (j - nx >= 0 && grid[i][j - nx] != 0) {
                        edges.push_back({grid[i][j], grid[i][j - nx], nx * 254});
                        break;
                    }
                    nx++;
                }
            }
        }
    }

    return edges;
}

int main() {

    vector<tuple<int, int, int>> data = generateData();
    //cout << "\n\n";

    AdjList graph;

    for(int i = 0; i < data.size(); i ++)
    {
        //cout << get<0>(data[i]) << "/" << get<1>(data[i]) << "/" << get<2>(data[i]) << ", ";
        graph.insertEdge(get<0>(data[i]), get<1>(data[i]), get<2>(data[i]));
    }

    //graph.printGraph();
    graph.numEdges();

    return 0;
}

//