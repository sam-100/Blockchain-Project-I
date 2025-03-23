#include "Node.hh"
#include "utils.hh"
#include "Transaction.hh"
#include "Event.hh"
#include "Block.hh"
#include <iostream>
#include <math.h>

using namespace std;


int Node::cnt = 0;

Node::Node(bool malicious, Block *gen) {
    id = cnt++;
    this->malicious = malicious;
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
        Node node(random_int(0, 100) < malicious, genesis);
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
            } while(nodes->at(peer).peers.size() >= MAX_DEGREE || peer == node.id);

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
    out << "Node {" << ", " << endl;
    out << "\tid : " << node.id << ", " << endl;
    out << "\tmalicious : " << btoa(node.malicious) << ", " << endl;
    out << "\tpeers : " << node.peers << ", " << endl;
    out << "\tbalance : " << node.blockchain->get_last_blk()->balance.at(node.id) << ", " << endl;
    out << "\tlast-block: " << node.blockchain->get_last_blk()->id << ", " << endl;
    out << "\tforks: " << node.blockchain->tail_blks.size() << ", " << endl;
    out << "}" << endl;
    return out;
}


/* Create a new block, and send it to the peers */
void Node::mine_block_event(Block *prev) {
    if(blockchain->get_last_blk() != prev)
    {
        mining = false;
        return;
    }

    Block *blk = create_block();
    add_block(blk);
    // broadcast(blk);
    broadcast(blk->get_hash());
    mining = false;
}

void Node::broadcast(string hash) const {
    for(int peer : peers)
        send(hash, peer);
}

void Node::send(string hash, int peer) const {
    clock_time latency = get_latency(peer, 64);
    event_queue.push(new HashRecvEvent(global_time + latency, peer, hash, id));
}

void Node::hash_recv_event(string hash, int sender) {
    // 1. If the corresponding block is received already, discard and return.
    if(blockchain->contains_hash(hash))
        return;

    // 2. If the time-out corresponding to the hash is running, add the node to pending_requests.
    if(wait_list.find(hash) != wait_list.end())
    {
        wait_list[hash].push_back(sender);
        return;
    }

    // 3. Else - add the hash to pending requests and send the BlockRecv request
    request(hash, sender);
    wait_list[hash].push_back(sender);
}

void Node::request(string hash, int sender) const {
    clock_time latency = get_latency(sender, 64);
    event_queue.push(new BlockGetReqEvent(global_time + latency, sender, hash, id));        // sending "get" request to the sender node
    event_queue.push(new TimeOutEvent(global_time + timeout_time, id, hash, sender));       // adding timeout time
}

void Node::timeout_event(string hash, int sender) {
    // 1. If block is received, then erase the queue and return.
    if(blockchain->contains_hash(hash))         
    {
        wait_list.erase(hash);
        return;
    }
    
    // 2. Pop the next peer from queue, and send it the get request
    if(wait_list[hash].empty())
        return;
    int next = wait_list[hash].front();
    wait_list[hash].pop_front();
    request(hash, next);
}

void Node::block_get_event(int sender, string hash) {
    if(malicious && !nodes->at(sender).malicious)
        return;
    send(blockchain->get_blk(hash), sender);
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
    wait_list.erase(blk->get_hash());
}

void Node::block_recv_event(Block *blk) {    
    if(!blockchain->contains(blk->prev_blk) || blockchain->contains(blk))
    {
        mining = false;
        return;
    }
    add_block(blk);
    // broadcast(blk);
    broadcast(blk->get_hash());
}


void Node::txn_send_event() {
    float bal = balance[id];
    Transaction *txn = new Transaction(id, random_int(0, nodes->size()), random_float(0, bal));
    add_txn(txn);
    broadcast(txn);
    start_mining();
}

void Node::txn_recv_event(Transaction *txn) {

    if(!is_valid(txn) || visited(txn))
        return;
    add_txn(txn);
    broadcast(txn);
    start_mining();
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
    return (double)1/(double)num_peers;
}

void Node::broadcast(Transaction *txn) const {
    for(int peer : peers)
        send(txn, peer);
}

void Node::send(Transaction *txn, int peer) const {
    clock_time latency = get_latency(peer, pow(2, 10)*8);
    event_queue.push(new TxnRecvEvent(global_time + latency, id, txn));
}

void Node::broadcast(Block *blk) const {
    for(int peer : peers)
        send(blk, peer);
}

void Node::send(Block *blk, int peer) const {
    clock_time latency = get_latency(peer, pow(2, 20)*8);
    event_queue.push(new BlockRecvEvent(global_time + latency, peer, blk));
}

bool Node::visited(Transaction *txn) const {
    return visited_txns.find(txn->id) != visited_txns.end();
}

bool Node::is_valid(Transaction *txn) const {
    return balance[txn->sender] >= txn->amount;
}

Block *Node::create_block() {
    Block *blk = new Block(blockchain->get_last_blk(), id, malicious);
    for(int i=0; i<BLOCK_SIZE; i++)
    {
        blk->add_txn(mem_pool.front());
        mem_pool.pop_front();
    }
    return blk;
}


clock_time Node::get_latency(int peer, int size) const {
    clock_time pd = prop_delay[id][peer];
    double link_speed = ((nodes->at(peer).malicious && malicious) ? 100 : 5)*pow(2, 20);
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