#include "log.hh"
#include "Node.hh"
#include <fstream>
#include "utils.hh"
#include <string.h>
#include <Transaction.hh>
#include "Event.hh"

using namespace std;


void log_topology() {
    ofstream file("log/topology");

    file << "-----------*** The Network Topology ***-----------------" << endl << endl;

    for(Node node : *nodes) {
        file << "Node " << node.id << ": " << node.peers << endl;
    }
    file.close();
}

void log_nodes() {
    ofstream file("log/nodes");

    file << "-----------*** The Node instances ***-----------------" << endl << endl;
    for(Node node : *nodes)
        file << node << endl;    
    file.close();
}

void log(const char* file, const char* msg) {
    
}

void log_event(Event *ev) {
    int n_id = ev->n_id;
    string filename = "log/events/node-" + to_string(n_id);
    ofstream event_log(filename, std::ios::app);
    event_log << ev << endl;
    return;
}

void log_mempools() {
    for(Node node : *nodes)
    {
        string filename = "log/mempool/node-" + to_string(node.id);
        ofstream mem_pool(filename);
        mem_pool << "---------------*** Mem-pool ***------------------" << endl << endl;
        for(Transaction *txn : node.mem_pool)
            mem_pool << *txn << endl;
        mem_pool.close();
    }
}

void log_blockchains() {
    for(Node node : *nodes)
    {
        string filename = "log/blockchain/node-" + to_string(node.id);
        ofstream file(filename);
        file << "----------------------*** Blockchain ***--------------------------" << endl;
        file << node.blockchain;
        file.close();
    }
}

void log_statistics() {
    int malicious_cnt = 0;
    for(Node node : *nodes) {
        malicious_cnt += node.malicious;
    }
    
    ofstream file("log/stats");
    file << "-------------------------*** Network statistics ***-------------------------" << endl;
    file << "Malicious nodes: " << malicious_cnt << "/" << num_peers << " (" << (malicious_cnt*100)/num_peers << "% ) " << endl;
    file.close();
}