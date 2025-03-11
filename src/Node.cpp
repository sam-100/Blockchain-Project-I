#include "Node.hh"
#include "utils.hh"
#include "Transaction.hh"
#include "Event.hh"
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

void forward_txn(int n_id, Transaction *txn) {
    Node &node = nodes[n_id];
    if(node.visited_txns.find(txn->id) != node.visited_txns.end())
        return;

    /* validate the transaction and mark as visited */
    if(node.balance[txn->sender] < txn->amount)
        return;
    node.balance[txn->sender] -= txn->amount;
    node.balance[txn->receiver] += txn->amount;
    node.visited_txns.insert(txn->id);
    node.mem_pool.push_back(txn);

    for(int peer : node.peers)
        event_queue.push(new TxnRecvEvent(global_time + 10, peer, txn));
    return;    
}

// 1. update and validate mem-pool
// 2. re-evaluate balance vector

/*
1. 45
2. 54
3. 23 
4. 17

23, 54

*/