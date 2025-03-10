#include "log.hh"
#include "Node.hh"
#include <fstream>
#include "utils.hh"

using namespace std;


void log_topology() {
    ofstream file("log/topology");

    file << "-----------*** The Network Topology ***-----------------" << endl << endl;

    for(Node node : nodes) {
        file << "Node " << node.id << ": " << node.peers << endl;
    }
    file.close();
}

void log_nodes() {
    ofstream file("log/nodes");

    file << "-----------*** The Node instances ***-----------------" << endl << endl;

    for(Node node : nodes)
        file << node << endl;
    
    file.close();
}

void log(const char* file, const char* msg) {
    
}