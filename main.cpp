#include "IntersectionGraph.h"

int main() {
    IntersectionGraph myGraph;
    myGraph.PopulateFromFile("overPassToC.txt");
    myGraph.PrintNodes();
    myGraph.WriteNodesToFile("CToGoogle.txt");

    return 0;
}
