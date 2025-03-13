#include "Node.hh"
#include "utils.hh"
#include "Transaction.hh"
#include "Event.hh"
#include "Block.hh"
#include <iostream>
#include <math.h>

using namespace std;


int Node::cnt = 0;

Node::Node(bool low, bool slow, Block *gen) {
    id = cnt++;
    this->low = low;
    this->slow = slow;
    this->mining = false;
    balance = vector<currency>(num_peers, 100);
    genesis = gen;
    blockchain = new Blockchain(genesis);
}

void create_topology(int node_cnt) {
    nodes = new vector<Node>();
    Block *genesis = new Block(nullptr, -1);
    for(int i=0; i<node_cnt; i++)
    {
        Node node(random_int(0, 100) <= low, random_int(0, 100) <= slow, genesis);
        nodes->push_back(node);
    }
    
    while(!is_connected())
    {
        clear_network();
        
        for(Node node : *nodes)
        {
            if(node.peers.size() >= MIN_DEGREE)
                continue;
            int peer;
            do {
                peer = random_int(0, nodes->size());
            } while(nodes->at(peer).peers.size() >= MAX_DEGREE);

            connect_nodes(node.id, peer);
        }
    }
}

bool is_connected() {
    vector<bool> visited(nodes->size(), false);

    visit_dfs(0, visited);
    
    for(int i=0; i<visited.size(); i++)
        if(visited[i] == false)
            return false;
    return true;
}

void visit_dfs(int curr, vector<bool> &visited) {
    visited[curr] = true;
    for(int peer : nodes->at(curr).peers)
        if(!visited[peer])
            visit_dfs(peer, visited);
    return;
}

void clear_network() {
    for(int i=0; i<nodes->size(); i++)
        nodes->at(i).peers.clear();
}

void connect_nodes(int i, int j) {
    nodes->at(i).peers.insert(j);
    nodes->at(j).peers.insert(i);
    
    clock_time pd = random_float(0.010, 0.500);
    prop_delay[i][j] = prop_delay[j][i] = pd;
}

void print_network() {
    for(Node node : *nodes) {
        cout << node.id << ": ";
        for(int peer : node.peers)
            cout << peer << " | ";
        cout << endl;
    }
}

void print_edges() {
    for(Node node : *nodes)
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
    out << "\tlast-block: " << node.blockchain->get_last_blk()->id << " " << endl;
    out << "}" << endl;
    return out;
}

void forward_txn(int n_id, Transaction *txn) {
    Node &node = nodes->at(n_id);
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
    
    if(node.mining == false && node.mem_pool.size() >= 1023)
    {
        node.mining = true;
        event_queue.push(new BlockMinedEvent(global_time + 20, n_id, node.blockchain->get_last_blk()));
    }

    return;    
}


/* Create a new block, and send it to the peers */
void add_block(int n_id, Block *blk) {
    // push first 1023 txns from mem_pool to blk
    Node &node = nodes->at(n_id);
    for(int i=0; i<1023; i++)
    {
        blk->txn_list.push_back(node.mem_pool.front());
        node.mem_pool.pop_front();
    }
    // node.blockchain.add(blk);
}

void mine_block(int n_id, Block *prev) {
    Node &node = nodes->at(n_id);
    if(node.blockchain->get_last_blk() != prev)
    {
        node.mining = false;
        return;
    }

    Block *blk = node.create_block();
    node.add_block(blk);
    node.forward(blk);
    node.mining = false;
}

void Node::reset_mempool() {
    balance = blockchain->get_last_blk()->balance;
    list<Transaction*> buffer = mem_pool;
    mem_pool.clear();
    for(Transaction *txn : buffer)
        if(is_valid(txn))
            add_txn(txn);
}

void Node::add_block(Block *blk) {
    blockchain->insert(blk);
    if(blk == blockchain->get_last_blk())
        reset_mempool();
}

void block_recv(int n_id, Block *blk) {
    Node &node = nodes->at(n_id);
    
    if(!node.blockchain->contains(blk->prev_blk) || node.blockchain->contains(blk))
    {
        node.mining = false;
        return;
    }

    node.add_block(blk);
    node.forward(blk);


}


void forward_blk(int n_id, Block* blk) {
    Node &node = nodes->at(n_id);
    if(node.blockchain->get_last_blk() != blk->prev_blk) 
        return;
    if(node.blockchain->contains(blk))
        return;
    add_block(n_id, blk);
    for(int peer : node.peers)
        event_queue.push(new BlockRecvEvent(global_time+10, peer, blk));    
}

void transaction_send(int n_id) {
    Node &node = nodes->at(n_id);
    float bal = node.balance[n_id];
    Transaction *txn = new Transaction(n_id, random_int(0, nodes->size()), random_float(0, bal));
    node.add_txn(txn);
    node.forward(txn);
    node.start_mining();
}

void transaction_recv(int n_id, Transaction *txn) {
    Node &node = nodes->at(n_id);
    if(!node.is_valid(txn) || node.visited(txn))
        return;
    node.add_txn(txn);
    node.forward(txn);
    node.start_mining();
}

void Node::add_txn(Transaction *txn) {
    mem_pool.push_back(txn);
    balance[txn->sender] -= txn->amount;
    balance[txn->receiver] += txn->amount;
    visited_txns.insert(txn->id);
    return;
}

void Node::start_mining() {
    if(mining || mem_pool.size() < BLOCK_SIZE)
        return;
    mining = true;
    clock_time mining_time = random_exp_float(BLOCK_INTV_TIME/h_fraction());
    event_queue.push(new BlockMinedEvent(global_time+mining_time, id, blockchain->get_last_blk()));
}

double Node::h_fraction() const {
    double hp = low ? 1 : 10;
    double thp = 0;
    for(Node node : *nodes)
        thp += node.low ? 1 : 10;
    return hp/thp;
}

void Node::forward(Transaction *txn) const {
    for(int peer : peers)
    {
        clock_time latency = get_latency(peer, pow(2, 10)*8);
        event_queue.push(new TxnRecvEvent(global_time + latency, id, txn));
    }
}

void Node::forward(Block *blk) const {
    for(int peer : peers)
    {
        clock_time latency = get_latency(peer, pow(2, 20)*8);
        event_queue.push(new BlockRecvEvent(global_time + latency, peer, blk));
    }
}

bool Node::visited(Transaction *txn) const {
    return visited_txns.find(txn->id) != visited_txns.end();
}

bool Node::is_valid(Transaction *txn) const {
    return balance[txn->sender] >= txn->amount;
}

Block *Node::create_block() {
    Block *blk = new Block(blockchain->get_last_blk(), id);
    for(int i=0; i<BLOCK_SIZE; i++)
    {
        blk->add_txn(mem_pool.front());
        mem_pool.pop_front();
    }
    return blk;
}


clock_time Node::get_latency(int peer, int size) const {
    clock_time pd = prop_delay[id][peer];
    double link_speed = ((nodes->at(peer).slow || slow) ? 5 : 100)*pow(2, 20);
    clock_time q_delay = random_exp_float(96*pow(2, 10)/link_speed);
    
    return pd + size/link_speed + q_delay;
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