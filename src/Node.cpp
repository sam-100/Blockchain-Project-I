#include "Node.hh"
#include "utils.hh"

#include <iostream>

using namespace std;


int Node::cnt = 0;
Node::Node(bool low, bool slow) {
    id = cnt++;
    this->low = low;
    this->slow = slow;
    balance = vector<currency>(num_peers, 100);
}

void create_topology(int node_cnt) {
    for(int i=0; i<node_cnt; i++)
    {
        Node node(random_int(0, 100) <= low, random_int(0, 100) <= slow);
        nodes.push_back(node);
    }
    
    while(!is_connected())
    {
        clear_network();
        
        for(Node node : nodes)
        {
            if(node.peers.size() >= MIN_DEGREE)
                continue;
            int peer;
            do {
                peer = random_int(0, nodes.size());
            } while(nodes[peer].peers.size() >= MAX_DEGREE);

            connect_nodes(node.id, peer);
        }
    }
}

bool is_connected() {
    vector<bool> visited(nodes.size(), false);

    visit_dfs(0, visited);
    
    for(int i=0; i<visited.size(); i++)
        if(visited[i] == false)
            return false;
    return true;
}

void visit_dfs(int curr, vector<bool> &visited) {
    visited[curr] = true;
    for(int peer : nodes[curr].peers)
        if(!visited[peer])
            visit_dfs(peer, visited);
    return;
}

void clear_network() {
    for(int i=0; i<nodes.size(); i++)
        nodes[i].peers.clear();
}

void connect_nodes(int node_this, int node_that) {
    nodes[node_this].peers.insert(node_that);
    nodes[node_that].peers.insert(node_this);
}

void print_network() {
    for(Node node : nodes) {
        cout << node.id << ": ";
        for(int peer : node.peers)
            cout << peer << " | ";
        cout << endl;
    }
}

void print_edges() {
    for(Node node : nodes)
        for(int peer : node.peers)
            cout << node.id << " " << peer << endl;
}

ostream &operator<<(ostream &out, const Node &node) {
    out << "Node {" << endl;
    out << "\tid : " << node.id << endl;
    out << "\tlow : " << node.low << endl;
    out << "\tslow : " << node.slow << endl;
    out << "\tpeers : " << node.peers << endl;
    out << "\tbalance : " << node.balance << endl;
    out << "}" << endl;
    return out;
}