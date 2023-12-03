#include "IntersectionGraph.h"


/******************************MACROS******************************/
const double PI = 3.14159265358979323;
const int EARTH_RADIUS_M = 6371000;

const int MIN_RADIUS = 50;
const int MAX_RADIUS = 500;

/****************************CONSTRUCTOR***************************/
IntersectionGraph::IntersectionGraph() {
    std::unordered_map<std::string, std::vector<std::string>> tempEdgeList; //Maps id to vector of linked ids
    std::unordered_map<std::string, Node> tempNodeMap;
    std::vector<std::string> tempIdVect;

    idVect = tempIdVect;
    edgeList = tempEdgeList;
    nodeMap = tempNodeMap;
    totalDeleted = 0;
    totalLinked = 0;
}

/*************************HELPER FUNCTIONS*************************/
void IntersectionGraph::InsertNode(std::string id, double lat, double lon) {
    Node currNode;
    currNode.lat = lat;
    currNode.lon = lon;

    std::vector<std::string> tempVect;

    if (nodeMap.find(id) == nodeMap.end()) {
        idVect.push_back(id);
        nodeMap[id] = currNode;
        edgeList[id] = tempVect;
    }
}

double IntersectionGraph::DirectDistanceBetween(std::string id1, std::string id2) {
    //Check if ids exist
    if (nodeMap.find(id1) == nodeMap.end() || nodeMap.find(id1) == nodeMap.end())
        return -1;

    //If both ids do exist, calculate distance using haversine formula
    double phi1 = nodeMap[id1].lat * PI / 180;
    double phi2 = nodeMap[id2].lat * PI / 180;

    double dPhi = (nodeMap[id2].lat - nodeMap[id1].lat) * PI / 180;
    double dLambda = (nodeMap[id2].lon - nodeMap[id1].lon) * PI / 180;

    double a = sin(dPhi / 2) * sin(dPhi / 2);
    double b = cos(phi1) * cos(phi2) * sin(dLambda / 2) * sin(dLambda / 2);
    double c = a + b;
    double d = 2 * atan(sqrt(c) / sqrt(1 - c));

    return d * EARTH_RADIUS_M;
}

void IntersectionGraph::DeleteCloseNodes() {
    std::vector<std::string> tempIdVect;
    for (const auto & i : idVect) {
        for (const auto & j : idVect) {
            if (DirectDistanceBetween(i, j) < MIN_RADIUS && i != j) {
                //idVect.erase(j);
                nodeMap.erase(j);
                edgeList.erase(j);
                totalDeleted ++;
            } else {
                tempIdVect.push_back(j);
            }
        }
        idVect = tempIdVect;
    }
}

void IntersectionGraph::LinkAllNodes() {
    for (const auto & i : idVect) {
        for (const auto & j : idVect) {
            if (DirectDistanceBetween(i, j) < MAX_RADIUS && i != j) {
                edgeList[i].push_back(j);
                totalLinked++;
            }
        }
    }
}

/*************************PUBLIC FUNCTIONS*************************/
void IntersectionGraph::PopulateFromFile(std::string filename) {
    std::string fileLine;
    std::ifstream infile(filename);

    while (std::getline(infile, fileLine)) {
        //Create stringstream object for line in file
        std::stringstream ss(fileLine);
        std::string fileSubstr;


        while (std::getline(ss, fileSubstr, ' ')) {
            //Check if line has node information
            if (fileSubstr == "<node") {
                //'id = "'
                std::getline(ss, fileSubstr, '\"');

                //Get id
                std::getline(ss, fileSubstr, '\"');
                std::string id = fileSubstr;

                //'" lat = "'
                std::getline(ss, fileSubstr, '\"');

                //Get lat
                std::getline(ss, fileSubstr, '\"');
                double lat = std::stod(fileSubstr);

                //'" lon = "'
                std::getline(ss, fileSubstr, '\"');

                //Get lon
                std::getline(ss, fileSubstr, '\"');
                double lon = std::stod(fileSubstr);

                //Create node
                InsertNode(id, lat, lon);
            }
        }
    }

    //Delete nodes less than 50m away
    DeleteCloseNodes();

    //Link all nodes within 500m
    LinkAllNodes();
}

void IntersectionGraph::PrintNodes() {
    for (const auto & i : idVect) {
        std::cout << i << ": (";
        std::cout << std::fixed << std::setprecision(7) << nodeMap[i].lat << ", ";
        std::cout << std::fixed << std::setprecision(7) << nodeMap[i].lon << ")\n";
    }
    std::cout << "Total deleted: " << totalDeleted << '\n';
    std::cout << "Total links: " << totalLinked;
}

void IntersectionGraph::WriteNodesToFile(std::string filename) {
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        for (const auto & i : idVect) {
            outfile << i << ": (";
            outfile << std::fixed << std::setprecision(7) << nodeMap[i].lat << ", ";
            outfile << std::fixed << std::setprecision(7) << nodeMap[i].lon << ")\n";
        }
    } else std::cout << "ERROR";
}