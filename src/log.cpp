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


void log_topology_m() {
    ofstream file("log/topology_m");

    file << "-----------*** The Overlay Network Topology ***-----------------" << endl << endl;

    for(Node node : *nodes) {
        if(!node.malicious)
            continue;
        file << "M_Node " << node.id << ": " << node.peers_m << endl;
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

#include <iostream>

void log_blockchain_graphs() {
    for(Node node : *nodes)
    {
        string filename = "log/blockchain-graph/node-" + to_string(node.id);
        ofstream file(filename);
        file << node.blockchain->to_edge_string();
        file.close();
    }
}

void log_statistics() {
    int avg_forks = 0;
    for(Node node : *nodes) {
        avg_forks += node.blockchain->tail_blks.size();
    }
    avg_forks /= nodes->size();
    
    ofstream file("log/stats");
    file << "-------------------------*** Network statistics ***-------------------------" << endl;
    file << "Malicious nodes: " << mal_nodes.size() << "/" << num_peers << " (" << (mal_nodes.size()*100)/num_peers << "% ) " << endl;
    file << "Average forks: " << avg_forks << endl;
    file << "Eclipsed honest nodes: " << eclipsed_nodes() << endl;
    file << "Malicious blocks = " << malicious_block_cnt << endl;
    file.close();
}

vector<int> eclipsed_nodes() {
    vector<int> arr;
    for(Node node : *nodes)
    {
        if(node.malicious)
            continue;
        bool flag = true;
        for(int peer : node.peers)
        {
            if(!nodes->at(peer).malicious)
            {
                flag = false;
                break;
            }
        }
        if(flag)
            arr.push_back(node.id);
    }
    return arr;
}